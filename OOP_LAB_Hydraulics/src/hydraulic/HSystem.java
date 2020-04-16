package hydraulic;

/**
 * Main class that act as a container of the elements for
 * the simulation of an hydraulics system 
 * 
 */
public class HSystem {
	
	/**
	 * Adds a new element to the system
	 * @param elem
	 */
	public void addElement(Element elem){
		// TODO: to be implemented
	}
	
	/**
	 * returns the element added so far to the system.
	 * If no element is present in the system an empty array (length==0) is returned.
	 * 
	 * @return an array of the elements added to the hydraulic system
	 */
	public Element[] getElements(){
		// TODO: to be implemented
		return null;
	}
	
	/**
	 * Prints the layout of the system starting at each Source
	 */
	public String layout(){
		// TODO: to be implemented
		return null;
	}
	
	/**
	 * starts the simulation of the system
	 */
	public void simulate(SimulationObserver observer){
		// TODO: to be implemented
	}

}
