package it.polito.po.test;

import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.SortedMap;
import transactions.*;

public class TestReq4 {
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
		tm.addRequest("x1", "beta2", "giallo");
		tm.addOffer("y1", "beta9", "giallo");
		tm.addOffer("y2", "beta9", "verde");
		tm.addOffer("y3", "beta9", "giallo");
		tm.addRequest("x3", "beta2", "giallo");
		tm.addRequest("x4", "beta9", "rosso");
		tm.addOffer("y4", "beta2", "rosso");
		
		tm.addRequest("x5", "beta8", "rosso");
		tm.addOffer("y5", "beta4", "rosso");
		tm.addTransaction("tau7", "gamma5", "x5", "y5");  //alfa3 rosso
		tm.addTransaction("tau1", "gamma5", "x1", "y1");  //alfa1 giallo
		tm.addTransaction("tau3", "gamma5", "x3", "y3");  //alfa1 giallo 
		tm.addTransaction("tau5", "gamma5", "x4", "y4");  //alfa3 rosso
		
		tm.addRequest("x7", "beta7", "rosso");
		tm.addOffer("y7", "beta6", "rosso");
		tm.addTransaction("tau8", "gamma3", "x7", "y7");  //alfa2 rosso
		
		tm.evaluateTransaction("tau1",6);
		tm.evaluateTransaction("tau3",8);
		tm.evaluateTransaction("tau5",7);
		tm.evaluateTransaction("tau8",10);
		
	}

	@Test
	public void testDeliveryRegionsPerNT1()  {
		SortedMap<Long, List<String>> m1 = tm.deliveryRegionsPerNT();

		assertEquals("Expected two distinct numbers of transactions",2, m1.size());
		assertTrue("Missing number 1",m1.containsKey(1L));
        assertTrue("Missing number 1",m1.containsKey(2L));
    }
	
    @Test
	public void testDeliveryRegionsPerNT2()  {
		SortedMap<Long, List<String>> m1 = tm.deliveryRegionsPerNT();

		assertEquals("Wrong list of regions with two deliveries",
		        Arrays.asList(new String[] {"alfa1","alfa3"}),
		                m1.get(2L));
        assertEquals("Wrong list of regions with one delivery",
                Arrays.asList(new String[] {"alfa2"}),
                        m1.get(1L));
	}
	
    @Test
	public void testScorePerCarrier1()  { //score in 1..10
		SortedMap<String, Integer> m2 = tm.scorePerCarrier(0);

		Set<String> s = m2.keySet();
		assertTrue("Missing carrier 'gamma3'",s.contains("gamma3"));
		assertTrue("Missing carrier 'gamma5'",s.contains("gamma5"));
	}


    @Test
	public void testScorePerCarrier2()  { //score in 1..10
	        SortedMap<String, Integer> m2 = tm.scorePerCarrier(0);

	        assertEquals("Wrong score for carrier 'gamma3'",10,m2.get("gamma3").intValue());
	        assertEquals("Wrong score for carrier 'gamma5'",21,m2.get("gamma5").intValue());
	    }

	    public void testScorePerCarrier3()  { //score in 1..10
	        SortedMap<String, Integer> m2 = tm.scorePerCarrier(7);

	        assertEquals("Wrong score for carrier 'gamma3'",10,m2.get("gamma3").intValue());
	        assertEquals("Wrong score for carrier 'gamma5'",15,m2.get("gamma5").intValue());
	    }

	    public void testNTPerProduct()  {
		SortedMap<String, Long> m3 = tm.nTPerProduct();

        assertEquals("Wrong score for product 'giallo'",2,m3.get("giallo").intValue());
        assertEquals("Wrong score for product 'rosso'",3,m3.get("rosso").intValue());
	}	
}