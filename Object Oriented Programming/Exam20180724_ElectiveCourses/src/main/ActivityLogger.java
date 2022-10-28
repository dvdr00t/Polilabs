package main;

import java.time.Instant;

import it.polito.oop.elective.Notifier;

/**
 * Public display mockup implementation of notifier that logs the activities on console. 
 * 
 * This is a sample implementation of the {@link it.polito.oop.elective.Notifier}
 * interface that prints the notifications to the standard output.
 *  
 */
public class ActivityLogger implements Notifier {
    
    @Override
    public void assignedToCourse(String id, String course) {
        System.out.print(Instant.now() + " : student " + id + " succesfully enrolled to course " + course);
    }

    @Override
    public void requestReceived(String id) {
        System.out.print(Instant.now() + " : Enrollement Request received for student " + id );
    }

}
