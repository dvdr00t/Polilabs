package it.polito.po.test;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import it.polito.oop.elective.ElectiveException;
import it.polito.oop.elective.ElectiveManager;
import it.polito.oop.elective.Notifier;

public class TestR4_Notifiche {

    private ElectiveManager manager;
    private Listener l;
    
    /**
     * This is a fake notification listener that just
     * records the notifications so the test can check
     * they were sent out as expected.
     *
     */
    private static class Listener implements Notifier {

        Map<String,String> assignments = new LinkedHashMap<>();
        @Override
        public void assignedToCourse(String id, String course) {
            assignments.put(id, course);
        }

        List<String> requests = new LinkedList<>();
        @Override
        public void requestReceived(String id) {
            requests.add(id);
        }
        
    }
    
    @Before
    public void setUp() {
        manager = new ElectiveManager();
        manager.addCourse("VIQ", 90);
        manager.addCourse("AI", 70);
        manager.addCourse("OR", 180);
        manager.addCourse("XY", 120);
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S33", 25);
        manager.loadStudent("S21", 21.3);
        manager.loadStudent("S5", 28.2);
        l = new Listener();
        manager.addNotifier(l);
    }

    @Test
    public void testRequest() throws ElectiveException {
        manager.requestEnroll("S1", Arrays.asList("OR","AI"));
        assertEquals("No notification sent",1,l.requests.size());
        assertEquals("Wrong student id for notification ","S1",l.requests.get(0));
    }

    @Test
    public void testRequest2() throws ElectiveException {
        manager.requestEnroll("S1", Arrays.asList("OR","AI"));
        manager.requestEnroll("S2", Arrays.asList("AI","VIQ"));
        manager.requestEnroll("S5", Arrays.asList("OR","AI"));
        assertEquals("Missing notifications ",3,l.requests.size());
        assertEquals("Wrong student id for notification ","S1",l.requests.get(0));
        assertEquals("Wrong student id for notification ","S2",l.requests.get(1));
        assertEquals("Wrong student id for notification ","S5",l.requests.get(2));
    }

    @Test
    public void testAssign() throws ElectiveException {
        manager.requestEnroll("S1", Arrays.asList("OR","AI"));
        manager.requestEnroll("S2", Arrays.asList("AI","VIQ"));
        manager.makeClasses();
        
        assertEquals("Missing notification",2,l.assignments.size());
        assertTrue("No notification for student S1",l.assignments.containsKey("S1"));
        assertTrue("No notification for student S1",l.assignments.containsKey("S2"));
        assertEquals("Wrong course  notification ","OR",l.assignments.get("S1"));
        assertEquals("Wrong course notification ","AI",l.assignments.get("S2"));
    }

}
