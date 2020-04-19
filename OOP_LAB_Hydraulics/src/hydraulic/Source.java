package hydraulic;

import java.util.Observer;

/**
 * Represents a source of water, i.e. the initial element for the simulation.
 *
 * The status of the source is defined through the method
 * {@link #setFlow(double) setFlow()}.
 */
public class Source extends Element {
	
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

	
}
