package diet;

import java.util.HashMap;
import java.util.LinkedHashMap;
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
		this.ingredients = new LinkedHashMap<>();
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
		
		//Useful values
		float caloriesForTotalGrams = 0;
		double totalGram = 0;
		
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet()) {
			caloriesForTotalGrams += ((float) (ingredient.getCalories() * this.ingredients.get(ingredient))) / 100;
			totalGram += this.ingredients.get(ingredient);
		}
		
		return ((caloriesForTotalGrams * 100)/totalGram);
	}

	@Override
	public double getProteins() {
		
		//Useful values
		float proteinsForTotalGrams = 0;
		double totalGram = 0;
		
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet()) {
			proteinsForTotalGrams += ((float) (ingredient.getProteins() * this.ingredients.get(ingredient))) / 100;
			totalGram += this.ingredients.get(ingredient);
		}
		
		return ((proteinsForTotalGrams * 100)/totalGram);
	}

	@Override
	public double getCarbs() {
		
		//Useful values
		float carbsForTotalGrams = 0;
		double totalGram = 0;
		
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet()) {
			carbsForTotalGrams += ((float) (ingredient.getCarbs() * this.ingredients.get(ingredient))) / 100;
			totalGram += this.ingredients.get(ingredient);
		}
		
		return ((carbsForTotalGrams * 100)/totalGram);
	}

	@Override
	public double getFat() {
		
		//Useful values
		float fatForTotalGrams = 0;
		double totalGram = 0;
		
		
		//Computing the sum of all the nutritional values for the recipe
		for (RawMaterial ingredient: this.ingredients.keySet()) {
			fatForTotalGrams += ((float) (ingredient.getFat() * this.ingredients.get(ingredient))) / 100;
			totalGram += this.ingredients.get(ingredient);
		}
		
		return ((fatForTotalGrams * 100)/totalGram);
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
