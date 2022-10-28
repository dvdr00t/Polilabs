package hydraulic;

import static hydraulic.SimulationObserver.exists;

import java.util.Arrays;

/**
 * Sample implementation of {@link SimulationObserver}.
 * This simulation observer simply prints out the notification info
 * and keeps count of how many notification it receive.
 * 
 * This class can be used for debugging purposes.
 *  
 */
public class PrintingObserverExt implements SimulationObserverExt{
	private int countNotifications = 0;
	private int countErrorNotifications = 0;
	
	@Override
	public void notifyFlow(String type, String name, double inFlow, double... outFlow) {
		System.out.println(type + " " + name + ": ");
		if(exists(inFlow)) System.out.println("\t-> in flow=" + inFlow);
		if(exists(outFlow)) System.out.println("\t<- out flow=" + Arrays.toString(outFlow));
		countNotifications++;
	}
	
	public int getCount() {
		return countNotifications;
	}

	public int getErrorCount() {
		return countErrorNotifications;
	}
	
	@Override
	public void notifyFlowError(String type, String name, double inFlow, double maxFlow) {
		System.err.println(type + " " + name + " inFlow is " + inFlow + " but maximum flow is " + maxFlow);
		countErrorNotifications++;
	}
}
