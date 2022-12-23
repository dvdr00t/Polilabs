import java.net.Socket;
import java.io.IOException;
import java.net.ServerSocket;       
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * It implements the TCP/IP Sockets used to convert the images
 */
public class Converter {

    /* --- ATTRIBUTES --- */
    private ServerSocket serverSocket = null;
    private ExecutorService threadPool = null;


    /* --- CONSTRUCTOR --- */
    
    /**
     * Constructor of the converter socket
     * @param port the port in which the server is listening to.
     * @throws IOException
     */
    public Converter(final int port) throws IOException {
        this.serverSocket = new ServerSocket(port);
    }


    /* --- METHODS --- */

    /**
     * Starts the execution of the service that manages the connection requests
     * @throws IOException
     */
    public void execute() throws IOException {

        /* creating new thread pool */
        this.threadPool = Executors.newCachedThreadPool();

        /* filling the pool with new incoming connection */
        while (true) {
            Socket socket = this.serverSocket.accept();
            this.threadPool.submit(new ConversionHandler(socket));
        }
    }


    /**
     * Managing the end of the connection
     * @throws IOException
     */
    public void destroy() throws IOException {

        /* managing connection and threads */
        this.threadPool.shutdown();
        this.serverSocket.close();
    }
}
