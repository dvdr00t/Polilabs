package hydraulic;

 /**
 * Main class that act as a container of the elements for
 * the simulation of an hydraulics system 
 * 
 */
public class HSystemExt extends HSystem{
	
	/**
	 * Attributes
	 */
	private String layoutString = "";
	
	
	
	/**
	 * Prints the layout of the system starting at each Source
	 */
	public String layout(){
		
//		//CONVERTING LINKED LIST TO AN ARRAY
//		Element[] elementsArray = new Element[this.elements.size()];
//		this.elements.toArray(elementsArray);
//				
//		//EVALUATING EVERY ELEMENT LOOKING FOR THE SOURCE ELEMENT
//		for (int i = 0; i < elementsArray.length; i++) {
//					
//			Element current = elementsArray[i];
//					
//			//CHECKING IF WE ARE IN A STARTING POINT
//			if (current instanceof Source) {
//				this.layoutString = current.layout(layoutString);
//			}
//		}
//			
//		return this.layoutString;
		
		return null;
		
		
	}
	
	/**
	 * Deletes a previously added element with the given name from the system
	 */
		
	public void deleteElement(String name) {
		
		Element removed = null;
		
		//CONVERTING LINKED LIST TO AN ARRAY
		Element[] elementsArray = new Element[this.elements.size()];
		this.elements.toArray(elementsArray);
		
		//SEARCHING THE ELEMENT THAT HAS TO BE DELETED
		for (int i = 0; i < elementsArray.length; i++)
			if (elementsArray[i].getName().equals(name))
				removed = this.elements.remove(i);
		
		//PERFORMING NEW CONNECTIONS
		if (removed == null)
			return;
		else if (removed instanceof Split || removed instanceof Multisplit)
			return;
		else if (removed instanceof Tap)
			removed.getInput().connect(removed.getOutput());
		else if (removed instanceof Source || removed instanceof Sink)
			return;
		
	}

	/**
	 * starts the simulation of the system; if enableMaxFlowCheck is true,
	 * checks also the elements maximum flows against the input flow
	 */
	public void simulate(SimulationObserverExt observer, boolean enableMaxFlowCheck) {
		
		//Evaluating the maxFlow
		if (enableMaxFlowCheck) {
			
			//CONVERTING LINKED LIST TO AN ARRAY
			Element[] elementsArray = new Element[this.elements.size()];
			this.elements.toArray(elementsArray);
			
			//EVALUATING EVERY ELEMENT LOOKING FOR THE SOURCE ELEMENT
			for (int i = 0; i < elementsArray.length; i++) {
				
				Element current = elementsArray[i];
				
				//CHECKING IF WE ARE IN A STARTING POINT
				if (current instanceof Source) {
					
					//STARTING SIMULATION
					((Source) current).simulate(observer.NO_FLOW, observer, enableMaxFlowCheck);
				}
			}
					
		}
		else
			this.simulate(observer);
	}
	
}
