import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;

import diet.Food;
import diet.NutritionalElement;
import diet.Menu;
import diet.Recipe;


public class ExampleTest {

	@Test
    public void testAll()  {
    	
        Food food = new Food();
        food.defineRawMaterial("Sugar", 400, 0, 100, 0);
        food.defineRawMaterial("Mais", 70, 2.7, 12, 1.3);
        food.defineRawMaterial("Pasta", 350, 12, 72.2, 1.5);
        food.defineRawMaterial("Olive Oil", 900, 0, 0, 100);
        food.defineRawMaterial("Nutella", 530, 6.8, 56, 31);
        food.defineRawMaterial("Passata di Pomodoro", 36, 2, 5, 0);
        
        assertNotNull("Missing materials", food.rawMaterials());
        assertEquals("Wrong number of materials", 6, food.rawMaterials().size());
        
        NutritionalElement oil = food.getRawMaterial("Olive Oil");
        assertNotNull("Missing material oil",oil);
        assertEquals("Wrong calories", 900, oil.getCalories(), 0.1);
        assertEquals("Wrong calories", 100, oil.getFat(), 0.1);
        
        food.defineProduct("Crackers", 111, 2.6, 17.2, 3.5);
        
        NutritionalElement crackers = food.getProduct("Crackers");
        
        assertNotNull("Missing product",crackers);
        assertEquals("Wrong carbs", 17.2, crackers.getCarbs(), 0.1);
        
        Recipe r = food.createRecipe("Pasta al Pomodoro");
        
        assertNotNull("Missing recipe",r);
        r.addIngredient("Pasta", 70).
          addIngredient("Passata di Pomodoro", 30).
          addIngredient("Olive Oil", 5);
        
        Menu menu = food.createMenu("M1");
        assertNotNull("Missing recipe",menu);
        menu.addRecipe("Pasta al Pomodoro", 50).
        	 addProduct("Crackers");
        
        assertFalse(menu.per100g());
        assertEquals(254.2,menu.getCalories(),0.1);
        assertEquals(6.9,menu.getProteins(),0.1);
        assertEquals(6.4,menu.getFat(),0.1);
    }
}
