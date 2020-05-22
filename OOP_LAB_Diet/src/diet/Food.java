package diet;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;



/**
 * Facade class for the diet management.
 * It allows defining and retrieving raw materials and products.
 *
 */
public class Food {
	
	
	/*
	 * ATTRIBUTES
	 */
	protected Map<String, NutritionalElement> collection;
	
	/*
	 * CONSTRUCTOR
	 */
	public Food() {
		this.collection = new TreeMap<>();
	}
	
	/**
	 * Define a new raw material.
	 * 
	 * The nutritional values are specified for a conventional 100g amount
	 * @param name 		unique name of the raw material
	 * @param calories	calories per 100g
	 * @param proteins	proteins per 100g
	 * @param carbs		carbs per 100g
	 * @param fat 		fats per 100g
	 */
	public void defineRawMaterial(String name, double calories, double proteins, double carbs, double fat){
		
		//Adding the element to the collection of NutritionalElements
		collection.put(name, new RawMaterial(name, calories, proteins, carbs, fat));
	}
	
	/**
	 * Retrieves the collection of all defined raw materials
	 * 
	 * @return collection of raw materials though the {@link NutritionalElement} interface
	 */
	public Collection<NutritionalElement> rawMaterials(){
				
		/*
		 * Creating a copy of the collection to be returned to the user.
		 * The collection is a collection of any generic elements that extend the
		 * NutritionalElement interface. The collection is create from the map values
		 * which are already sorted.
		 */
		Collection<NutritionalElement> toBeReturned = new LinkedList<NutritionalElement>();
		
		for (NutritionalElement n: this.collection.values())
			if (n instanceof RawMaterial)
				toBeReturned.add(n);

		
		//Returning the collection of elements
		return toBeReturned;
	}
	
	/**
	 * Retrieves a specific raw material, given its name
	 * 
	 * @param name  name of the raw material
	 * 
	 * @return  a raw material though the {@link NutritionalElement} interface
	 */
	public NutritionalElement getRawMaterial(String name){
		
		//Checking if the element exist in the collection of food or not
		if (this.collection.containsKey(name)) {
			
			//Returning a copy of the element
			NutritionalElement toBeReturned = this.collection.get(name);		//CLONE() ??????????
			return toBeReturned;
			
		}
		
		return null;
	}

	/**
	 * Define a new packaged product.
	 * The nutritional values are specified for a unit of the product
	 * 
	 * @param name 		unique name of the product
	 * @param calories	calories for a product unit
	 * @param proteins	proteins for a product unit
	 * @param carbs		carbs for a product unit
	 * @param fat 		fats for a product unit
	 */
	public void defineProduct(String name, double calories, double proteins, double carbs, double fat){
		
		//Adding the element to the collection of NutritionalElements
		collection.put(name, new Product(name, calories, proteins, carbs, fat));
		
	}
	
	/**
	 * Retrieves the collection of all defined products
	 * 
	 * @return collection of products though the {@link NutritionalElement} interface
	 */
	public Collection<NutritionalElement> products(){
		/*
		 * Creating a copy of the collection to be returned to the user.
		 * The collection is a collection of any generic elements that extend the
		 * NutritionalElement interface. The collection is create from the map values
		 * which are already sorted.
		 */
		Collection<NutritionalElement> toBeReturned = new LinkedList<NutritionalElement>();
		
		for (NutritionalElement n: this.collection.values())
			if (n instanceof Product)
				toBeReturned.add(n);
		
		//Returning the collection of elements
		return toBeReturned;
	}
	
	/**
	 * Retrieves a specific product, given its name
	 * @param name  name of the product
	 * @return  a product though the {@link NutritionalElement} interface
	 */
	public NutritionalElement getProduct(String name){

		//Checking if the element exist in the collection of food or not
		if (this.collection.containsKey(name)) {
			
			//Returning a copy of the element
			NutritionalElement toBeReturned = this.collection.get(name);		//CLONE() ??????????
			return toBeReturned;
			
		}
		
		return null;
	}
	
	/**
	 * Creates a new recipe stored in this Food container.
	 *  
	 * @param name name of the recipe
	 * 
	 * @return the newly created Recipe object
	 */
	public Recipe createRecipe(String name) {
		
		//Creating the recipe
		Recipe recipe = new Recipe(name, this.collection);
		//Adding the recipe in the collection
		collection.put(name, recipe);
		
		return recipe;
	}
	
	/**
	 * Retrieves the collection of all defined recipes
	 * 
	 * @return collection of recipes though the {@link NutritionalElement} interface
	 */
	public Collection<NutritionalElement> recipes(){

		/*
		 * Creating a copy of the collection to be returned to the user.
		 * The collection is a collection of any generic elements that extend the
		 * NutritionalElement interface. The collection is create from the map values
		 * which are already sorted.
		 */
		Collection<NutritionalElement> toBeReturned = new LinkedList<NutritionalElement>();
		
		for (NutritionalElement n: this.collection.values())
			if (n instanceof Recipe)
				toBeReturned.add(n);

		//Returning the collection of elements
		return toBeReturned;
	}
	
	/**
	 * Retrieves a specific recipe, given its name
	 * 
	 * @param name  name of the recipe
	 * 
	 * @return  a recipe though the {@link NutritionalElement} interface
	 */
	public NutritionalElement getRecipe(String name){		

		//Checking if the element exist in the collection of food or not
		if (this.collection.containsKey(name)) {
			
			//Returning a copy of the element
			NutritionalElement toBeReturned = this.collection.get(name);		//CLONE() ??????????
			return toBeReturned;
		}
		
		return null;
	}
	
	/**
	 * Creates a new menu
	 * 
	 * @param name name of the menu
	 * 
	 * @return the newly created menu
	 */
	public Menu createMenu(String name) {

		//Creating the recipe
		Menu menu = new Menu(name, this.collection);
		
		//Adding the recipe in the collection
		collection.put(name, menu);
		
		return menu;
	}
	
	/**
	 * Return the menu if exists
	 * 
	 * @param name	name of the menu
	 * @return Menu the menu
	 */
	public Menu getMenu (String name) {
		
		//Checking if the element exist in the collection of food or not
		if (this.collection.containsKey(name)) {
			
			//Returning a copy of the element
			Menu toBeReturned = (Menu) this.collection.get(name);
			return toBeReturned;
		}
		
		return null;
	}

}
