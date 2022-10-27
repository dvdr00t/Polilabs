package it.polito.po.test;
import junit.framework.TestCase;

import java.util.*;

import managingProperties.*;
public class TestReq5 extends TestCase {
	PropertyManager pm = new PropertyManager();	

	protected void setUp() throws Exception {
		super.setUp();
		pm.addBuilding("b6",20); 
		pm.addBuilding("b1",90); 
		pm.addBuilding("b5",20);	
		pm.addOwner("ow4","b5:1","b1:10");
		pm.addOwner("ow5","b6:1","b5:2");
		pm.addProfessionals("electrician", "e2", "e4", "e5", "e6");
		pm.addProfessionals("plumber", "p1", "p4", "p5");
		pm.addProfessionals("mason", "m1");
		pm.addRequest("ow4","b5:1","electrician"); // -> 1
		pm.assign(1,"e2");
		pm.addRequest("ow5","b6:1","mason"); // -> 2
		pm.addRequest("ow5","b5:2","plumber"); // -> 3
		pm.assign(3,"p1");
		pm.assign(2,"m1");
		pm.charge(3, 500);
		pm.charge(2, 700);
		pm.charge(1, 400);
	}
	public void testGetCharges1() {
		Map<String, Integer> oMap = pm.getCharges(); 
		assertNotNull("Not charges returned", oMap);
		assertEquals("Expected two charged owners",2,oMap.size());
	}
	public void testGetCharges2() {
		Map<String, Integer> oMap = pm.getCharges(); 
		String s = "{ow4=400, ow5=1200}";
		assertNotNull("Not charges returned", oMap);
		assertEquals(s,oMap.toString());
	}
	
	public void testGetChargeBuild() {
		Map<String, Map<String, Integer>> bcMap = pm.getChargesOfBuildings(); 
		assertNotNull("Not charges returned", bcMap);
		assertEquals(2,bcMap.size());
	}
	public void testGetChargeBuild2() {
		Map<String, Map<String, Integer>> bcMap = pm.getChargesOfBuildings();
		
		String s = "{b5={e2=400, p1=500}, b6={m1=700}}";
		assertNotNull("Not charges returned", bcMap);
		assertEquals("Wrong buildings",
				new HashSet<>(Arrays.asList(new String[]{"b5", "b6"})),
				bcMap.keySet());
		assertEquals(s,bcMap.toString());
	}
	
}
