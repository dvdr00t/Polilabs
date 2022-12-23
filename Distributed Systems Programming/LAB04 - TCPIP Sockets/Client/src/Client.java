import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class Client {

    /* --- ATTRIBUTES --- */
    private static final int CHUNK_LENGTH = 1024;
    private static final int TIMEOUT = 3000;

	Socket clientSocket = null;
	private DataOutputStream outputStream = null; 
	private DataInputStream inputStream = null;



    /* --- METHODS --- */
	
	/**
	 * Constructor of the Client class.
	 * @param serverAdress the server address
	 * @param serverPort the server port
	 * @throws IOException
	 */
	private Client(InetAddress serverAdress, int serverPort) throws IOException {
		this.clientSocket = new Socket(serverAdress, serverPort);
		this.inputStream = new DataInputStream(clientSocket.getInputStream());
		this.outputStream = new DataOutputStream(clientSocket.getOutputStream());
		this.clientSocket.setSoTimeout(TIMEOUT);
	}

	/**
	 * Send a request of conversion to the server.
	 * @param inputMediaType the source media type.
	 * @param outputMediaType the destination media type.
	 * @param imagePathString the image path in the client filesystem.
	 * @throws IOException
	 */
	private void sendRequest(String inputMediaType, String outputMediaType, String imagePathString) throws IOException {

		/* prepare input stream */
		InputStream inputSocketStream = this.clientSocket.getInputStream();

		/* prepare file to stream */
		File file = new File("image/" + imagePathString);
		FileInputStream fin = new FileInputStream("image/" + imagePathString);
		int fileSizeToSend = (int) file.length();
		int fileSizeToRead = 0;
        int count = 0;
        byte[] fileByteArray = new byte[CHUNK_LENGTH];
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
		
		/* sending formats */
		byte[] inputBytes = inputMediaType.getBytes(StandardCharsets.US_ASCII);
		byte[] outputBytes = outputMediaType.getBytes(StandardCharsets.US_ASCII);
		this.outputStream.write(inputBytes);
		this.outputStream.write(outputBytes);
		System.out.println("[+] INFO: the information about the media types has been sent.");
			
		/* sending file size */
		this.outputStream.writeInt(fileSizeToSend);
		System.out.println("[+] INFO: the information about the file length has been sent.");
		
		/* sending actual file */
		while ((count = fin.read(fileByteArray)) > 0) {
			this.outputStream.write(fileByteArray, 0, count);
		}
		this.outputStream.flush();
		fin.close();
		System.out.println("[+] the file has been sent.");
	   
		/* reading feedback string */
		char isSuccess = (char) inputSocketStream.read();
		System.out.println("[+] the response code has been received.");

		/* evaluating result */
		switch (isSuccess) {
			case '0':
				fileSizeToRead = inputStream.readInt();
				int bytesToRead = fileSizeToRead;

				/* reading file chunks, until less than CHUNK_LENGTH bytes remain */
				while (fileSizeToRead > CHUNK_LENGTH) {
					int readBytes = inputStream.read(fileByteArray, 0, CHUNK_LENGTH);
					baos.write(fileByteArray, 0, readBytes);
					bytesToRead -= readBytes;
					fileSizeToRead = bytesToRead;
					fileByteArray = new byte[CHUNK_LENGTH];
				}
				
				/* reading last chunk */
				while (bytesToRead > 0) {
					int readBytes = inputStream.read(fileByteArray, 0, bytesToRead);
					baos.write(fileByteArray, 0, readBytes);
					bytesToRead -= readBytes;
					fileByteArray = new byte[CHUNK_LENGTH];
				}			 

				/* saving received file in the filesystem */
				try (OutputStream outputStream = new FileOutputStream("image/output." + outputMediaType.toLowerCase())) {
					baos.writeTo(outputStream);
					outputStream.close();
				}
				System.out.println("[+] INFO: the converted file has been received.");

				break;
			case '1': 
				System.err.println("[-] ERROR: invalid request.");
				break;

			case '2': 
				System.err.println("[-] ERROR: generic server error.");
				break;
		
			default:
				System.err.println("[-] ERROR: invalid code response.");
				break;
		}
	}

	/**
	 * Main driver of the client side.
	 * @param args list of arguments passed from command line.
	 * @throws IOException
	 */
    public static void main(String[] args) throws IOException {

        /* checking arguments passed through command line */
        if (args.length != 3) {
            System.err.println("[-] USAGE: java -jar client.jar <original_type> <target_type> <image_path>");
            System.exit(1);
        } else if (!args[0].matches("[A-Z]{3}") || !args[1].matches("[A-Z]{3}")) {
			System.err.println("[-] USAGE: <original_type> and <target_type> must be 3 uppercase letters each.");
			System.exit(1);
		}

		/* parsing arguments and opening socket */
		String serverAddress = "0.0.0.0";
		int serverPort = 2001;
		Client client = null;
		String inputMediaType = args[0];
		String outputMediaType = args[1];
		String imagePathString = args[2];
		try {
			client = new Client(InetAddress.getByName(serverAddress), serverPort);
			System.out.printf("[+] INFO: connected to the server %s:%d\n", serverAddress, serverPort);
			System.out.printf("[+] INFO: received: %s %s %s\n", inputMediaType, outputMediaType, imagePathString);
		} catch (Exception e) {
			System.err.println("[-] ERROR: connection to the server failed");
			e.printStackTrace();
			System.exit(1);
		}

		/* sending data */
		try {
			client.sendRequest(inputMediaType, outputMediaType, imagePathString);
		} catch (IOException e) {
			System.err.println("[-] ERROR: connection to the server failed");
			e.printStackTrace();
			System.exit(1);
		}

    }
    
}
