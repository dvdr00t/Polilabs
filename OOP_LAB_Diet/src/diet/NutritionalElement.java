package diet;

/**
 * Interface representing a generic nutritional element, that is a component of the diet.
 * A nutritional element can be a raw material, a packaged product, a recipe, or a menu.
 * 
 * This interface must be implemented by the different classes that represent the components 
 * of the diet so as to allow easy computation of the nutritional values.
 *
 * The nutritional values can be interpreted in two different ways depending 
 * on the value returned by {@link #per100g()}:
 * <ul>
 * <li> values referred to 100g of nutritional element if {@link #per100g()} {@code == true}
 * <li> values referred to a portion of unit of nutritional element if {@link #per100g()} {@code == false}
 * </ul>
 */
public interface NutritionalElement {

	/**
	 * Retrieves the name of the nutritional element.
	 * Typically the name is unique within a specific category (e.g. raw materials)
	 * 
	 * @return the string containing the name
	 */
	public String getName();
	
	/**
	 * Retrieves the quantity of calories for the element.
	 * Such value can be referred to 100g of element (if {@link #per100g()} returns {@code true})
	 * or to a unit of element (if {@link #per100g()} returns {@code false}).
	 * 
	 * @return calories of the element
	 */
	public double getCalories();
	
	/**
	 * Retrieves the quantity of proteins for the element.
	 * Such value can be referred to 100g of element (if {@link #per100g()} returns {@code true})
	 * or to a unit of element (if {@link #per100g()} returns {@code false}).
	 * 
	 * @return proteins of the element
	 */
	public double getProteins();
	
	/**
	 * Retrieves the quantity of carbs for the element.
	 * Such value can be referred to 100g of element (if {@link #per100g()} returns {@code true})
	 * or to a unit of element (if {@link #per100g()} returns {@code false}).
	 * 
	 * @return carbs of the element
	 */
	public double getCarbs();
	
	/**
	 * Retrieves the quantity of fat for the element.
	 * Such value can be referred to 100g of element (if {@link #per100g()} returns {@code true})
	 * or to a unit of element (if {@link #per100g()} returns {@code false}).
	 * 
	 * @return fats of the element
	 */
	public double getFat();

	/**
	 * Indicates whether the nutritional values returned by the other methods
	 * refer to a conventional 100g quantity of nutritional element,
	 * or to a unit of element.
	 * 
	 * @return boolean indicator of the values
	 */
	public boolean per100g();
}
