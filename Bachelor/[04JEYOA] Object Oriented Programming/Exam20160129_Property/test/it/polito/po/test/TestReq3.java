package it.polito.po.test;

import junit.framework.TestCase;

import java.util.*;

import managingProperties.*;

public class TestReq3 extends TestCase {
    PropertyManager pm = new PropertyManager();

    protected void setUp() throws Exception {
        super.setUp();
            pm.addBuilding("b6", 20);
            pm.addBuilding("b1", 90);
            pm.addBuilding("b5", 20);
            pm.addOwner("ow4", "b5:1", "b1:10");
            pm.addOwner("ow5", "b6:1", "b5:2");
            pm.addProfessionals("electrician", "e2", "e4", "e5", "e6");
            pm.addProfessionals("plumber", "p1", "p4", "p5");
            pm.addProfessionals("mason", "m1");
            pm.addRequest("ow4", "b5:1", "electrician");
            pm.assign(1, "e2");
    }

    public void testAddRequest() throws PropertyException {
        int r2 = pm.addRequest("ow5", "b6:1", "electrician");
        assertEquals(r2, 2);
    }

    public void testReqUnknownOwner() {
        try {
            pm.addRequest("ow8", "b6:10", "electrician");
            fail("Creating a request with undefined owner should not be allowed");
        } catch (PropertyException e) {
            // OK
        }
    }

    public void testReqUndefinedApartment() {
        try {
            pm.addRequest("ow4", "b3:1", "electrician");
            fail("Creating a request with undefined apartment should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testReqUndefinedProfession() {
        try {
            pm.addRequest("ow4", "b5:1", "decorator");
            fail("Creating a request with undefined profession should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testReqNoOwnership() {
        try {
            pm.addRequest("ow4", "b6:1", "mason");
            fail("Creating a request for the wrong owner should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testDupAssignment() {
        try {
            pm.assign(1, "e4");
            fail("Duplicate assignment of request should not be allowed");
        } catch (PropertyException e) { // OK
        }
    }

    public void testAssignWrongSkill() {
        try {
            pm.addRequest("ow5", "b6:1", "mason");
            pm.assign(2, "e4");
            fail("Assigning a request to a wrong worker should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testgetAssignments1() throws PropertyException {
            pm.addRequest("ow5", "b6:1", "mason");
            pm.addRequest("ow5", "b5:2", "plumber");
            pm.assign(3, "p1");
            List<Integer> arList = pm.getAssignedRequests();
            assertNotNull("No assigned requests returned",arList);
            assertEquals(2,arList.size());
    }

    public void testgetAssignments2() throws PropertyException {
            pm.addRequest("ow5", "b6:1", "mason");
            pm.addRequest("ow5", "b5:2", "plumber");
            pm.assign(3, "p1");
            List<Integer> arList = pm.getAssignedRequests();
            String s = "[1, 3]";
            assertNotNull("No assigned requests returned",arList);
            assertEquals(s,arList.toString());
    }
}
