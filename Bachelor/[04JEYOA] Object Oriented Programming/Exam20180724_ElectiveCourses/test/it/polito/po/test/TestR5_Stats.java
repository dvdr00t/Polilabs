package it.polito.po.test;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import it.polito.oop.elective.ElectiveException;
import it.polito.oop.elective.ElectiveManager;

public class TestR5_Stats {


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
    public void testSuccRate0() throws ElectiveException {
        
        manager0.makeClasses();
        
        double sr1 = manager0.successRate(1);
        double sr2 = manager0.successRate(2);
        
        assertEquals("Wrong success rate",.75,sr1,0.01);
        assertEquals("Wrong success rate",.25,sr2,0.01);
        
    }

    @Test
    public void testSuccRate() throws ElectiveException {
        manager.makeClasses();
        
        double sr1 = manager.successRate(1);
        double sr2 = manager.successRate(2);
        double sr3 = manager.successRate(3);
        
        assertEquals("Wrong success rate for 1st choice ",4.0/8.0,sr1,0.01);
        assertEquals("Wrong success rate for 2nd choice ",3.0/8.0,sr2,0.01);
        assertEquals("Wrong success rate for 3rd choice ",0.0,sr3,0.01);

    }
    
    @Test
    public void testSuccRate2() throws ElectiveException {
        manager.loadStudent("S9", 26.3);
        manager.requestEnroll("S9", Arrays.asList("VIQ","AI","XY"));

        manager.makeClasses();
        
        double sr1 = manager.successRate(1);
        double sr2 = manager.successRate(2);
        double sr3 = manager.successRate(3);
        
        assertEquals("Wrong success rate for 1st choice ",4.0/9.0,sr1,0.01);
        assertEquals("Wrong success rate for 2nd choice ",3.0/9.0,sr2,0.01);
        assertEquals("Wrong success rate for 3rd choice ",1.0/9.0,sr3,0.01);
    }
    
    @Test
    public void testNotAssigned() {
        manager.makeClasses();
        List<String> unassigned = manager.getNotAssigned();
        
        assertNotNull("Missing not assigned list",unassigned);
        assertEquals("There should be one unassigned student",1,unassigned.size());
        assertEquals("Student S2 should be not assigned","S2",unassigned.get(0));
    }

}
