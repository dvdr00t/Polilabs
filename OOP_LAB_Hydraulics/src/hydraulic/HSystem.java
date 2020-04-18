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
			Element next;
			
			//CHECKING IF WE ARE IN A STARTING POINT
			if (current instanceof Source) {
				
				//SINCE WE ARE IN A SOURCE, WE CAN PRINT ITS FLOW PARAMETERS
				observer.notifyFlow(current.getClassName(), current.getName(), current.getInputFlow(), current.getOutputFlow());
				
				
				//GOING THROUGH THE FLOW UNTIL WE REACH THE SINKS
				while (!(current instanceof Sink)) {
					Element next = current.getOutput();
					
					//CHECKING IF THE NEXT ELEMENT IS A SPLIT OR NOT
					if (next == null)
						return;
					else if (next instanceof Split) {
						
						//SETTING NEW PARAMETERS
						next.setInputFlow(current.getOutputFlow());
						next.setOutputFlow(next.getInputFlow());
						observer.notifyFlow(((Split) next).getClassName(), next.getName(), next.getInputFlow(), next.getOutputFlow());
						
						//SPLITTING
						Element[] nextAfterSplit = ((Split) next).getOutputs();
						nextAfterSplit[0].setInputFlow(next.getOutputFlow()/2);
						nextAfterSplit[1].setInputFlow(next.getOutputFlow()/2);
						nextAfterSplit[0].setOutputFlow(nextAfterSplit[0].getInputFlow());
						nextAfterSplit[1].setOutputFlow(nextAfterSplit[1].getInputFlow());
						observer.notifyFlow(nextAfterSplit[0].getClassName(), nextAfterSplit[0].getName(), nextAfterSplit[0].getInputFlow(), nextAfterSplit[0].getOutputFlow());
						observer.notifyFlow(nextAfterSplit[1].getClassName(), nextAfterSplit[1].getName(), nextAfterSplit[1].getInputFlow(), nextAfterSplit[1].getOutputFlow());
						
						current = current.getOutput();
						
					}
					else {
						
						//SETTING NEW PARAMETERS
						
						observer.notifyFlow(next.getClassName(), next.getName(), next.getInputFlow(), next.getOutputFlow());
						current = current.getOutput();
					}
				};
			}
		}
	}

	
	/*
	 * DEPTH-FIRST SEARCH UTIL METHOD
	 */
	void DFSUtil(Element[] elementsArray, int v, Boolean visited[]) {
		
		//MARK THE CURRENT NODE A VISITED AND COMPUTE ITS FLOWS
		visited[v] = true;
		elementsArray[v].setInputFlow(inputFlow);
		
	}
}
