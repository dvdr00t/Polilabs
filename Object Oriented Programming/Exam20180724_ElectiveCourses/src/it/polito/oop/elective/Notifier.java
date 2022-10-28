package it.polito.oop.elective;

/**
 * Listener interface that must be implemented by
 * classes intended to show the status of the
 * waiting queue management system
 * <p>
 * A typical class implementing this interface
 * will show the notification on a display panel.
 *
 */
public interface Notifier {
    /**
     * Method invoked when a new ticket is going
     * to be served by a given counter.
     * 
     * @param ticketNo the number of the ticket to be served
     * @param counterId the counter that will serve the ticket 
     */
    void assignedToCourse(String id, String course);
    
    /**
     * Method called to provide updated information
     * about the accepted enrollment request.
     * 
     * @param id   student id
     */
    void requestReceived(String id);
}
