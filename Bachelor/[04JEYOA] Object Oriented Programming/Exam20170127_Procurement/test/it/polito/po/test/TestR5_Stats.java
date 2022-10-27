 package it.polito.po.test;

import java.util.List;
import java.util.Map;
import java.util.Set;

import junit.framework.TestCase;
import warehouse.InvalidSupplier;
import warehouse.MultipleDelivery;
import warehouse.Order;
import warehouse.Product;
import warehouse.Supplier;
import warehouse.Warehouse;

public class TestR5_Stats extends TestCase {

    private Warehouse wh;
    private Order ord1 ;
    private Order ord2 ;
    private Order ord3 ;
    private Order ord4 ;
    private Order ord5 ;

    public void setUp() throws InvalidSupplier, MultipleDelivery{
        wh = new Warehouse();
        
        Product banane = wh.newProduct("BNN","Banane");
        banane.setQuantity(33);
        Product kiwi = wh.newProduct("KWI","Kiwi");
        kiwi.setQuantity(44);
        Product mele = wh.newProduct("APL","Apples");
        mele.setQuantity(80);

        Supplier chiquita = wh.newSupplier("CQT", "Chiquita");
        Supplier delmonte = wh.newSupplier("DMT", "Del Monte");
        Supplier valVenosta = wh.newSupplier("VVN", "Val Venosta");
        Supplier melinda = wh.newSupplier("MLD", "Melinda");
        
        
        chiquita.newSupply(banane);
        delmonte.newSupply(kiwi);
        delmonte.newSupply(banane);
        valVenosta.newSupply(mele);
        melinda.newSupply(mele);
        
         ord1 = wh.issueOrder(banane,67,chiquita);
         ord2 = wh.issueOrder(banane,100,delmonte);
         ord3 = wh.issueOrder(mele,150,valVenosta);
         ord4 = wh.issueOrder(kiwi,67,delmonte);
         ord5 = wh.issueOrder(mele,50,melinda);

         ord2.setDelivered();
         ord4.setDelivered();
         ord3.setDelivered();
         ord5.setDelivered();
    }
    
    public void testOrdersByProduct(){
        Map<String,List<Order>> obp = wh.ordersByProduct();
        
        assertNotNull("Cannot get order by product map",obp);
        List<Order> bnn = obp.get("BNN");
        
        
        assertNotNull("Missing orders for product BNN",bnn);
        assertEquals("Wrong number of BNN orers",2,bnn.size());
        
        assertTrue("Missing order for BNN",bnn.contains(ord1));
        assertTrue("Missing order for BNN",bnn.contains(ord2));
        
        assertNotNull("Missing orders for product APL",obp.get("APL"));
    }
    
    public void testOrderNBySupplier (){
        Map<String,Long> nbs = wh.orderNBySupplier();
        
        assertNotNull("Cannot get number of orders by supplier",nbs);
        Set<String> suppliers = nbs.keySet();
        assertTrue("Missing supplier VVN in order count",suppliers.contains("Val Venosta"));
        assertTrue("Missing supplier DMT in order count",suppliers.contains("Del Monte"));
        
        assertEquals("Wrong number of orders for Del Monte",2,nbs.get("Del Monte").longValue());
    }
    
    public void testCountDeliveredByProduct(){
        
        List<String> dbp = wh.countDeliveredByProduct();
        
        assertNotNull("Cannot get delivered products count",dbp);
        
        System.out.println(dbp);
        
        String apples = dbp.get(0); // 2 Apples orders
        assertTrue("Wrong product in first position",apples.indexOf("APL")>=0);
        assertTrue("Wrong order count in first position",apples.indexOf("2")>=0);

        String second = dbp.get(1); // 1 Banana or 1 Kiwi
        assertTrue("Wrong product in second position",second.indexOf("BNN")>=0 || second.indexOf("KWI")>=0);

    }
}
