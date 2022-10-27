package it.polito.po.test;

import junit.framework.TestCase;

import java.util.*;

import managingProperties.*;

public class TestReq4 extends TestCase {
    PropertyManager pm = new PropertyManager();
    int ch1,ch2,ch3;

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
            ch1= pm.addRequest("ow4", "b5:1", "electrician");
            pm.assign(1, "e2");
            ch2= pm.addRequest("ow5", "b6:1", "mason");
            ch3= pm.addRequest("ow5", "b5:2", "plumber");
            pm.assign(3, "p1");
            pm.charge(3, 500);
    }
    
    public void testRequestNumbers(){
        assertEquals("First request should have code 1",1,ch1);
        assertEquals("Second request should have code 2",2,ch2);
        assertEquals("Third request should have code 3",3,ch3);
    }

    public void testChargeNoRequest() {
        try {
            pm.charge(4, 100);
            fail("Charging an undefined request should not be allowed");
       } catch (PropertyException e) { 
           // OK
       }
    }

    public void testChargeNotAssigned() {
        try {
            pm.charge(3, 100);
            fail("Charging an unassigned request should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testChargeOutOfRange() {
        try {
            pm.charge(1, 2000);
            fail("Charging an out of range amount should not be allowed");
      } catch (PropertyException e) { 
          // OK
      }
    }

    public void testGetComplete() throws PropertyException {
            pm.assign(2, "m1");
            pm.charge(2, 700);
            List<Integer> crList = pm.getCompletedRequests();
            assertNotNull("No completed reuqests returned",crList);
            assertEquals(2,crList.size());
    }

    public void testGetComplete2() throws PropertyException {
            pm.assign(2, "m1");
            pm.charge(2, 700);
            List<Integer> crList = pm.getCompletedRequests();
            String s = "[2, 3]";
            assertNotNull("No completed reuqests returned",crList);
            assertEquals(s,crList.toString());
    }
}
