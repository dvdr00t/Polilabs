package transactions;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Carrier {
	
	/*
	 * ATTRIBUTES
	 */
	private String carrierName;
	private Map<String, Region> regionsSupplied;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param carrierName
	 */
	public Carrier(String carrierName) {
		this.carrierName = carrierName;
		this.regionsSupplied = new LinkedHashMap<String, Region>();
	}
	
	/**
	 * Add a new region to the list of regions supplied by this carrier
	 * 
	 * @param region
	 */
	public void addRegion(Region region) {
		if (!this.regionsSupplied.containsKey(region.getRegionName())) {
			this.regionsSupplied.put(region.getRegionName(), region);
			region.addCarrier(this);
		}
	}

	/*
	 * GETTERS
	 */
	public String getCarrierName() {
		return carrierName;
	}
	public List<String> getRegionsSupplied() {
		return this.regionsSupplied.keySet()
				.stream()
				.sorted()
				.collect(Collectors.toList());
	}
	
	

}
