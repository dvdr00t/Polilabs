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
public interface SimulationObserver {
	/**
	 * Constant used to mark a flow as missing,
	 * (e.g. for the classes {@link Source} and {@link Sink})
	 */
	public final static double NO_FLOW = Double.NaN;
	
	/**
	 * Notification method.
	 * 
	 * In case of missing flow (e.g. for the classes {@link Source}
	 * and {@link Sink}) where either the input or output flows
	 * are undefined the constant {@link #NO_FLOW} has to be used.
	 * 
	 * @param type class name of the element 
	 * @param name name of the element
	 * @param inFlow input flow for the element
	 * @param outFlow output flow for the element
	 */
	void notifyFlow(String type, String name, double inFlow, double... outFlow);
	
	/**
	 * can be used to check whether any flow is present, (i.e. it is not a {@link #NO_FLOW}).
	 * 
	 * @param flow the flows to be tested
	 * 
	 * @return {@code true} if the flow is present
	 */
	static boolean exists(double... flow) {
		boolean res=false;
		for(double f : flow) {
			res |= ! Double.isNaN(f);
		}
		return res;
	}
}
