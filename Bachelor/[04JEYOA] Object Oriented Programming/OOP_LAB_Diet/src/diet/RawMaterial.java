package diet;

public class RawMaterial extends EdibleElement implements NutritionalElement{
	
	
	/*
	 * CONSTRUCTOR
	 */
	public RawMaterial(String name, double calories, double proteins, double carbs, double fat) {
		this.name = name;
		this.calories = calories;
		this.proteins = proteins;
		this.carbs = carbs;
		this.fat = fat;
	}
	
	
	@Override
	public String toString() {
		return this.name;
	}
	
	@Override
	public String getName() {
		return this.name;
	}

	@Override
	public double getCalories() {
		return this.calories;
	}

	@Override
	public double getProteins() {
		return this.proteins;
	}

	@Override
	public double getCarbs() {
		return this.carbs;
	}

	@Override
	public double getFat() {
		return this.fat;
	}

	@Override
	public boolean per100g() {
		// RawMaterial values are always expressed to 100g
		return true;
	}

}
