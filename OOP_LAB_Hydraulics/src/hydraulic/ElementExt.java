package hydraulic;

public abstract class ElementExt extends Element{
	
	/*
	 * ATTRIBUTES FOR THE ELEMENT EXTENDED
	 */
	private double maxFlow;

	/*
	 * CONSTRUCTOR
	 */
	public ElementExt(String name) {
		super(name);
	}

	/*
	 * GETTERS AND SETTERS
	 */
	public void setMaxFlow(double maxFlow) {

		//CHECKING IF WE ARE IN A SOURCE (maxFlow does not affect)
		if (this instanceof Source)
			return;
		
		//SETTING MAXFLOW
		this.maxFlow = maxFlow;
	}
	public double getMaxFlow() {
		return this.maxFlow;
	}
	
	public abstract void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck);
	
}
