package it.polito.po.test;

import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.List;


import transactions.*;

public class TestReq1 {
    TransactionManager tm = null;

    @Before
    public void setUp() throws Exception {
        tm = new TransactionManager();
    }

    @Test
    public void testAddRegion1() throws TMException {
        List<String> list = tm.addRegion("alfa1", "beta3", "beta2", "beta4");

        String[] ref = { "beta2", "beta3", "beta4" };
        assertEquals("Wrong place list", Arrays.asList(ref), list);
    }

    @Test
    public void testAddRegion2() throws TMException {
        tm.addRegion("alfa1", "beta3", "beta2", "beta4");
        List<String> list = tm.addRegion("alfa2", "beta7", "beta6", "beta3");

        String[] ref = { "beta6", "beta7" };
        assertEquals("Wrong number of places: already defined places should be ignored", 2, list.size());
        assertEquals("Wrong place list", Arrays.asList(ref), list);
    }

    @Test
    public void testAddRegion3() throws TMException {
        tm.addRegion("alfa1", "beta3", "beta2", "beta4");
        List<String> list = tm.addRegion("alfa2", "beta7", "beta6", "beta7");

        String[] ref = { "beta6", "beta7" };
        assertEquals("Wrong number of places: duplicates should be ignored", 2, list.size());
        assertEquals("Wrong place list", Arrays.asList(ref), list);
    }

    @Test
    public void testAddCarrier1() throws TMException {
        tm.addRegion("alfa3", "beta8", "beta9");
        tm.addRegion("alfa2", "beta7", "beta6");
        tm.addRegion("alfa1", "beta3", "beta2", "beta4");
        List<String> list = tm.addCarrier("gamma1", "alfa2", "alfa1");

        String[] ref = { "alfa1", "alfa2" };
        assertEquals(Arrays.asList(ref), list);
    }

    @Test
    public void testAddCarrier2() throws TMException {
        tm.addRegion("alfa3", "beta8", "beta9");
        tm.addRegion("alfa2", "beta7", "beta6");
        tm.addRegion("alfa1", "beta3", "beta2", "beta4");
        List<String> list = tm.addCarrier("gamma2", "alfa3", "alfa1", "alfa4");

        String[] ref = { "alfa1", "alfa3" };
        assertEquals("Wrong list of carriers", Arrays.asList(ref), list);
    }

    @Test
    public void testGetCarriersForRegion() throws TMException {
        tm.addRegion("alfa3", "beta8", "beta9");
        tm.addRegion("alfa2", "beta7", "beta6");
        tm.addRegion("alfa1", "beta3", "beta2", "beta4");
        tm.addCarrier("gamma3", "alfa2", "alfa1");
        tm.addCarrier("gamma5", "alfa3", "alfa1");
        tm.addCarrier("gamma1", "alfa4", "alfa2");
        List<String> list = tm.getCarriersForRegion("alfa1");

        String[] ref = { "gamma3", "gamma5" };
        assertEquals("Wrong list of carriers for region 'alfa1'", Arrays.asList(ref), list);
    }
}