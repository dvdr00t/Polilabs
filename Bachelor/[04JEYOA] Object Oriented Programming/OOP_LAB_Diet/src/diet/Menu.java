package diet;

import java.util.HashMap;
import java.util.Map;

/**
 * Represents a complete menu.
 * 
 * It can be made up of both packaged products and servings of given recipes.
 *
 */
public class Menu extends EdibleElement implements NutritionalElement, Comparable<Menu> {
	
	/*
	 * ATTRIBUTES
	 */
	private Map<NutritionalElement, Double> composition;
	private Map<String, NutritionalElement> collection;
	
	/*
	 * CONSTRUCTORS
	 */
	public Menu (String name, Map<String, NutritionalElement> collection) {
		this.name = name;
		this.composition = new HashMap<>();
		this.collection = collection;
		
	}
	
	/**
	 * Adds a given serving size of a recipe.
	 * 
	 * The recipe is a name of a recipe defined in the
	 * {@Link Food} in which this menu has been defined.
	 * 
	 * @param recipe the name of the recipe to be used as ingredient
	 * @param quantity the amount in grams of the recipe to be used
	 * @return the same Menu to allow method chaining
	 */
	public Menu addRecipe(String recipe, double quantity) {
		
		//Retrieving the recipe from the collection
		Recipe menuRecipe = (Recipe) this.collection.get(recipe);
		
		//Adding the recipe to the menu
		this.composition.put(menuRecipe, quantity);
		return this;
		
	}

	/**
	 * Adds a unit of a packaged product.
	 * The product is a name of a product defined in the
	 * {@Link Food} in which this menu has been defined.
	 * 
	 * @param product the name of the product to be used as ingredient
	 * @return the same Menu to allow method chaining
	 */
	public Menu addProduct(String product) {
		
		//Retrieving the product from the collection
		Product menuProduct = (Product) this.collection.get(product);
		
		//Adding the product to the menu
		this.composition.put(menuProduct, 1.0);
		return this;
	}

	/**
	 * Name of the menu
	 */
	@Override
	public String getName() {
		return this.name;
	}

	/**
	 * Total KCal in the menu
	 */
	@Override
	public double getCalories() {
		float sum = 0;
		
		//Computing the total Kcal in the menu
		for (NutritionalElement n: this.composition.keySet()) {
			if (n.per100g())
				sum += ((float) (n.getCalories() * this.composition.get(n))) / 100;
			else
				sum += n.getCalories();
		}
		
		return sum;
	}

	/**
	 * Total proteins in the menu
	 */
	@Override
	public double getProteins() {
		float sum = 0;
		
		//Computing the total Kcal in the menu
		for (NutritionalElement n: this.composition.keySet()) {
			if (n.per100g())
				sum += ((float) (n.getProteins() * this.composition.get(n))) / 100;
			else 
				sum += n.getProteins();
		}
		
		return sum;
	}

	/**
	 * Total carbs in the menu
	 */
	@Override
	public double getCarbs() {
		float sum = 0;
		
		//Computing the total Kcal in the menu
		for (NutritionalElement n: this.composition.keySet()) {
			if (n.per100g())
				sum += ((float) (n.getCarbs() * this.composition.get(n))) / 100;
			else 
				sum += n.getCarbs();
		}
		
		return sum;
	}

	/**
	 * Total fats in the menu
	 */
	@Override
	public double getFat() {
		float sum = 0;
		
		//Computing the total Kcal in the menu
		for (NutritionalElement n: this.composition.keySet()) {
			if (n.per100g())
				sum += ((float) (n.getFat() * this.composition.get(n))) / 100;
			else
				sum += n.getFat();
		}
		
		return sum;
	}

	/**
	 * Indicates whether the nutritional values returned by the other methods
	 * refer to a conventional 100g quantity of nutritional element,
	 * or to a unit of element.
	 * 
	 * For the {@link Menu} class it must always return {@code false}:
	 * nutritional values are provided for the whole menu.
	 * 
	 * @return boolean 	indicator
	 */
	@Override
	public boolean per100g() {
		// nutritional values are provided for the whole menu.
		return false;
	}

	@Override
	public int compareTo(Menu o) {
		return this.getName().compareTo(o.getName());
	}
}
