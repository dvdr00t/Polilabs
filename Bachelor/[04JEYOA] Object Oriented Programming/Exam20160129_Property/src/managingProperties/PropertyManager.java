package managingProperties;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class PropertyManager {
	
	/*
	 * ATTRIBUTES
	 */
	private Map<String, Building> buildings;
	private Map<String, Owner> owners;
	private Map<String, Profession> professions;
	private Map<Integer, Request> requests;
	
	/**
	 * CONSTRUCTOR
	 */
	public PropertyManager() {
		this.buildings = new LinkedHashMap<String, Building>();
		this.owners = new LinkedHashMap<String, Owner>();
		this.professions = new LinkedHashMap<String, Profession>();
		this.requests = new LinkedHashMap<Integer, Request>();
	}

	/**
	 * Add a new building to the list if the building does not already exists
	 * or the number of apartments does not match the requirements
	 * 
	 * @param String the building ID
	 * @param int the number of apartments
	 */
	public void addBuilding(String building, int n) throws PropertyException {
		
		/*
		 * Checking if the building does not already exists or the number of apartments 
		 * does not match the requirements
		 */
		if (this.buildings.containsKey(building) || (n < 1 || n > 100))
			throw new PropertyException();
		
		//Adding the new building
		this.buildings.put(building, new Building(building, n));	
	}

	/**
	 * Add a new owner if matches the conditions.
	 * 
	 * @param owner
	 * @param apartments
	 * @throws PropertyException
	 */
	public void addOwner(String owner, String... apartments) throws PropertyException {
		
		//Checking if the owner is not already existing
		if (this.owners.containsKey(owner))
			throw new PropertyException();
		
		//Creating the new owner to be added
		Owner newOwner = new Owner(owner, apartments);
		
		//Checking if the both building and the apartment exist 
		String buildingID;
		String apartmentID;
		for (String s: apartments) {
			buildingID = s.split(":")[0];
			apartmentID = s.split(":")[1];
			
			//checking if the building exists
			if (!this.buildings.containsKey(buildingID))
				throw new PropertyException();
			
			//Checking if the apartment exists
			if (Integer.parseInt(apartmentID) < 1 || Integer.parseInt(apartmentID) > this.buildings.get(buildingID).getNumberOfApartments())
				throw new PropertyException();
			
			/*
			 * Checking if the apartments has already been occupied, if the apartment is not occupied yet,
			 * add the owner to the apartment
			 */
			if (this.buildings.get(buildingID).hasOwner(Integer.parseInt(apartmentID)))
				throw new PropertyException();
			this.buildings.get(buildingID).getApartment(Integer.parseInt(apartmentID)).setOwner(newOwner);
		}
		
		//Adding the owner with the list of apartment in the list of owners
		this.owners.put(owner, newOwner);
		
	}

	/**
	 * Returns a map ( number of apartments => list of buildings ) 
	 * 
	 * @param void
	 * @return A sorted map
	 */
	public SortedMap<Integer, List<String>> getBuildings() {
		
		/*
		 * Transform a COLLECTION into a new MAP that is not defined as DEFAULT in the Eclipse using
		 * the GROUPING BY function with three parameters.
		 * 
		 * .stream() transform the COLLECTION into a stream
		 * .sorted() sort the given stream using a new Comparator
		 * .collect(Collectors.GroupingBy()) transform the stream into a new MAP
		 * 		I)	LambdaFunction -> Key of the new map		(FUNCTION CLASSIFIER)
		 * 	   II)	Create a specific new map					(SUPPLIER FACTORY)
		 * 	  III)	Fill the map created by the factory			(COLLECTOR DOWNSTREAM)
		 */
		return this.buildings.values()
				.stream()
				.sorted(new Comparator<Building>() {

					@Override
					public int compare(Building o1, Building o2) {
						return o1.getBuildingID().compareTo(o2.getBuildingID());
					}
					
				})
				.collect(Collectors.groupingBy(b -> b.getNumberOfApartments(), 
												TreeMap::new, 
												Collectors.mapping(Building::getBuildingID, 
																	Collectors.toList())));
		
	}

	/**
	 * Add a new profession with the list of workers ID that refers to the workers
	 * that work with this profession
	 * 
	 * @param profession
	 * @param professionals
	 * @throws PropertyException
	 */
	public void addProfessionals(String profession, String... professionals) throws PropertyException {
		
		//Checking if the profession already exists
		if (this.professions.containsKey(profession))
			throw new PropertyException();
		
		//Checking if the new worker has not already been inserted
		for (Profession p: this.professions.values())
			for (String s: professionals)
				if (p.hasWorker(s))
					throw new PropertyException();
		
//		//Using ANYMATCH
//		boolean found = this.professions.values().stream()
//								 .map(p -> p.getWorkers())
//								 .anyMatch(p -> new LinkedList<String>(Arrays.asList(professionals))
//										 											.stream()
//										 											.anyMatch(p::contains));
//		if (found)
//			throw new PropertyException();
		
		//Adding the new profession to the list of profession
		this.professions.put(profession, new Profession(profession));
				
		//Adding the whole list of workers to this profession
		for (String s: professionals)
			this.professions.get(profession).addWorker(s);
	}
	
	/**
	 * Returns a map ( profession => number of workers )
	 * 
	 * @param void
	 * @return SortedMap
	 */
	public SortedMap<String, Integer> getProfessions() {
		
		//Create the map that will be returned
		SortedMap<String, Integer> toBeReturned = new TreeMap<String, Integer>();
		
		//Adding all the value from profession to the map
		this.professions.values()
				.stream()
				.forEach(p -> {
					toBeReturned.put(p.getProfessionName(), p.getNumberOfWorker());
				});
		
		return toBeReturned;
	}

	/**
	 * Create and add a new pending request
	 * 
	 * @param owner
	 * @param apartment
	 * @param profession
	 * @return
	 * @throws PropertyException
	 */
	public int addRequest(String owner, String apartment, String profession) throws PropertyException {
		
		//Checking if the owner exists
		if (!this.owners.containsKey(owner))
			throw new PropertyException();
		
		//Checking if the profession is valid
		if (!(profession.compareTo("plumber") == 0 || profession.compareTo("electrician") == 0 || profession.compareTo("mason") == 0))
			throw new PropertyException();
		
		String buildingID;
		String apartmentID;
		buildingID = apartment.split(":")[0];
		apartmentID = apartment.split(":")[1];
		
		//Checking if the building exists
		if (!this.buildings.containsKey(buildingID))
			throw new PropertyException();
		
		//Checking if the apartment exists
		if (Integer.parseInt(apartmentID) < 1 || Integer.parseInt(apartmentID) > this.buildings.get(buildingID).getNumberOfApartments())
			throw new PropertyException();
		
		//checking if the owner that called the request is for real the owner of the apartment
		if(this.owners.get(owner) != this.buildings.get(buildingID).getApartment(Integer.parseInt(apartmentID)).getOwner())
			throw new PropertyException();
		
		//Adding the request
		int id = this.requests.size()+1;
		Owner requirent = this.owners.get(owner);
		Apartment apartmentToBeAdded = this.buildings.get(buildingID).getApartment(Integer.parseInt(apartmentID));
		this.requests.put(id, new Request(id, requirent, apartmentToBeAdded, profession));
		
		//Returning the number of the request
		return id;
	}

	/**
	 * Assign the request to the given worker if it matches the condition
	 * 
	 * @param requestN
	 * @param professional
	 * @throws PropertyException
	 */
	public void assign(int requestN, String professional) throws PropertyException {
		
		//Checking if the request exists
		if (!this.requests.containsKey(requestN))
			throw new PropertyException();
		
		//Checking if the request is pending
		if (!this.requests.get(requestN).isPending())
			throw new PropertyException();
		
		//Checking if the worker works for this profession
		if (!this.professions
				.get( this.requests.get(requestN).getProfession() ).hasWorker(professional))
			
			throw new PropertyException();
		
		//Set the worker working on this request
		this.requests.get(requestN).setWorkerID(professional);
		
		//Set the state of the request as assigned
		this.requests.get(requestN).setAssigned();
	}
	
	/**
	 * Retrieves all the requests in order
	 * 
	 * @return the list of requests
	 */
	public List<Integer> getAssignedRequests() {
		
		return this.requests.values()
				.stream()
				.filter(r -> r.isAssigned())
				.map(r -> r.getId())
				.collect(Collectors.toList());
	}


	/**
	 * Set the cost of the request
	 * 
	 * @param requestN
	 * @param amount
	 * @throws PropertyException
	 */
	public void charge(int requestN, int amount) throws PropertyException {
		
		//Checking if the request exists
		if (!this.requests.containsKey(requestN))
			throw new PropertyException();
		
		//Checking if the request is in state ASSIGNED
		if (!this.requests.get(requestN).isAssigned())
			throw new PropertyException();
		
		//Checking if the amount is a valid value
		if (amount < 0 || amount > 1000)
			throw new PropertyException();
		
		//Set the cost for the request in the request
		this.requests.get(requestN).setCost(amount);
		
		//Set the request as completed
		this.requests.get(requestN).setCompleted();
		
		//Add the cost of the request to the total amount of money that the owner has to pay
		this.requests.get(requestN).getRequirent().addAmount(amount);
		
		//Add to the building in which the request takes place, the name of the worker and its cost
		this.buildings.get(this.requests.get(requestN).getApartment().getBuilding().getBuildingID())
					.addWorkAndCost(this.requests.get(requestN).getWorkerID(), amount);
		
		
		
	}

	/**
	 * Returns the list of request id that have been completed
	 * 
	 * @param void
	 * @return the list of the requested id
	 */
	public List<Integer> getCompletedRequests() {
		
		return this.requests.values()
				.stream()
				.filter(r -> r.isCompleted())
				.map(r -> r.getId())
				.collect(Collectors.toList());
	}
	
	/**
	 * Returns a map ( owner => total expenses )
	 * 
	 * @param void
	 * @return the map sorted by the owner and its total amount that has to pay
	 */
	public SortedMap<String, Integer> getCharges() {
		
		SortedMap<String, Integer> toBeReturned = new TreeMap<String, Integer>();
		
//		this.requests.values().stream()	
//						.filter(r -> r.isCompleted())
//						.forEach(r -> {
//							
//							String ownerID = r.getRequirent().getOwnerID();
//							if (toBeReturned.containsKey(ownerID))
//								toBeReturned.replace(ownerID, toBeReturned.get(ownerID) + r.getCost());
//							else
//								toBeReturned.put(ownerID, r.getCost());
//								
//						});
		
		this.owners.values().stream()
						//Selecting only the owner with something to pay
						.filter(o -> o.getAmountToPayForRequests() != 0)	
						
						//For each of them, adding its ID to the map and the toal amount of money to be paid
						.forEach(o -> {
							toBeReturned.put(o.getOwnerID(), o.getAmountToPayForRequests());
						});
		
		//Return the map
		return toBeReturned; 
	}

	/**
	 * Returns the map ( building => ( profession => total expenses) ).
	 * Both buildings and professions are sorted alphabetically
	 * 
	 * @param void
	 * @return the map sorted by building id with the WorkAndCost map as values
	 */
	public SortedMap<String, Map<String, Integer>> getChargesOfBuildings() {
		
		SortedMap<String, Map<String, Integer>> toBeReturned = new TreeMap<String, Map<String,Integer>>();
		
		this.buildings.values().stream()
		
						//Selecting only the building hosting a request
						.filter(b -> b.getWorkAndCost().size() != 0)
						
						//For each of this building, adding its ID and the map WorkAdCost
						.forEach(b -> {
							toBeReturned.put(b.getBuildingID(), b.getWorkAndCost());
						});
		
		//Returning the map
		return toBeReturned;
	}

}
