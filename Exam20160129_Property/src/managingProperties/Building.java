package managingProperties;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Building {
	
	/*
	 * ATTRIBUTES
	 */
	private String buildingID;
	private int numberOfApartments;
	private List<Apartment> apartments;
	private Map<String, Integer> workerAndCost;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param buildingID
	 * @param numberOfApartments
	 */
	public Building(String buildingID, int numberOfApartments) {
		this.buildingID = buildingID;
		this.numberOfApartments = numberOfApartments;
		this.apartments = new LinkedList<Apartment>();
		
		//Adding to the list all the new apartments
		for (int id = 0; id < this.numberOfApartments; id++)
			this.apartments.add(new Apartment(this, id));
		
		//Creating the map WorkAndCost
		this.workerAndCost = new TreeMap<String, Integer>();

	}

	
	/*
	 * GETTERS
	 */
	public String getBuildingID() {
		return buildingID;
	}
	public int getNumberOfApartments() {
		return numberOfApartments;
	}
	
	/**
	 * Check if the apartment has already been occupied
	 * 
	 * @param apartmentID
	 * @return
	 */
	public boolean hasOwner (int apartmentID) {
		return this.apartments.get(apartmentID-1).isOccupied();
	}
	
	/**
	 * Get the apartment
	 * 
	 * @param apartmentID
	 * @return
	 */
	public Apartment getApartment (int apartmentID) {
		return this.apartments.get(apartmentID-1);
	}
	
	/**
	 * Add to the map a workerID with its cost. If the workerID already exists, the previous
	 * value is uploaded.
	 * 
	 * @param workerID
	 * @param cost
	 */
	public void addWorkAndCost(String workerID, int cost) {
		
		//Checking if the workerID already exists
		if (this.workerAndCost.containsKey(workerID))
			this.workerAndCost.replace(workerID, this.workerAndCost.get(workerID) + cost);
		
		//Adding a new worker ID
		this.workerAndCost.put(workerID, cost);
	}
	
	/**
	 * Get the map WorkAndCost
	 * 
	 * @return the map sorted by the workerID
	 */
	public Map<String, Integer> getWorkAndCost() {
		return new TreeMap<String, Integer>(this.workerAndCost);
	}
	

}
