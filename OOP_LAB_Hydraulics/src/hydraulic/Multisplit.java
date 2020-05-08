package hydraulic;

/**
 * Represents a multisplit element, an extension of the Split that allows many outputs
 * 
 * During the simulation each downstream element will
 * receive a stream that is determined by the proportions.
 */

public class Multisplit extends Split {
	
	
	/*
	 * Attributes for the class
	 */
	private double[] proportion;
	

	/**
	 * Constructor
	 * @param name
	 * @param numOutput
	 */
	public Multisplit(String name, int numOutput) {
		super(name); //you can edit also this lines
		
		/*
		 * Setting the number of outputs (numOutput) and
		 * creating an array of length numOutput (we are in a
		 * multisplit)
		 */
		this.setNumOutputs(numOutput);
		this.setOutputs(new Element[this.getNumOutputs()]);
		
		/*
		 * Creating array for proportion of length numOutput
		 */
		this.proportion = new double[numOutput];
		
	}
    
	/**
	 * returns the downstream elements
	 * @return array containing the two downstream element
	 */
    public Element[] getOutputs(){
        return super.getOutputs();
    }

    /**
     * connect one of the outputs of this split to a
     * downstream component.
     * 
     * @param elem  the element to be connected downstream
     * @param noutput the output number to be used to connect the element
     */
	public void connect(Element elem, int noutput){
		
		if (noutput < this.getNumOutputs())
			//CONNECTING ELEMENT WITH ITS OUTPUT
			this.getOutputs()[noutput] = elem;
		
	}
	
	/**
	 * Define the proportion of the output flows w.r.t. the input flow.
	 * 
	 * The sum of the proportions should be 1.0 and 
	 * the number of proportions should be equals to the number of outputs.
	 * Otherwise a check would detect an error.
	 * 
	 * @param proportions the proportions of flow for each output
	 */
	public void setProportions(double... proportions) {
		
		double sum = 0;
		for (double p: proportions)
			sum += p;
		
		if (sum == 1) {
			int i = 0;
			for (double p: proportions)
				this.proportion[i++] = p;
		}
		else 
			return;	
	}
	
	
	@Override
	public void simulate(Double inFlow, SimulationObserver observer) {
		
		//COMPUTING OUTPUT FLOWS
		double[] outFlows = new double[this.getNumOutputs()];
		for (int i = 0; i < outFlows.length; i++)
			outFlows[i] = inFlow * this.proportion[i];
		
		//SHOWING DATA ABOUT THIS ELEMENT FLOW
		observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlows);
		
		//SIMULATE NEXT ELEMENT
		Element[] next = this.getOutputs();
		for (int i = 0; i < next.length; i++)
			next[i].simulate(inFlow*this.proportion[i], observer);
	}
	
	@Override
	public void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck) {
		
		if (enableMaxFlowCheck) {
			//CHECKING IF THE INFLOW EXCEDES THE MAXIMUM VALUE
			if (inFlow > this.getMaxFlow())
				observer.notifyFlowError(getClassName(), getName(), inFlow, getMaxFlow());
			
			//COMPUTING OUTPUT FLOWS
			double[] outFlows = new double[this.getNumOutputs()];
			for (int i = 0; i < outFlows.length; i++)
				outFlows[i] = inFlow * this.proportion[i];
			
			//SHOWING DATA ABOUT THIS ELEMENT FLOW
			observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlows);
			
			//SIMULATE NEXT ELEMENT
			Element[] next = this.getOutputs();
			for (int i = 0; i < next.length; i++)
				next[i].simulate(inFlow*this.proportion[i], observer);
			
		}
		else 
			this.simulate(inFlow, observer);
	}	
}
