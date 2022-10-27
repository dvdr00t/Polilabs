package it.polito.po.test;

import junit.framework.TestCase;
import warehouse.InvalidSupplier;
import warehouse.Order;
import warehouse.Product;
import warehouse.Supplier;
import warehouse.Warehouse;

public class TestR3_Orders extends TestCase {

	public void testSuppliers() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Supplier delmonte = m.newSupplier("DMT", "Del Monte");
		
		Product banane = m.newProduct("BNN","Banane");
		Product kiwi = m.newProduct("KWI","Kiwi");
		
		chiquita.newSupply(banane);
		chiquita.newSupply(kiwi);
		
		delmonte.newSupply(banane);
		
		Order ord1 = m.issueOrder(banane,100,chiquita);
		Order ord2 = m.issueOrder(banane,100,delmonte);
		
		assertTrue(ord1.getCode().toUpperCase().startsWith("ORD"));
        assertTrue(ord1.getCode().toUpperCase().startsWith("ORD"));
		assertFalse(ord1.getCode().equals(ord2.getCode()));
	}

	
	   public void testFindOrder() throws Exception {
	        Warehouse m = new Warehouse();
	        
	        Supplier chiquita = m.newSupplier("CQT", "Chiquita");
	        Supplier delmonte = m.newSupplier("DMT", "Del Monte");
	        
	        Product banane = m.newProduct("BNN","Banane");
	        Product kiwi = m.newProduct("KWI","Kiwi");
	        
	        chiquita.newSupply(banane);
	        chiquita.newSupply(kiwi);
	        
	        delmonte.newSupply(banane);
	        
	        Order ord1 = m.issueOrder(banane,100,chiquita);
	        
	        Order o = m.findOrder(ord1.getCode());
	        
	        assertSame(ord1,o);
	    }

	public void testInvalidSupplier() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Supplier delmonte = m.newSupplier("DMT", "Del Monte");
		
		Product banane = m.newProduct("BNN","Banane");
		Product kiwi = m.newProduct("KWI","Kiwi");
		
		chiquita.newSupply(banane);
		chiquita.newSupply(kiwi);
		
		delmonte.newSupply(banane);
		
		try{
		 m.issueOrder(kiwi,100,delmonte);
		 fail("Il fornitore non e' valido: manca l'eccezione");
		}catch(InvalidSupplier fnv){
			
		}
	}

	public void testOrdineString() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		/*Supplier delmonte =*/ m.newSupplier("DMT", "Del Monte");
		
		Product banane = m.newProduct("BNN","Banane");
		/*Product kiwi =*/ m.newProduct("KWI","Kiwi");
		
		chiquita.newSupply(banane);
		
		Order ord1 = m.issueOrder(banane,100,chiquita);
		
		String str = ord1.toString();
		
		int indexCode = str.indexOf(ord1.getCode());
		int indexQty = str.indexOf("100");
		int indexPCode = str.indexOf(banane.getCode());
		int indexPDescr = str.indexOf(banane.getDescription());
		int indexFName = str.indexOf(chiquita.getNome());
		
		assertTrue("Manca il codice dell'ordine",indexCode>0);
		assertTrue("Manca la quantita ordinata",indexQty>0);
		assertTrue("Manca il codice del prodotto",indexPCode>0);
		assertTrue("Manca la descrizione del prodotto",indexPDescr>0);
		assertTrue("Manca il nome del fornitore",indexFName>0);
	}

}
