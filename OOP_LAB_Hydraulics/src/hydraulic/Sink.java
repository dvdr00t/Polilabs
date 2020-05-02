package hydraulic;

/**
 * Represents the sink, i.e. the terminal element of a system
 *
 */
public class Sink extends ElementExt {

	/**
	 * Constructor
	 * @param name
	 */
	public Sink(String name) {
		super(name);
		this.className = "Sink";
	}

	@Override
	public void simulate(Double inFlow, SimulationObserver observer) {
		
		//COMPUTING OUTPUT FLOW (SETTING TO NaN)
		double outFlow = observer.NO_FLOW;
		
		//SHOWING DATA ABOUT THIS ELEMENT
		observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
		
		//NO MORE SIMULATION FROM A SINK
		
	}

	@Override
	public String layout(String layoutString) {
		return layoutString + "[" + this.getName() + "]" + "Sink * ";
	}

	@Override
	public void simulateMaximumFlow(Double inFlow, SimulationObserverExt observer) {
		
		//CHECKING THE INFLOW EXCEDES THE MAXIMUM VALUE
		if(inFlow > this.getMaxFlow())
			observer.notifyFlowError(getClassName(), getName(), inFlow, this.getMaxFlow());
		else
			this.simulate(inFlow, observer);
		
	}
	
}
