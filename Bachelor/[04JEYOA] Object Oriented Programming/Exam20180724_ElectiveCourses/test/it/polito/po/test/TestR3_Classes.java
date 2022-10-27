package it.polito.po.test;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import it.polito.oop.elective.ElectiveException;
import it.polito.oop.elective.ElectiveManager;

public class TestR3_Classes {

    private ElectiveManager manager;
    private ElectiveManager manager0;
    
    @Before
    public void setUp() throws ElectiveException {
        manager0 = new ElectiveManager();
        manager0.addCourse("VIQ", 4);
        manager0.addCourse("AI", 2);
        manager0.loadStudent("S1", 29);
        manager0.loadStudent("S2", 28);
        manager0.loadStudent("S3", 27);
        manager0.loadStudent("S4", 26);
        manager0.requestEnroll("S1", Arrays.asList("VIQ","AI"));
        manager0.requestEnroll("S2", Arrays.asList("AI","VIQ"));
        manager0.requestEnroll("S3", Arrays.asList("AI","VIQ"));
        manager0.requestEnroll("S4", Arrays.asList("AI","VIQ"));

        
        manager = new ElectiveManager();
        manager.addCourse("VIQ", 4);
        manager.addCourse("AI", 1);
        manager.addCourse("OR", 2);
        manager.addCourse("XY", 120);
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S3", 25.0);
        manager.loadStudent("S4", 21.3);
        manager.loadStudent("S5", 28.2);
        manager.loadStudent("S6", 28.7);
        manager.loadStudent("S7", 27.0);
        manager.loadStudent("S8", 26.4);
        manager.requestEnroll("S1", Arrays.asList("VIQ","AI","OR"));
        manager.requestEnroll("S2", Arrays.asList("AI","VIQ"));
        manager.requestEnroll("S3", Arrays.asList("AI","OR"));
        manager.requestEnroll("S4", Arrays.asList("AI","OR"));
        manager.requestEnroll("S5", Arrays.asList("VIQ","AI","OR"));
        manager.requestEnroll("S6", Arrays.asList("VIQ","OR"));
        manager.requestEnroll("S7", Arrays.asList("AI","VIQ","OR"));
        manager.requestEnroll("S8", Arrays.asList("AI","VIQ","OR"));
    }

    @Test
    public void testMakeClasses() throws ElectiveException {
        long n = manager0.makeClasses();
        
        assertEquals("No student is not assigned",0,n);
    }

    @Test
    public void testNotAssigned() throws ElectiveException {
        long n = manager.makeClasses();
        
        assertEquals("One student is not assigned",1,n);
    }

    @Test
    public void testAssignments0() throws ElectiveException {
        manager0.makeClasses();
        
        Map<String,List<String>> assign = manager0.getAssignments();
        
        assertNotNull("Missing assignments",assign);
        assertEquals("Wrong enrolled students for VIQ",
                Arrays.asList("S1", "S4"),
                assign.get("VIQ"));
        assertEquals("Wrong enrolled students for AI",
                Arrays.asList("S2","S3"),
                assign.get("AI"));
    }

    @Test
    public void testAssignments() throws ElectiveException {
        manager.makeClasses();
        
        Map<String,List<String>> assign = manager.getAssignments();
        
        assertNotNull("Missing assignments",assign);
        assertEquals("Wrong enrolled students for VIQ",
                Arrays.asList("S1", "S6", "S5", "S8"),
                assign.get("VIQ"));
        assertEquals("Wrong enrolled students for AI",
                Arrays.asList("S7"),
                assign.get("AI"));
        assertEquals("Wrong enrolled students for OR",
                Arrays.asList("S3","S4"),
                assign.get("OR"));
    }

}
