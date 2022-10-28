package it.polito.po.test;

import java.util.List;

import junit.framework.TestCase;
import warehouse.MultipleDelivery;
import warehouse.Order;
import warehouse.Product;
import warehouse.Supplier;
import warehouse.Warehouse;

public class TestR4_Pending extends TestCase {

	public void testDelivery() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Supplier delmonte = m.newSupplier("DMT", "Del Monte");
		
		Product banana = m.newProduct("BNN","Banane");
		banana.setQuantity(33);
		Product kiwi = m.newProduct("KWI","Kiwi");
		kiwi.setQuantity(44);
		
		chiquita.newSupply(banana);
		chiquita.newSupply(kiwi);
		
		delmonte.newSupply(banana);
		
		Order ord1 = m.issueOrder(banana,67,chiquita);
		Order ord2 = m.issueOrder(banana,100,delmonte);
		assertFalse(ord1.delivered());
		assertFalse(ord2.delivered());
		assertEquals(33,banana.getQuantity());
		assertEquals(44,kiwi.getQuantity());
		
		ord1.setDelivered();

		assertTrue(ord1.delivered());
		assertEquals(100,banana.getQuantity());
	}

	public void testMultipleDelivery() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Product banana = m.newProduct("BNN","Banane");
		chiquita.newSupply(banana);
		Order ord1 = m.issueOrder(banana,67,chiquita);		
		ord1.setDelivered();
		
		try {
			ord1.setDelivered();
			fail("Consegna multipla non segnalata");
		}catch(MultipleDelivery cm){
			
		}
	}
	
	public void testPendingGlobal() throws Exception {
		Warehouse m = new Warehouse();
		
		Supplier chiquita = m.newSupplier("CQT", "Chiquita");
		Supplier delmonte = m.newSupplier("DMT", "Del Monte");
		
		Product banana = m.newProduct("BNN","Banane");
		banana.setQuantity(33);
		Product kiwi = m.newProduct("KWI","Kiwi");
		kiwi.setQuantity(44);
		
		chiquita.newSupply(banana);
		chiquita.newSupply(kiwi);
		
		delmonte.newSupply(banana);
		
		Order ord1 = m.issueOrder(banana,67,chiquita);
		 m.issueOrder(banana,100,delmonte);
		 m.issueOrder(kiwi,100,chiquita);
		 m.issueOrder(kiwi,50,chiquita);
		
		ord1.setDelivered();
		
		List<Order> pending = m.pendingOrders();
		assertEquals(3,pending.size());
		
		for(Order o : pending){
		    assertFalse("Order should not be delivered",o.delivered());
		}
		// OR
		assertTrue("All pending orders should be not delivered",
		            pending.stream().noneMatch(Order::delivered) );
	}

	public void testPendingGlobalSorted() throws Exception {
        Warehouse m = new Warehouse();
        
        Product banana = m.newProduct("BNN","Banana");
        banana.setQuantity(33);
        Product kiwi = m.newProduct("KWI","Kiwi");
        kiwi.setQuantity(44);

        Product apple = m.newProduct("APL","Apple");
        apple.setQuantity(0);

        
        Supplier chiquita = m.newSupplier("CQT", "Chiquita");
        Supplier delmonte = m.newSupplier("DMT", "Del Monte");
        
        
        chiquita.newSupply(banana);
        chiquita.newSupply(kiwi);
        delmonte.newSupply(apple);
        delmonte.newSupply(banana);
        
        
        Order ord1 = m.issueOrder(banana,67,chiquita);
        Order ord2 = m.issueOrder(banana,100,delmonte);
        Order ord3 = m.issueOrder(kiwi,100,chiquita);
        Order ord4 = m.issueOrder(apple,50,delmonte);
        
        ord1.setDelivered();
        
        List<Order> pending = m.pendingOrders();
        assertEquals(3,pending.size());
        
        
        assertSame("Wrong order for orders",pending.get(0),ord4);
        assertSame("Wrong order for orders",pending.get(1),ord2);
        assertSame("Wrong order for orders",pending.get(2),ord3);
        
    }

   public void testPendingPerProduct() throws Exception {
        Warehouse m = new Warehouse();
        
        Supplier chiquita = m.newSupplier("CQT", "Chiquita");
        Supplier delmonte = m.newSupplier("DMT", "Del Monte");
        
        Product banana = m.newProduct("BNN","Banane");
        banana.setQuantity(33);
        Product kiwi = m.newProduct("KWI","Kiwi");
        kiwi.setQuantity(44);
        
        chiquita.newSupply(banana);
        chiquita.newSupply(kiwi);
        
        delmonte.newSupply(banana);
        
        Order ord1 = m.issueOrder(banana,67,chiquita);
         m.issueOrder(banana,100,delmonte);
        Order ord3 = m.issueOrder(kiwi,100,chiquita);
         m.issueOrder(banana,50,chiquita);
        
        List<Order> bpending = banana.pendingOrders();
        assertEquals(3,bpending.size());
        assertTrue(kiwi.pendingOrders().contains(ord3));
        assertTrue("Pending orders should not be delivered",
                    bpending.stream().noneMatch(Order::delivered));
        
        ord1.setDelivered();
        bpending = banana.pendingOrders();

        assertEquals(2,bpending.size());
    }

   public void testPendingPerProductSorted() throws Exception {
       Warehouse m = new Warehouse();
       
       Supplier chiquita = m.newSupplier("CQT", "Chiquita");
       Supplier delmonte = m.newSupplier("DMT", "Del Monte");
       
       Product banana = m.newProduct("BNN","Banane");
       banana.setQuantity(33);
       Product kiwi = m.newProduct("KWI","Kiwi");
       kiwi.setQuantity(44);
       
       chiquita.newSupply(banana);
       chiquita.newSupply(kiwi);
       
       delmonte.newSupply(banana);
       
       Order ord1 = m.issueOrder(banana,67,chiquita);
       Order ord2 = m.issueOrder(banana,100,delmonte);
        m.issueOrder(kiwi,100,chiquita);
       Order ord4 = m.issueOrder(banana,50,chiquita);
       
       List<Order> bpending = banana.pendingOrders();
       // descending order
       assertSame("First element not right",ord2,bpending.get(0));
       assertSame("First element not right",ord1,bpending.get(1));
       assertSame("First element not right",ord4,bpending.get(2));
   }

}
