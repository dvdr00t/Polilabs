package hydraulic;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
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
		
		//CONVERTING LINKED LIST TO AN ARRAY
		Element[] elementsArray = new Element[this.elements.size()];
		this.elements.toArray(elementsArray);
		
		//EVALUATING EVERY ELEMENT LOOKING FOR THE SOURCE ELEMENT
		for (int i = 0; i < elementsArray.length; i++) {
			
			Element current = elementsArray[i];
			
			//CHECKING IF WE ARE IN A STARTING POINT
			if (current instanceof Source) {
				
				//SINCE WE ARE IN A SOURCE, WE CAN PRINT ITS FLOW PARAMETERS
				observer.notifyFlow(current.getClassName(), current.getName(), current.getInputFlow(), current.getOutputFlow());
				
				//VISITING TREE
				treeVisit(current.getOutput(), current.getOutputFlow(), observer);
	
			}
		}
	}

	
	/*
	 * VISIT TO THE TREE
	 */
	private void treeVisit(Element vertex, double inFlow, SimulationObserver observer) {
		
		
		//TERMINAL CONDITION
		if (vertex == null)
			return;
		
		//CHECKING IF WE ARE IN A SPLIT
		if (vertex instanceof Split) {
			
			//GETTING OUTPUTS
			Element[] next = ((Split) vertex).getOutputs();
			
			//COMPUTING FLOW
			vertex.setInputFlow(inFlow);
			vertex.setOutputFlow(vertex.getInputFlow());
			
			//SHOWING DATA
			observer.notifyFlow(vertex.getClassName(), vertex.getName(), vertex.getInputFlow(), vertex.getOutputFlow());
			
			//VISIT LEFT CHILD
			treeVisit(next[0], vertex.getOutputFlow()/2, observer);
			
			//VISIT RIGHT CHILD
			treeVisit(next[1], vertex.getOutputFlow()/2, observer);
			
		}
		else {
			
			//COMPUTING FLOWS
			vertex.setInputFlow(inFlow);
			vertex.setOutputFlow(vertex.getInputFlow());
			
			//SHOWING DATA
			observer.notifyFlow(vertex.getClassName(), vertex.getName(), vertex.getInputFlow(), vertex.getOutputFlow());
			
			//VISITING CHILD
			treeVisit(vertex.getOutput(), vertex.getOutputFlow(), observer);
			
		}
		
		
		
	}
}
