package hydraulic;

/**
 * Represents a split element, a.k.a. T element
 * 
 * During the simulation each downstream element will
 * receive a stream that is half the input stream of the split.
 */

public class Split extends ElementExt {
	
	/*
	 * ATTRIBUTE FOR THE SPLIT ELEMENT
	 */
	private int numOutputs;
	private Element[] outputs;
	
	
	/*
	 * GETTERS AND SETTERS
	 */
	public int getNumOutputs() {
		return numOutputs;
	}
	public void setNumOutputs(int numOutputs) {
		this.numOutputs = numOutputs;
	}
	
	public void setOutputs(Element[] outputs) {
		this.outputs = outputs;
	}
	
	
	/**
	 * Constructor
	 * @param name
	 */
	public Split(String name) {
		super(name);
		this.className = "Split";
		
		/*
		 * Setting the number of outputs (two) and creating outputs array 
		 * of length two (we are in a simple split)
		 */
		this.numOutputs = 2;
		this.outputs = new Element[this.numOutputs];	

	}
    
	/**
	 * returns the downstream elements
	 * @return array containing the two downstream element
	 */
    public Element[] getOutputs(){
        return this.outputs;
    }

    /**
     * connect one of the outputs of this split to a
     * downstream component.
     * 
     * @param elem  the element to be connected downstream
     * @param noutput the output number to be used to connect the element
     */
	public void connect(Element elem, int noutput){
		
		if (noutput < 2)
			//CONNECTING ELEMENT WITH ITS OUTPUT
			this.outputs[noutput] = elem;
		
	}

	@Override
	public void simulate(Double inFlow, SimulationObserver observer) {
		
		//COMPUTING OUTPUT FLOW
		double outFlow = inFlow/2;
		
		//SHOWING DATA ABOUT THIS ELEMENT FLOW
		observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow, outFlow);
		
		//SIMULATING NEXT ELEMENTS
		this.outputs[0].simulate(outFlow, observer);
		this.outputs[1].simulate(outFlow, observer);
	}
	@Override
	public String layout(String layoutString) {
		layoutString = layoutString + "[" + this.getName() + "]" + "Split +-> " + this.getOutput().layout(layoutString);
		return layoutString;
	}
	@Override
	public void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck) {
		
		if (enableMaxFlowCheck) {
			//CHECKING IF THE INFLOW EXCEDES THE MAXIMUM VALUE
			if(inFlow > this.getMaxFlow())
				observer.notifyFlowError(getClassName(), getName(), inFlow, this.getMaxFlow());
			
			//COMPUTING OUTPUT FLOW
			double outFlow = inFlow/2;
			
			//SHOWING DATA ABOUT THIS ELEMENT FLOW
			observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow, outFlow);
			
			//SIMULATING NEXT ELEMENTS
			this.outputs[0].simulate(outFlow, observer, enableMaxFlowCheck);
			this.outputs[1].simulate(outFlow, observer, enableMaxFlowCheck);
		}
		else 
			this.simulate(inFlow, observer);		
	}
}
