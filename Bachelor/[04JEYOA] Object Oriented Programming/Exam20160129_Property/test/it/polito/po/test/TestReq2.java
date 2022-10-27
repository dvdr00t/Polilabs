package it.polito.po.test;

import junit.framework.TestCase;

import java.util.*;

import managingProperties.*;

public class TestReq2 extends TestCase {
    PropertyManager pm = new PropertyManager();

    protected void setUp() throws Exception {
        super.setUp();
            pm.addProfessionals("electrician", "e2", "e4", "e5", "e6");
    }

    public void testDupProf() {
        try {
            pm.addProfessionals("electrician", "e9");
            fail("Duplicate profession should not be allowed");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testDupWorker() {
        try {
            pm.addProfessionals("plumber", "p1", "p4", "p5", "p4");
            fail("Duplicate professional workers should not be allowed in the same list");
        } catch (PropertyException e) { 
            // OK
        }
    }

    public void testDoubleProfession() {
        try {
            pm.addProfessionals("plumber", "p1", "e2", "p5", "p4");
            fail("Professional workers cannot exercise more than one profession");
        } catch (PropertyException e) {
            // OK
        }
    }

    public void testProfessionals1() throws PropertyException {
            pm.addProfessionals("plumber", "p1", "p5", "p4");
            pm.addProfessionals("mason", "m1");
            Map<String, Integer> pMap = pm.getProfessions();

            assertNotNull("No professions returned",pMap);
            assertEquals("Three professions were expected",3,pMap.size());
    }

    public void testProfessionals2() throws PropertyException {
            pm.addProfessionals("plumber", "p1", "p5", "p4");
            pm.addProfessionals("mason", "m1");
            Map<String, Integer> pMap = pm.getProfessions();

            assertNotNull("No professions returned",pMap);
            
            Set<String> p = new HashSet<>(Arrays.asList(
            		new String[]{"electrician", "plumber", "mason"}));
            
            assertEquals("Missing some professional",p,pMap.keySet());
            		
            String s = "{electrician=4, mason=1, plumber=3}";
            
            assertEquals("Wrong professional workers",s,pMap.toString());
    }

}
