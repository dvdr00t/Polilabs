package hydraulic;

/**
 * Represents a tap that can interrupt the flow.
 * 
 * The status of the tap is defined by the method
 * {@link #setOpen(boolean) setOpen()}.
 */

public class Tap extends ElementExt {
	
	/*
	 * 	ATTRIBUTE FOR THE TAP ELEMENT
	 */
	private boolean status; 	//OPEN OR CLOSE
	
	public Tap(String name) {
		super(name);
		this.className = "Tap";
	}
	
	/**
	 * Defines whether the tap is open or closed.
	 * 
	 * @param open  opening level
	 */
	public void setOpen(boolean open){
		this.status = open;
	}

	public boolean getStatus() {
		return status;
	}

	@Override
	public void simulate(Double inFlow, SimulationObserver observer) {
		
		//COMPUTING OUTPUT FLOW
		double outFlow = inFlow;
		
		//CHECKING IF THE TAP IS OPEN OR NOT
		if (this.status) {
			
			//SHOWING DATA ABOUT THIS ELEMENT
			observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);

			//SIMULATING NEXT ELEMENT FLOW
			this.getOutput().simulate(outFlow, observer);
		}
		else {
			
			//SETTING TO ZERO THE OUTPUT AND SHOWING DATA ABOUT THIS ELEMENT
			outFlow = 0.0;
			observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
			
			//SIMULATING NEXT ELEMENT FLOW
			this.getOutput().simulate(outFlow, observer);
		}
		
	}

	@Override
	public String layout(String layoutString) {
		layoutString = layoutString + "[" + this.getName() + "]" + "Tap -> " + this.getOutput().layout(layoutString);
		return layoutString;
	}

	@Override
	public void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck) {
		
		
		if (enableMaxFlowCheck) {
			//CHECKING IF THE INFLOW EXCEDES THE MAXIMUM VALUE
			if(inFlow > this.getMaxFlow())
				observer.notifyFlowError(getClassName(), getName(), inFlow, this.getMaxFlow());
			
			//COMPUTING OUTPUT FLOW
			double outFlow = inFlow;
			
			//CHECKING IF THE TAP IS OPEN OR NOT
			if (this.status) {
				
				//SHOWING DATA ABOUT THIS ELEMENT
				observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
				
				//SIMULATING NEXT ELEMENT FLOW
				this.getOutput().simulate(outFlow, observer, enableMaxFlowCheck);
			}
			else {
				
				//SETTING TO ZERO THE OUTPUT AND SHOWING DATA ABOUT THIS ELEMENT
				outFlow = 0.0;
				observer.notifyFlow(this.getClassName(), this.getName(), inFlow, outFlow);
				
				//SIMULATING NEXT ELEMENT FLOW
				this.getOutput().simulate(outFlow, observer, enableMaxFlowCheck);
			}
		}
		else {
			this.simulate(inFlow, observer);
		}
	}
}
