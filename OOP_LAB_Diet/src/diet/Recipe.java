package diet;

import java.util.HashMap;
import java.util.Map;

/**
 * Represents a recipe of the diet.
 * 
 * A recipe consists of a a set of ingredients that are given amounts of raw materials.
 * The overall nutritional values of a recipe can be computed
 * on the basis of the ingredients' values and are expressed per 100g
 * 
 *
 */
public class Recipe extends EdibleElement implements NutritionalElement {

	/*
	 * ATTRIBUTES
	 */
	private Map <RawMaterial, Double> ingredients;
	private Map<String, NutritionalElement> collection;
	
	/*
	 * CONSTRUCTOR
	 */
	public Recipe (String name, Map<String, NutritionalElement> collection) {
		this.name = name;
		this.ingredients = new HashMap<>();
		this.collection = collection;	//Creating a link
	}
    

	/**
	 * Adds a given quantity of an ingredient to the recipe.
	 * The ingredient is a raw material.
	 * 
	 * @param material the name of the raw material to be used as ingredient
	 * @param quantity the amount in grams of the raw material to be used
	 * @return the same Recipe object, it allows method chaining.
	 */
	public Recipe addIngredient(String material, double quantity) {
		
		RawMaterial ingredient = (RawMaterial) this.collection.get(material);
		
		//Adding the ingredient to the recipe
		ingredients.put(ingredient, quantity);
		return this;
	}

	@Override
	public String getName() {
		return this.name;
	}

	@Override
	public double getCalories() {
		float sum = 0;
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet())
			sum += ((float) (ingredient.getCalories() * this.ingredients.get(ingredient))) / 100;
		
		return sum;
	}

	@Override
	public double getProteins() {
		float sum = 0;
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet())
			sum += (float) ingredient.getProteins();
		
		return sum/100;
	}

	@Override
	public double getCarbs() {
		float sum = 0;
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet())
			sum += (float) ingredient.getCarbs();
		
		return sum/100;
	}

	@Override
	public double getFat() {
		float sum = 0;
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet())
			sum += (float) ingredient.getFat();
		
		return sum/100;
	}

	/**
	 * Indicates whether the nutritional values returned by the other methods
	 * refer to a conventional 100g quantity of nutritional element,
	 * or to a unit of element.
	 * 
	 * For the {@link Recipe} class it must always return {@code true}:
	 * a recipe expresses nutritional values per 100g
	 * 
	 * @return boolean indicator
	 */
	@Override
	public boolean per100g() {
		return true;
	}
	
	
	/**
	 * Returns the ingredients composing the recipe.
	 * 
	 * A string that contains all the ingredients, one per per line, 
	 * using the following format:
	 * {@code "Material : ###.#"} where <i>Material</i> is the name of the 
	 * raw material and <i>###.#</i> is the relative quantity. 
	 * 
	 * Lines are all terminated with character {@code '\n'} and the ingredients 
	 * must appear in the same order they have been added to the recipe.
	 */
	@Override
	public String toString() {
		
		String toBeReturned = "";
		for (RawMaterial ingredient: this.ingredients.keySet())
			toBeReturned += ingredient.getName() + " " + this.ingredients.get(ingredient) + "\n";
		
		return toBeReturned;
	}
}
