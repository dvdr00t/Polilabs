package it.polito.po.test;

import junit.framework.TestCase;

import java.util.*;

import managingProperties.*;

public class TestReq1 extends TestCase {
    PropertyManager pm = new PropertyManager();

    protected void setUp() throws Exception {
        super.setUp();
            pm.addBuilding("b6", 20);
            pm.addBuilding("b1", 90);
            pm.addBuilding("b5", 20);
    }

    public void testBuildingDuplicated() {
        try {
            pm.addBuilding("b5", 30);
            fail("Duplicated buildings should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testApartmentsN() {
        try {
            pm.addBuilding("b7", 120);
            fail("wrong ApartmentsN");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testGetBuildings1() {
        Map<Integer, List<String>> bMap = pm.getBuildings();
        assertNotNull("No buildings were returned",bMap);
        assertEquals("Buildings map should have two entries",2,bMap.size());
    }

    public void testGetBuildings2() {
        String s = "{20=[b5, b6], 90=[b1]}";
        Map<Integer, List<String>> bMap = pm.getBuildings();
        assertNotNull("No buldings list returned", bMap);
        assertEquals(s,bMap.toString());
    }


    public void testOwner1() {
        try {
            pm.addOwner("ow1", "b5:1", "b5:2");
            pm.addOwner("ow1", "b5:3");
            fail("Duplicate owner should not be allowed.");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testOwner2() {
        try {
            pm.addOwner("ow2", "b6:1", "b8:2");
            fail("Undefined buildings should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testOwner3() {
        try {
            pm.addOwner("ow2", "b6:1", "b5:21");
            fail("Undefined apartments should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testOwner4() {
        try {
            pm.addOwner("ow2", "b6:1", "b5:1");
            pm.addOwner("ow1", "b6:2", "b5:1");
            fail("Should not be possible to assign two owner to the same apartment");
        } catch (PropertyException e) { 
            // OK
        }
    }
}
