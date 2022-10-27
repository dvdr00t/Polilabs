package hydraulic;

import java.util.Observer;

/**
 * Represents a source of water, i.e. the initial element for the simulation.
 *
 * The status of the source is defined through the method
 * {@link #setFlow(double) setFlow()}.
 */
public class Source extends ElementExt {
	
	public Source(String name) {
		super(name);
		this.className = "Source";
	}

	/**
	 * defines the flow produced by the source
	 * 
	 * @param flow
	 */
	public void setFlow(double flow){
		this.setInputFlow(SimulationObserver.NO_FLOW);
		this.setOutputFlow(flow);
	}

	/*
	 * SIMULATE METHOD FOR THE SOURCE
	 */
	
	@Override
	public void simulate(Double inFlow, SimulationObserver observer) {
		
		//GETTING OUTPUTFLOW
		Double outFlow = this.getOutputFlow();
		
		//SHOWING DATA ABOUT THIS ELEMENT FLOW
		observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
		
		//SIMULATE NEXT ELEMENT FLOW
		this.getOutput().simulate(outFlow, observer);
		
	}

	@Override
	public String layout(String layoutString) {
		layoutString = layoutString + "[" + this.getName() + "]" + "Source -> " + this.getOutput().layout(layoutString);
		return layoutString;
		
	}

	@Override
	public void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck) {
		
		//GETTING OUTPUTFLOW
		Double outFlow = this.getOutputFlow();
						
		//SHOWING DATA ABOUT THIS ELEMENT FLOW
		observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
						
		//SIMULATE NEXT ELEMENT FLOW
		this.getOutput().simulate(outFlow, observer, enableMaxFlowCheck);
		
	}



	
}
