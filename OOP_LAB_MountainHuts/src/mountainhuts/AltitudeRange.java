package mountainhuts;

public class AltitudeRange {

	/*
	 * ATTRIBUTES
	 */
	private int minValue;
	private int maxValue;
	
	
	
	/**
	 * Constructor
	 * 
	 * @param minValue
	 * @param maxValue
	 */
	public AltitudeRange(String range) {
		String[] values = range.split("-");
		
		this.minValue = Integer.parseInt(values[0]);
		this.maxValue = Integer.parseInt(values[1]);
	}


	
	/*
	 * Getters and setters
	 */

	public int getMinValue() {
		return minValue;
	}
	public int getMaxValue() {
		return maxValue;
	}
	
	
	@Override
	public String toString() {
		return this.minValue + "-" + this.maxValue;
	}

}
