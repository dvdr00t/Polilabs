package transactions;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Region {
	
	/*
	 * ATTRIBUTES
	 */
	private String regionName;
	private List<String> places;
	private Map<String, Carrier> carriersSupplying;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param regionName
	 * @param places
	 */
	public Region(String regionName) {
		this.regionName = regionName;
		this.places = new LinkedList<String>();
		this.carriersSupplying = new LinkedHashMap<String, Carrier>();
	}

	/*
	 * GETTERS
	 */
	public String getRegionName() {
		return regionName;
	}
	public List<String> getPlaces() {
		return new LinkedList<String>(this.places);
	}
	
	/**
	 * Add a new place to the list of places for this region
	 * 
	 * @param place
	 */
	public void addPlace(String place) {
		if (!this.places.contains(place))
			this.places.add(place);
	}
	
	/**
	 * Add a new carrier to the list of carriers that supply this region
	 * 
	 * @param carrier
	 */
	public void addCarrier(Carrier carrier) {
		this.carriersSupplying.put(carrier.getCarrierName(), carrier);
	}
	
	/**
	 * Get the list of carriers name that supply this region
	 * 
	 * @return an ordered list
	 */
	public List<String> getCarriers() {
		return this.carriersSupplying.values()
				.stream()
				.map(c -> c.getCarrierName())
				.sorted()
				.collect(Collectors.toList());
	}
	

}
