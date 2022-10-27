package it.polito.po.test;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import it.polito.oop.elective.ElectiveException;
import it.polito.oop.elective.ElectiveManager;

public class TestR2_Preferences {

    private ElectiveManager manager;
    
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
    }

    @Test
    public void testEnroll() throws ElectiveException {
        int n = manager.requestEnroll("S2", Arrays.asList("VIQ"));
        
        assertEquals("Wrong number of preferences detected",1,n);
    }

    @Test
    public void testEnroll2() throws ElectiveException {
        int n = manager.requestEnroll("S2", Arrays.asList("VIQ","OR","AI"));
        
        assertEquals("Wrong number of preferences detected",3,n);
    }

    @Test
    public void testEnrollErr() {
        try {
            manager.requestEnroll("S2", Arrays.asList("VIQ","OR","AI","XY"));
            fail("Four preferences are not valid");
        } catch (ElectiveException e) {
            // OK
        }
        try {
            manager.requestEnroll("S2", Arrays.asList());
            fail("No preferences are not valid");
        } catch (ElectiveException e) {
            // OK
        }
    }

    @Test
    public void testEnrollErr2() {
        try {
            manager.requestEnroll("S2", Arrays.asList("VIQ","OR","XX"));
            fail("Course XX is not defined");
        } catch (ElectiveException e) {
            // OK
        }
    }

    @Test
    public void testEnrollErr3() {
        try {
            manager.requestEnroll("S99", Arrays.asList("VIQ","OR","XX"));
            fail("Student S99 is not defined");
        } catch (ElectiveException e) {
            // OK
        }
    }
    
    @Test
    public void testNumbers() throws ElectiveException {
        manager.requestEnroll("S1", Arrays.asList("VIQ","OR","AI"));
        manager.requestEnroll("S2", Arrays.asList("VIQ","OR","AI"));
        manager.requestEnroll("S33", Arrays.asList("AI","OR","VIQ"));
        manager.requestEnroll("S21", Arrays.asList("OR","VIQ","AI"));
        manager.requestEnroll("S5", Arrays.asList("AI"));
        
        Map<String,List<Long>> numbers = manager.numberRequests();
        
        assertNotNull("Missing requests",numbers);
        assertEquals("Missing courses",4,numbers.size());
        assertEquals("Wrong numbers for VIQ",
                        Arrays.asList(2L,1L,1L),numbers.get("VIQ"));    
        assertEquals("Wrong numbers for AI",
                Arrays.asList(2L,0L,3L),numbers.get("AI"));    
        assertEquals("Wrong numbers for OR",
                Arrays.asList(1L,3L,0L),numbers.get("OR"));    
    }


}
