package hydraulic;

/**
 * Interface for simulation observers.
 * 
 * Any class interested in being notified about the
 * flow computations performed during the simulation
 * must implement this interface.
 *  
 *
 */
public interface SimulationObserverExt extends SimulationObserver{
	/**
	 * Notification method, to be used to raise an error if the 
	 * input flow of an element exceeds its maximum flow rate.
	 * 
	 * @param type class name of the element 
	 * @param name name of the element
	 * @param inFlow input flow for the element
	 * @param maxFlow the maximum flow for the element
	 */
	void notifyFlowError(String type, String name, double inFlow, double maxFlow);
}
