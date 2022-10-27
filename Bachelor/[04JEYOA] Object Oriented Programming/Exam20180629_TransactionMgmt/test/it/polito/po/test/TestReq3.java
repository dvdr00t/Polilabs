package it.polito.po.test;

import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import transactions.*;

public class TestReq3 {
	TransactionManager tm = null;

	@Before
	public void setUp() throws TMException, Exception {
		tm = new TransactionManager();
		tm.addRegion("alfa3", "beta8", "beta9");
		tm.addRegion("alfa2", "beta7", "beta6");
		tm.addRegion("alfa1", "beta3", "beta2", "beta4");
		tm.addCarrier("gamma3", "alfa2", "alfa1");
		tm.addCarrier("gamma5", "alfa3", "alfa1"); 
		tm.addCarrier("gamma1", "alfa4", "alfa1");
		tm.addRequest("x1", "beta2", "giallo"); //alfa1
		tm.addOffer("y1", "beta9", "giallo"); //alfa3
		tm.addOffer("y2", "beta9", "verde");  //alfa3
		tm.addOffer("y3", "beta9", "giallo"); //alfa3
		tm.addRequest("x3", "beta2", "giallo"); //alfa1
		tm.addRequest("x4", "beta9", "rosso");
		tm.addOffer("y4", "beta2", "rosso");
	}
	
	@Test
	public void testAddTransaction2() {
		try {
			tm.addTransaction("tau1", "gamma3", "x1", "y1"); 
			fail("carrier doesn't handle both regions!");
		} catch (TMException ex) {
			//ok
		}
	}

	@Test
	public void testAddTransaction3() {
		try {
			tm.addTransaction("tau1", "gamma5", "x1", "y2"); 
			fail("mismatch in productIds!");
		} catch (TMException ex) {
		}
	}	

	@Test
	public void testAddTransaction4() throws TMException {
		tm.addTransaction("tau1", "gamma5", "x1", "y1");
		try {
			tm.addTransaction("tau2", "gamma5", "x1", "y3"); 
			fail("request already served!");
		} catch (TMException ex) {
			//ok
		}
	}

	@Test
	public void testAddTransaction5() throws TMException {
		tm.addTransaction("tau1", "gamma5", "x1", "y1");
		try {
			tm.addTransaction("tau3", "gamma5", "x3", "y1"); 
			fail("offer already served!");
		} catch (TMException ex) {
			//ok
		}
	}
	
    @Test
	public void testEvaluateTransaction1() throws TMException {
		tm.addTransaction("tau1", "gamma5", "x1", "y1");
		boolean b = tm.evaluateTransaction("tau1",6);
		assertTrue("Score should be valid", b);
	}
	
    @Test
	public void testEvaluateTransaction2() throws TMException {
		tm.addTransaction("tau1", "gamma5", "x1", "y1");
		boolean b = tm.evaluateTransaction("tau1",12);
		assertFalse("Score should not be valid", b);
        b = tm.evaluateTransaction("tau1",6);
        assertTrue("Score should be valid", b);
	}
	
    @Test
	public void testEvaluateTransaction3() throws TMException {
		tm.addTransaction("tau1", "gamma5", "x1", "y1");
		boolean b = tm.evaluateTransaction("tau1",0);
        assertFalse("Score should not be valid", b);
        b = tm.evaluateTransaction("tau1",6);
        assertTrue("Score should be valid", b);
	}
}