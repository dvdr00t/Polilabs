import java.io.IOException;

public class Server {

    /* --- ATTRIBUTES --- */
    public static final int PORT = 2001;

    /**
     * Main driver of the server
     * @param args list of arguments (should be empty)
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {

        /* creating the server socket */
        Converter converter = null;
        try {
            converter = new Converter(PORT);
        } catch (IOException e) {
            System.err.println("[-] ERROR: an error occurred while trying to create the socket.\n");
            e.printStackTrace();
            System.exit(0);
        }

        /* managing the communication with the client */
        System.out.println("[+] INFO: starting server on port: " + PORT);
        try {
            converter.execute();
        } catch (IOException e) {
            System.err.println("[-] ERROR: an error occurred while trying to executing the server socket.\n");
            e.printStackTrace();

            /* managing the closing of the server socket */
            try {
                converter.destroy();
            } catch (IOException ex) {
                System.err.println("[-] ERROR: an error occurred while trying to destroying the server socket.\n");
                ex.printStackTrace();
            }
        }
    }
}
