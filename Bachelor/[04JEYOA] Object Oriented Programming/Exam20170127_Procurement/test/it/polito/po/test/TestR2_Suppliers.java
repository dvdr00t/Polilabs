package it.polito.po.test;

import junit.framework.TestCase;
import warehouse.Product;
import warehouse.Supplier;
import warehouse.Warehouse;

public class TestR2_Suppliers extends TestCase {

	public void testNewSupplier() throws Exception {
		Warehouse m = new Warehouse();
		
		String code = "CQT";
		String name = "Chiquita";
		Supplier chiquita = m.newSupplier(code, name);
		
		assertNotNull("Missing supplier",chiquita);
		
		assertEquals("Wrong code",code,chiquita.getCodice());
		assertEquals("Wrong name",name,chiquita.getNome());
	}

	
	   public void testFindSupplier() throws Exception {
	        Warehouse m = new Warehouse();
	        
	        String code = "CQT";
	        String name = "Chiquita";
	        Supplier chiquita = m.newSupplier(code, name);
	        
	        Supplier c = m.findSupplier(code); 
	        
	        assertNotNull("Missing supplier",c);
	        
	        assertSame("Returned wrong supplier object",chiquita,c);
	    }

	public void testNewSupply() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		
		Product banana = m.newProduct("BNN","Banana");
		Product kiwi = m.newProduct("KWI","Kiwi");
		
		chiquita.newSupply(banana);
		chiquita.newSupply(kiwi);
		
		assertEquals(2,chiquita.supplies().size());
		assertTrue(chiquita.supplies().contains(banana));
		assertTrue(chiquita.supplies().contains(kiwi));
	}

	public void testSuppliers() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Supplier delmonte = m.newSupplier("DMT", "Del Monte");
		
		Product banana = m.newProduct("BNN","Banane");
		Product kiwi = m.newProduct("KWI","Kiwi");
		
		chiquita.newSupply(banana);
		chiquita.newSupply(kiwi);
		
		delmonte.newSupply(banana);
		
		assertEquals(2,banana.suppliers().size());
		assertEquals(1,kiwi.suppliers().size());
		
		assertTrue(banana.suppliers().contains(chiquita));
		assertTrue(banana.suppliers().contains(delmonte));
	}

	
}
