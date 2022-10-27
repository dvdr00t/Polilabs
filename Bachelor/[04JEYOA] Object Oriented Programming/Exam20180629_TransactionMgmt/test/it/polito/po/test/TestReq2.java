package it.polito.po.test;

import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import transactions.*;

public class TestReq2 {
	TransactionManager tm = null;

	@Before
	public void setUp() throws Exception {
		tm = new TransactionManager();
	}
	
	@Test
	public void testAddRequest1() throws TMException {
		tm.addRegion("alfa1", "beta3", "beta2", "beta4");
		tm.addRequest("x1", "beta2", "giallo");
		try {
			tm.addRequest("x1", "beta3", "red");
			fail("duplicated requestId not detected!");
		} catch(TMException ex) {
		}
	}
	
    @Test
	public void testAddRequest2() throws TMException {
		tm.addRegion("alfa1", "beta3", "beta2", "beta4");
		try {
			tm.addRequest("x1", "beta5", "giallo");
			fail("undefined place not detected!");
		} catch(TMException ex) {
			//ok
		}
	}
	
    @Test
	public void testAddOffer1() throws TMException {
		tm.addRegion("alfa1", "beta3", "beta2", "beta4");
		tm.addOffer("y1", "beta2", "giallo");
		try {
			tm.addOffer("y1", "beta4", "green");
			fail("duplicated offerId not detected!");
		} catch(TMException ex) {
		    // ok
		}
	}
	
    @Test
	public void testAddOffer2() throws TMException {
		tm.addRegion("alfa1", "beta3", "beta2", "beta4");
		try {
			tm.addOffer("y1", "beta5", "giallo");
			fail("undefined place not detected!");
		} catch(TMException ex) {
			//ok
		}
	}
}
