package it.polito.po.test;

import java.util.Collection;

import junit.framework.TestCase;
import warehouse.Product;
import warehouse.Warehouse;

public class TestR1_Products extends TestCase {

	public void testNewProduct() throws Exception {
		Warehouse m = new Warehouse();
		
		String code = "PMD";
		String descr = "Pomodori/Tomato";
		Product p = m.newProduct(code, descr);
		
		assertNotNull("Missing new product",p);
		assertEquals("Wrong code",code,p.getCode());
		assertEquals("Wrong description",descr,p.getDescription());
	}

	public void testProdQuantity() throws Exception {
		Warehouse m = new Warehouse();
		
		String code = "PMD";
		String descr = "Pomodori/Tomato";
		Product p = m.newProduct(code, descr);
		
		int qty = 100;
		p.setQuantity(qty);
		
		assertEquals("Wrong quantity",qty,p.getQuantity());
		
		p.decreaseQuantity();
		assertEquals("Wrong quantity",qty-1,p.getQuantity());
		
	}

	public void testGetProd() throws Exception {
		Warehouse m = new Warehouse();
		
		String code = "PMD";
		String descr = "Pomodori/Tomato";
		Product p1 = m.newProduct(code, descr);
		 m.newProduct("MLR", "Delizia rosse/Red Delicious");

		assertSame("Wrong product returned",p1,m.findProduct(code));		
	}


    public void testGetProds() throws Exception {
        Warehouse m = new Warehouse();
        
        String code = "PMD";
        String descr = "Pomodori/Tomato";
        Product p1 = m.newProduct(code, descr);
        Product p2 = m.newProduct("MLR", "Delizia rosse/Red Delicious");

        Collection<Product> prods = m.products();
        
        assertTrue("Missing product",prods.contains(p1));
        assertTrue("Missing product",prods.contains(p2));
    }
}
