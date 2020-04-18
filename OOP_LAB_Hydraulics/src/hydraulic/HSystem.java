package hydraulic;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Main class that act as a container of the elements for
 * the simulation of an hydraulics system 
 * 
 */
public class HSystem {
	
	
	/*
	 * ATTRIBUTE OF THE HYDRAULIC SYSTEM
	 */
	private LinkedList<Element> elements = new LinkedList<Element>();
	
	/**
	 * Adds a new element to the system
	 * @param elem
	 */
	public void addElement(Element elem){
		this.elements.add(elem);
	}
	
	/**
	 * returns the element added so far to the system.
	 * If no element is present in the system an empty array (length==0) is returned.
	 * 
	 * @return an array of the elements added to the hydraulic system
	 */
	public Element[] getElements(){
		
		//CREATING A NEW ARRAY OF ELEMENTS (IN ORDER TO CONVERT THE LIST TO AN ARRAY SUCCESSFULLY)
		Element[] elementsArray = new Element[this.elements.size()];
		
		//RETURNING THE LIST CONVERTED TO THE ARRAY
		return this.elements.toArray(elementsArray);
		
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
