import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Collection;

import org.junit.Test;

import diet.Food;
import diet.Menu;
import diet.NutritionalElement;
import diet.Order;
import diet.Recipe;
import diet.Restaurant;
import diet.Takeaway;
import diet.User;
import diet.Order.OrderStatus;
import diet.Order.PaymentMethod;

public class ExampleTest_Ext {
	
	@Test
    public void testAll()  {
    	
		Food foods = new Food();
		
        Takeaway takeaway = new Takeaway();
        
        //Registering new users
        User u1 = takeaway.registerUser("Ralph", "Fiennes", "r.fiennes@gmail.com", "333413493");
        User u2 = takeaway.registerUser("Ian", "McKellen", "i.mckellen@gmail.com", "124882578");
        User u3 = takeaway.registerUser("Maggie", "Smith", "m.smith@gmail.com", "3647851225");
        User u4 = takeaway.registerUser("Judi", "Dench", "j.dench@gmail.com", "9885422544");
        takeaway.registerUser("Adam", "McKellen", "a.mckellen@gmail.com", "124882578");
        
        takeaway.users();
        
        //Defining restaurants and their working hours
        Restaurant r1 = new Restaurant("Napoli",foods);
        r1.setHours("08:00", "14:00", "19:00", "23:59");
        takeaway.addRestaurant(r1);
        Restaurant r2 = new Restaurant("Roma",foods);
        r2.setHours("08:45", "13:30", "18:20", "23:00");
        takeaway.addRestaurant(r2);
        Restaurant r3 = new Restaurant("Venezia",foods);
        r3.setHours("12:30", "16:00", "07:45", "11:00");
        takeaway.addRestaurant(r3);
        Restaurant r4 = new Restaurant("Milano",foods);
        r4.setHours("08:45", "23:30");
        takeaway.addRestaurant(r4);
        
        Collection<String> restaurants = takeaway.restaurants();
        assertEquals("Wrong number of restaurants",4,restaurants.size());
       
        //Defining materials used by restaurant r1 - Napoli
        foods.defineRawMaterial("Pasta", 350, 12, 72.2, 1.5);
        foods.defineRawMaterial("Oil", 900, 0, 0, 100);
        foods.defineRawMaterial("Nutella", 480, 6.8, 56, 31);
        foods.defineRawMaterial("Eggs", 120, 6.8, 56, 31);
        foods.defineRawMaterial("Flour", 100, 6.8, 56, 31);
        foods.defineRawMaterial("Sausage", 500, 15.8, 20, 4.1);
        foods.defineRawMaterial("Tomato sauce", 120, 6.8, 56, 31);
        foods.defineRawMaterial("Minced meat", 450, 6.8, 56, 31);
        foods.defineRawMaterial("Mozzarella", 250, 6.8, 56, 31);
        foods.defineRawMaterial("Olives", 60, 6.8, 56, 31);
        foods.defineRawMaterial("Pesto", 120, 6.8, 56, 20);
        foods.defineRawMaterial("Onion", 40, 3, 20.3, 1);
        foods.defineProduct("Crackers", 111, 2.6, 17.2, 3.5);
        foods.defineProduct("Beer 0.5l", 40, 0.5, 0.2, 0.05);
        foods.defineProduct("Grissini", 20, 0.5, 0.2, 0.05);
        
  
       //Defining materials used by restaurant r2 - Roma
        foods.defineRawMaterial("Prosciutto Crudo", 420, 6.8, 56, 31);
        foods.defineRawMaterial("Carrots", 60, 6.8, 56, 31);
        foods.defineRawMaterial("Red wine", 60, 6.8, 56, 31);
        foods.defineRawMaterial("Bacon", 530, 6.8, 56, 31);
        foods.defineRawMaterial("Garlic", 34, 4.1, 21.2, 2);
        foods.defineProduct("Biscuits", 150, 2.0, 10.2, 1.4);
        foods.defineProduct("Amaro", 10, 0.6, 0.25, 0.1);
        foods.defineProduct("Wine 0.5l",  40, 0.5, 1.2, 0.05);
        foods.defineProduct("Water bottle 0.33l", 5, 0.1, 0.2, 0.05);
             
        
        //Defining materials used by restaurant r3 - Venezia
        foods.defineRawMaterial("Cream", 530, 6.8, 56, 31);
        foods.defineRawMaterial("Gorgonzola", 300, 6.8, 56, 31);
        foods.defineRawMaterial("Walnuts", 220, 6.8, 56, 31);
        foods.defineProduct("Orange Juice 0.4l", 80, 0.5, 2.2, 0.05);
        
        //Defining materials used by restaurant r4 - Milano
        foods.defineRawMaterial("Prosciutto Cotto", 400, 6.8, 56, 31);
        foods.defineRawMaterial("Mortadella", 420, 6.8, 56, 31);
   
        
        assertNotNull("Missing materials", foods.rawMaterials());
        
        assertEquals("Wrong numnber of materials", 22, foods.rawMaterials().size());
        
        
        NutritionalElement oil = foods.getRawMaterial("Oil");
        assertNotNull("Missing material oil",oil);
        assertEquals("Wrong calories", 900, oil.getCalories(), 0.1);
        assertEquals("Wrong calories", 100, oil.getFat(), 0.1);
        
        NutritionalElement crackers = foods.getProduct("Crackers");
        assertNotNull("Missing product",crackers);
        assertEquals("Wrong carbs", 17.2, crackers.getCarbs(), 0.1);
        
        Recipe recipe1 = foods.createRecipe("Pasta and Nutella");
        
        assertNotNull("Missing recipe",recipe1);
        recipe1.addIngredient("Pasta", 70).
          addIngredient("Nutella", 30);
                
        assertEquals(389.0 , recipe1.getCalories(), 0.1);
        
        //Defining recipes        
        foods.createRecipe("Pasta al Ragu").addIngredient("Pasta", 350)
															.addIngredient("Onion", 100)
															.addIngredient("Garlic", 40)
															.addIngredient("Tomato sauce", 250)
															.addIngredient("Red wine", 50)
															.addIngredient("Carrots", 150)
															.addIngredient("Bacon", 200)
															.addIngredient("Minced meat", 400);
        
        foods.createRecipe("Pizza 1").addIngredient("Flour", 150)
													 .addIngredient("Oil", 20)
													 .addIngredient("Sausage", 100)
        											 .addIngredient("Tomato sauce", 75)
        											 .addIngredient("Pesto", 40)
        											 .addIngredient("Mozzarella", 50);
        
                
        foods.createRecipe("Pizza 2").addIngredient("Flour", 150)
        											    .addIngredient("Oil", 20)
				                                        .addIngredient("Cream", 75)
				                                        .addIngredient("Gorgonzola", 175)
				                                        .addIngredient("Walnuts", 50);

        foods.createRecipe("Pizza 3").addIngredient("Flour", 160)
        											 .addIngredient("Oil", 30)
        											 .addIngredient("Mozzarella", 200)
        											 .addIngredient("Prosciutto Crudo", 140)
        											 .addIngredient("Olives", 65);
       
        foods.createRecipe("Meatballs").addIngredient("Tomato sauce", 400)
				 									   .addIngredient("Eggs", 50)
				 									   .addIngredient("Onion", 175)
				 									   .addIngredient("Minced meat", 500);
        
        //Defining menus offered by all the restaurants
        Menu r1_menu1 = r1.createMenu("M1");
        assertNotNull("Missing recipe",r1_menu1);
        r1_menu1.addRecipe("Pasta and Nutella", 50).
        	 addProduct("Crackers");
        
        assertFalse(r1_menu1.per100g());
        assertEquals(305.5,r1_menu1.getCalories(),0.1);
        assertEquals(7.8,r1_menu1.getProteins(),0.1);
        assertEquals(8.6,r1_menu1.getFat(),0.1);
       
        Menu r1_menu2 = r1.createMenu("M2");
        assertNotNull("Missing recipe",r1_menu1);
        r1_menu2.addRecipe("Pizza 1", 350).addProduct("Beer 0.5l");
        
        Menu r1_menu6 = r1.createMenu("M6");
        assertNotNull("Missing recipe",r1_menu6);
        r1_menu6.addRecipe("Meatballs", 500).addProduct("Grissini");
     
        assertEquals(919.4, r1_menu2.getCalories(), 0.1);
        
        
        Menu r2_menu3 = r2.createMenu("M3");
        assertNotNull("Missing recipe",r2_menu3);
        r2_menu3.addRecipe("Pasta al Ragu", 320).
   	    addProduct("Wine 0.5l").addProduct("Biscuits").addProduct("Amaro");
        
        Menu r2_menu5 = r2.createMenu("M5");
        assertNotNull("Missing recipe",r2_menu5);
        r2_menu5.addRecipe("Pizza 3", 380).addProduct("Water bottle 0.33l");
        
        assertEquals(999.4, r2_menu5.getCalories(), 0.1);
    
        
        Menu r3_menu4 = r3.createMenu("M4");
        assertNotNull("Missing recipe",r3_menu4);
        r3_menu4.addRecipe("Pizza 2", 350).addProduct("Orange Juice 0.4l");
        
        Menu r3_menu1 = r3.createMenu("M1");
        assertNotNull("Missing recipe",r3_menu1);
        r3_menu1.addRecipe("Pasta and Nutella", 300).addProduct("Crackers");
        
        assertEquals(1278, r3_menu1.getCalories(), 0.1);
        
        Menu r4_menu2 = r4.createMenu("M2");
        assertNotNull("Missing recipe",r4_menu2);
        r4_menu2.addRecipe("Pizza 1", 350).addProduct("Beer 0.5l");
        
        Menu r4_menu3 = r4.createMenu("M3");
        assertNotNull("Missing recipe",r4_menu3);
        r4_menu3.addRecipe("Pasta al Ragu", 320).addProduct("Wine 0.5l").addProduct("Biscuits").addProduct("Amaro");
        
        Menu r4_menu4 = r4.createMenu("M4");
        assertNotNull("Missing recipe",r4_menu4);
        r4_menu4.addRecipe("Pizza 2", 350).addProduct("Orange Juice 0.4l");
        
        Menu r4_menu5 = r4.createMenu("M5");
        assertNotNull("Missing recipe",r4_menu5);
        r4_menu5.addRecipe("Pizza 3", 400).addProduct("Water bottle 0.33l");
        
        assertEquals(919.4, r4_menu2.getCalories(), 0.1);
        
        
        //Making new orders
        Order o1 = takeaway.createOrder(u1, "Napoli", 17, 47);//r1
        o1.addMenus("M6", 1).addMenus("M1", 2);
        assertEquals(o1.toString().trim(),"Napoli, Ralph Fiennes : (08:00):\n\tM1->2\n\tM6->1");
        o1.addMenus("M1", 3);
        assertEquals(o1.toString().trim(),"Napoli, Ralph Fiennes : (08:00):\n\tM1->3\n\tM6->1");
        o1.addMenus("M1", 2);
        assertEquals(o1.toString().trim(),"Napoli, Ralph Fiennes : (08:00):\n\tM1->2\n\tM6->1");
        
        
       
        Order o2 = takeaway.createOrder(u3, "Napoli", 17, 55);//r1
        o2.addMenus("M6", 2).addMenus("M2", 2);
        
        Order o3 = takeaway.createOrder(u2, "Milano",  20, 30);//r4
        o3.addMenus("M2", 1).addMenus("M4", 2).addMenus("M5", 1);
        assertTrue(o3.getPaymentMethod() == PaymentMethod.CASH);
        
        Order o4 = takeaway.createOrder(u3, "Roma",  2, 15);//r2
        o4.addMenus("M3", 4).addMenus("M5", 3);
        o4.setPaymentMethod(PaymentMethod.CARD);
        assertTrue(o4.getPaymentMethod() == PaymentMethod.CARD);
        
        Order o5 = takeaway.createOrder(u4, "Napoli",  17, 18);//r1
        o5.addMenus("M6", 1);
        
        Order o6 = takeaway.createOrder(u3, "Venezia", 11, 15);//r3
        o6.addMenus("M4", 2).addMenus("M1", 1);
        
        Order o7 = takeaway.createOrder(u1, "Napoli",  11, 47);//r1
        o7.addMenus("M6", 2);
        assertTrue(o7.getPaymentMethod() == PaymentMethod.CASH);
        
        Order o8 = takeaway.createOrder(u1, "Napoli", 9, 15);//r1
        o8.addMenus("M1", 1);
        assertTrue(o8.getPaymentMethod() == PaymentMethod.CASH);
        
       
        assertTrue(o1.getStatus()==OrderStatus.ORDERED);
        o1.setStatus(OrderStatus.DELIVERED);
        assertTrue(o1.getStatus()==OrderStatus.DELIVERED);
        o2.setStatus(OrderStatus.READY);
        o4.setStatus(OrderStatus.READY);
        o5.setStatus(OrderStatus.DELIVERED);
        
        
        assertEquals(r1.ordersWithStatus(OrderStatus.DELIVERED).trim(),"Napoli, Judi Dench : (19:00):\n\tM6->1\nNapoli, Ralph Fiennes : (19:00):\n\tM1->2\n\tM6->1");    
        assertEquals(r2.ordersWithStatus(OrderStatus.READY).trim(),"Roma, Maggie Smith : (08:45):\n\tM3->4\n\tM5->3");       
        assertEquals(r3.ordersWithStatus(OrderStatus.ORDERED).trim(),"Venezia, Maggie Smith : (12:30):\n\tM1->1\n\tM4->2");
        assertEquals(r4.ordersWithStatus(OrderStatus.ORDERED).trim(),"Milano, Ian McKellen : (20:30):\n\tM2->1\n\tM4->2\n\tM5->1");
        
        
        //Checking openedRestaurants()
        StringBuffer user_sort = new StringBuffer();
        for (User u : takeaway.users()) {
        	user_sort.append(u).append("\n");
        }
        assertTrue(user_sort.toString().equals("Judi Dench\nRalph Fiennes\nAdam McKellen\nIan McKellen\nMaggie Smith\n"));
        
        
        StringBuffer openR1 = new StringBuffer();
        for (Restaurant r: takeaway.openedRestaurants("05:50")) {
        	openR1.append(r.getName()).append("\n");
        }
        assertTrue(openR1.toString().equals(""));
        
        
        StringBuffer openR2 = new StringBuffer();
        for (Restaurant r: takeaway.openedRestaurants("11:31")) {
        	openR2.append(r.getName()).append("\n");
        }
        assertEquals("Milano\nNapoli\nRoma\n",openR2.toString());
                
        assertEquals(0,takeaway.openedRestaurants("02:15").size());
               
    }
}
