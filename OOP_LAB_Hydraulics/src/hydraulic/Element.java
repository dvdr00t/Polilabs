package hydraulic;

/**
 * Represents the generic abstract element of an hydraulics system.
 * It is the base class for all elements.
 *
 * Any element can be connect to a downstream element
 * using the method {@link #connect(Element) connect()}.
 */
public class Element {
	
	/**
	 * Constructor
	 * @param name the name of the element
	 */
	public Element(String name){
		// TODO: to be implemented
	}

	/**
	 * getter method
	 * @return the name of the element
	 */
	public String getName(){
		// TODO: to be implemented
		return null;
	}
	
	/**
	 * Connects this element to a given element.
	 * The given element will be connected downstream of this element
	 * @param elem the element that will be placed downstream
	 */
	public void connect(Element elem){
		// TODO: to be implemented
	}
	
	/**
	 * Retrieves the element connected downstream of this
	 * @return downstream element
	 */
	public Element getOutput(){
		// TODO: to be implemented
		return null;
	}
	
}
