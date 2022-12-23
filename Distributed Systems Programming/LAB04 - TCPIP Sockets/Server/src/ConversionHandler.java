import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;

import javax.imageio.ImageIO;

public class ConversionHandler implements Runnable {

    /* --- ATTRIBUTES --- */
    private static final int CHUNK_LENGTH = 1024;
    private static final int TIMEOUT = 3000;
    private final int TYPE_LENGTH = 3;
    private final String[] _valid_mediatype = {"JPG", "PNG", "GIF"};
    private final List<String> VALID_MEDIATYPE = Arrays.asList(_valid_mediatype);

    Socket socket = null;
    DataInputStream inputStream = null;
    DataOutputStream outputStream = null;
    boolean success = false;
    String imageTypeSource = null;
    String imageTypeDestination = null;

    /* response data */
    String errorMessage = null;
    int responseCode = 0;
    int fileLength = 0;

    /* --- CONSTRUCTOR --- */

    /**
     * COnstructor of the ConversionHandler class.
     * @param socket the new Socket accepted by the Server
     * @throws IOException
     */
    public ConversionHandler(Socket socket) throws IOException {
        this.socket = socket;

        /* setting up socket */
        try {
            this.socket.setSoTimeout(TIMEOUT);
            this.inputStream = new DataInputStream(this.socket.getInputStream());
            this.outputStream = new DataOutputStream(this.socket.getOutputStream());
            this.success = true;
        } catch (SocketException se) {
            this.errorMessage = new String("Error in setting the timeout for the socket.");
            this.responseCode = 2;
            this.success = false;
            se.printStackTrace();
        } catch (IOException ioe) {
            this.errorMessage = new String("Error in accessing the socket I/O streams.");
            this.responseCode = 2;
            this.success = false;
            ioe.printStackTrace();
        }
    }

    
    @Override
    public void run() {

        /* checking if the streams are configured correctly */
        if (this.success) {
            try {

                /* read metadata from the client */
                byte[] imageByteSource = new byte[3];
                byte[] imageByteDestination = new byte[3];
                int totalReadBytes = 0;

                /* reading source image type */
                while (totalReadBytes < this.TYPE_LENGTH) {
                    totalReadBytes += inputStream.read(imageByteSource, totalReadBytes, 3-totalReadBytes);
                }

                /* reading destination image type */
                totalReadBytes = 0;
                while (totalReadBytes < this.TYPE_LENGTH) {
                    totalReadBytes += inputStream.read(imageByteDestination, totalReadBytes, 3-totalReadBytes);
                }

                /* saving types into strings */
                this.imageTypeSource = new String(imageByteSource, StandardCharsets.US_ASCII);
                this.imageTypeDestination = new String(imageByteDestination, StandardCharsets.US_ASCII);
                System.out.println("[+] INFO: image media types have been received:");
                System.out.println("[+]       source media type: " + this.imageTypeSource);
                System.out.println("[+]       destination media type: " + this.imageTypeDestination);

                /* checks received media types */
                if (!VALID_MEDIATYPE.contains(this.imageTypeSource) || !VALID_MEDIATYPE.contains(this.imageTypeDestination)) {
                    this.success = false;
                    this.responseCode = 1;
                    this.errorMessage = new String("Media types not supported.");
                    System.out.println("[-] INFO: media type received is not supported.");
                }
                
            } catch (SocketException se) {
                this.errorMessage = new String("Timeout expired for reading the medatadata.");
                this.responseCode = 1;
                this.success = false;
                se.printStackTrace();
            } catch (IOException ioe) {
                this.errorMessage = new String("Error in receiving the metadata.");
                this.responseCode = 2;
                this.success = false;
                ioe.printStackTrace();
            }
        }

        /* checking result of metadata reading operation */
        if (this.success) {

            /* reading file length sent by the client */
            try {
                this.fileLength = this.inputStream.readInt();
                System.out.println("[+] INFO: the information about the file length has been received.");
            } catch (SocketException se) {
                this.errorMessage = new String("Timeout expired for receiving the file length.");
                this.responseCode = 1;
                this.success = false;
                se.printStackTrace();
            } catch (IOException ioe) {
                this.errorMessage = new String("Error in receiving the file length.");
                this.responseCode = 2;
                this.success = false;
                ioe.printStackTrace();
            }
        } 


        /* checking result of file length reading operation */
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        if (this.success) {

            /* reading actual file from the client */
            try {

                byte[] fileArray = new byte[CHUNK_LENGTH];
                int bytesToRead = fileLength;
                
                /* read file chunks, until less than CHUNK_LENGTH bytes remain */
                while (fileLength > CHUNK_LENGTH) {
                    int readBytes = this.inputStream.read(fileArray, 0, CHUNK_LENGTH);
                    baos.write(fileArray, 0, readBytes);
                    bytesToRead -= readBytes;
                    fileLength=bytesToRead;
                    fileArray = new byte[CHUNK_LENGTH];
                }

                /* read last chunk */
                while(bytesToRead > 0) {
                    int readBytes = this.inputStream.read(fileArray, 0, bytesToRead);
                    baos.write(fileArray, 0, readBytes);
                    bytesToRead -= readBytes;
                    fileArray = new byte[CHUNK_LENGTH];
                }
                System.out.println("[+] INFO: the file has been received.");

            } catch (SocketException se) {
                this.errorMessage = new String("Timeout expired for receiving the file.");
                this.responseCode = 1;
                this.success = false;
                se.printStackTrace();
            } catch (IOException ioe) {
                this.errorMessage = new String("Error in receiving the file.");
                this.responseCode = 2;
                this.success = false;
                ioe.printStackTrace();
            }
        }

        /* checking the result of file reading operation */
        ByteArrayOutputStream baosImageToSend = new ByteArrayOutputStream();
        if (this.success) {

            /* converting file */
            try {

                byte[] bytes = baos.toByteArray();
                ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
                BufferedImage imageReceived = ImageIO.read(bais);
                ImageIO.write(imageReceived, this.imageTypeDestination.toString().toLowerCase(), baosImageToSend); 
                System.out.println("[+] the file has been converted.");

            } catch (IOException ioe) {
                this.errorMessage = new String("Error during the image conversion.");
                this.responseCode = 2;
                this.success = false;
                ioe.printStackTrace();
            }
        }

        /* checking result of conversion operation */
        if (this.success) {

            /* sending back the file to the client */
            try {
                this.outputStream.write('0');
                int convertedLength = baosImageToSend.size();
                this.outputStream.writeInt(convertedLength);
                
                /* send the converted file */
                BufferedInputStream bisImageToSend = new BufferedInputStream(new ByteArrayInputStream(baosImageToSend.toByteArray()));
                byte[] buffer = new byte[CHUNK_LENGTH];
                int bytesToWrite;
                while ((bytesToWrite = bisImageToSend.read(buffer, 0, CHUNK_LENGTH)) != -1) {
                    this.outputStream.write(buffer, 0, bytesToWrite);
                    buffer = new byte[CHUNK_LENGTH];
                }
                System.out.println("[+] the converted file has been sent back.");
                
            } catch (SocketException se) {
                this.errorMessage = new String("Error in socket management while sending the positive response.");
                this.responseCode = 1;
                this.success = false;
                se.printStackTrace();
            } catch (IOException ioe) {
                this.errorMessage = new String("Error in sending the positive response.");
                this.responseCode = 2;
                this.success = false;
                ioe.printStackTrace();
            }
        } else {
            
            /* CASE OF ERRPR */
            try {

                /* sending the error code */
                if (this.responseCode == 1) {
                    this.outputStream.write('1');
                } else if (this.responseCode == 2) {
                    this.outputStream.write('2');
                }
                
                /* sending the length of the error message */
                int messageLength = this.errorMessage.length();
                this.outputStream.writeInt(messageLength);
                
                /* sending the actual error message */
                BufferedInputStream bisMessageToSend = new BufferedInputStream(new ByteArrayInputStream(this.errorMessage.getBytes()));
                byte[] buffer = new byte[CHUNK_LENGTH];
                int bytesToWrite;
                while ((bytesToWrite = bisMessageToSend.read(buffer, 0, CHUNK_LENGTH)) != -1) {
                    this.outputStream.write(buffer, 0, bytesToWrite);
                    buffer = new byte[CHUNK_LENGTH];
                }
                System.out.println("[+] ERROR: " + this.errorMessage);
                System.out.println("[+] ERROR: the error message has been sent back.");
                
            } catch (SocketException se) {
                this.errorMessage = new String("Error in socket management while sending the negative response.");
                se.printStackTrace();
            } catch (IOException ioe) {
                this.errorMessage = new String("Error in sending the negative response.");
                ioe.printStackTrace();
            }
        }  

        /* at the end, close the socket */
        try {
            this.socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        } 
    
    }
}
