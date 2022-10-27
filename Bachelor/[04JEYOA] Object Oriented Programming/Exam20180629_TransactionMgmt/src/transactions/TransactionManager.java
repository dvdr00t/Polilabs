package transactions;
import java.util.*;
import java.util.stream.Collectors;

//import static java.util.stream.Collectors.*;
//import static java.util.Comparator.*;

public class TransactionManager {
	
	/*
	 * ATTRIBUTES
	 */
	private Map<String, Region> regions;
	private List<String> places;
	private Map<String, Carrier> carriers;
	private Map<String, Request> requests;
	private Map<String, Offer> offers;
	private Map<String, Transaction> transactions;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param void
	 */
	public TransactionManager() {
		this.regions = new LinkedHashMap<String, Region>();
		this.carriers = new LinkedHashMap<String, Carrier>();
		this.requests = new LinkedHashMap<String, Request>();
		this.offers = new LinkedHashMap<String, Offer>();
		this.transactions = new LinkedHashMap<String, Transaction>();
		this.places = new LinkedList<String>();
	}
	
//R1
	
	/**
	 * Add a new region to the list of regions
	 * 
	 * @param regionName
	 * @param placeNames
	 * @return the list of places added for this region (ordered)
	 */
	public List<String> addRegion(String regionName, String... placeNames) { 
		
		//Creating the region
		Region r = new Region(regionName);
		for (String s: placeNames)
			if (!this.places.contains(s)) {
				this.places.add(s);
				r.addPlace(s);
			}
		
		//Adding the region to the list
		this.regions.put(regionName, r);		
		
		//Return statement
		return r.getPlaces()
				.stream()
				.sorted()
				.collect(Collectors.toList());
	}
	
	/**
	 * Add a new carrier to the list of carriers
	 * 
	 * @param carrierName
	 * @param regionNames
	 * @return the list of regions supplied (ordered)
	 */
	public List<String> addCarrier(String carrierName, String... regionNames) { 
		
		//Creating the carrier
		Carrier c = new Carrier(carrierName);
		for (String s: regionNames)
			if (this.regions.containsKey(s))
				c.addRegion(this.regions.get(s));
		
		//Adding the carrier to the list
		this.carriers.put(carrierName, c);
		
		return c.getRegionsSupplied();
	}
	
	/**
	 * Get the ordered list of the names of carriers supplying 
	 * the region with the given name.
	 * 
	 * @param regionName
	 * @return an ordered list
	 */
	public List<String> getCarriersForRegion(String regionName) { 
		if (this.regions.containsKey(regionName))
			return this.regions.get(regionName).getCarriers();
		return null;
	}
	
//R2
	
	/**
	 * Create a new request. The method throws an exception if the 
	 * place is undefined or if the code is duplicated.
	 * 
	 * @param requestId
	 * @param placeName
	 * @param productId
	 * @throws TMException
	 */
	public void addRequest(String requestId, String placeName, String productId) throws TMException {
		
		//Checking if the code is duplicated
		if (this.requests.containsKey(requestId))
			throw new TMException();
		
		//Checking if the place is undefined
		boolean found = false;
		for (Region r: this.regions.values())
			if (r.getPlaces().contains(placeName))
				found = true;
		if (!found)
			throw new TMException();
		
		//Create the new request and add to the list
		this.requests.put(requestId, new Request(requestId, placeName, productId));
		
	}
	
	/**
	 * Create a new offer. The method throws an exception if the 
	 * place is undefined or if the code is duplicated.
	 * 
	 * @param offerId
	 * @param placeName
	 * @param productId
	 * @throws TMException
	 */
	public void addOffer(String offerId, String placeName, String productId) throws TMException {
		
		//Checking if the code is duplicated
		if (this.offers.containsKey(offerId))
			throw new TMException();
				
		//Checking if the place is undefined
		boolean found = false;
		for (Region r: this.regions.values())
			if (r.getPlaces().contains(placeName))
				found = true;
		if (!found)
		throw new TMException();
				
		//Create the new request and add to the list
		this.offers.put(offerId, new Offer(offerId, placeName, productId));
		
	}
	

//R3
	
	/**
	 * Defines a transaction given its code, the carrier's name, and 
	 * the request's and offer's id.
	 * Request and offer should not have been bound to previous transactions, 
	 * they should relate to the same product id and the carrier should serve both 
	 * the delivery and pickup places (i.e. the corresponding regions). 
	 * If any of the previous checks should fail, an exception must be raised, 
	 * otherwise the transaction must be associated to both the request and the offer.
	 * 
	 * @param transactionId
	 * @param carrierName
	 * @param requestId
	 * @param offerId
	 * @throws TMException
	 */
	public void addTransaction(String transactionId, String carrierName, String requestId, String offerId) throws TMException {
		
		//Checking if the request is bounded
		if (this.requests.get(requestId).isBounded())
			throw new TMException();
		
		//Checking if the offer is bounded
		if (this.offers.get(offerId).isBounded())
			throw new TMException();
		
		//Checking if request and offers relate to the same product id
		if (this.requests.get(requestId).getProductId().compareTo(this.offers.get(offerId).getProductId()) != 0)
			throw new TMException();
		
		//Checking if the carrier serve both the delivery and pickup places
		String deliveryPlace = this.requests.get(requestId).getPlaceName();
		String pickUpPlace = this.offers.get(offerId).getPlaceName();
		boolean foundDeliveryPlace = false;
		boolean foundPickUpPlace = false;
		for (String s: this.carriers.get(carrierName).getRegionsSupplied()) {
			if (this.regions.get(s).getPlaces().contains(deliveryPlace))
				foundDeliveryPlace = true;
			if (this.regions.get(s).getPlaces().contains(pickUpPlace))
				foundPickUpPlace = true;
		}
		if (!foundDeliveryPlace || !foundPickUpPlace)
			throw new TMException();
			
		
		//Creating and adding the transaction
		this.transactions.put(transactionId,
				new Transaction(transactionId, 
						this.carriers.get(carrierName), 
						this.requests.get(requestId), 
						this.offers.get(offerId)));
		this.requests.get(requestId).setBounded();
		this.offers.get(offerId).setBounded();
			
		
	}
	
	/**
	 * Assigns a score to the transaction.
	 * The returned value is false if the score is not comprised between 1 and 10 
	 * (extremes included); 
	 * otherwise the method returns true.
	 * 
	 * @param transactionId
	 * @param score
	 * @return boolean value
	 */
	public boolean evaluateTransaction(String transactionId, int score) {
		if (score < 1 || score > 10)
			return false;
		
		this.transactions.get(transactionId).setScore(score);
		return true;
	}
	
//R4
	
	/**
	 * Returns the list of names of regions (ordered alphabetically) that are delivery 
	 * locations for the same number of transactions. A region is a location of delivery 
	 * for a transaction if the region includes the place of delivery of the request associated 
	 * to the transaction. The returned map shows the number of transactions in decreasing order.
	 * 
	 * @return
	 */
	public SortedMap<Long, List<String>> deliveryRegionsPerNT() {
		
		SortedMap<String, Long> map = this.transactions.values()
				.stream()
				.map(t -> getRegion(t.getRequest().getPlaceName()).getRegionName())
				.collect(Collectors.groupingBy(r -> r, TreeMap::new, Collectors.counting()));
		
		SortedMap<Long, List<String>> toBeReturned = map.entrySet()
				.stream()
				.collect(Collectors.groupingBy(e -> e.getValue(), 
						TreeMap::new, 
						Collectors.mapping(e -> e.getKey(), Collectors.toList())));
		
		return toBeReturned;
				
	}
	
	/**
	 * Returns the total score of the transactions related to the same carrier. 
	 * Transaction with a total score below the given minimumScore are ignored. 
	 * Carriers appear in alphabetical order.
	 * 
	 * @param minimumScore
	 * @return
	 */
	public SortedMap<String, Integer> scorePerCarrier(int minimumScore) {
		
		SortedMap<String, Integer> toBeReturned = new TreeMap<String, Integer>();
		
		this.transactions.values()
						.stream()
						.filter(t -> t.getScore() >= minimumScore)
						.forEach(t -> {
							
							String carrierName = t.getCarrier().getCarrierName();
							
							//Checking if the carrier has already been inserted
							if (!toBeReturned.containsKey(carrierName))
								toBeReturned.put(carrierName, t.getScore());
							else
								toBeReturned.replace(carrierName, toBeReturned.get(carrierName) + t.getScore());
						});
				
		return toBeReturned;
	}
	
	/**
	 * Returns the number of transactions (only if greater than 0) 
	 * for product's id, with the latter alphabetically ordered.
	 * 
	 * @return
	 */
	public SortedMap<String, Long> nTPerProduct() {
		
		return this.transactions.values()
				.stream()
				.map(t -> t.getProductID())
				.sorted()
				.collect(Collectors.groupingBy(x -> x, TreeMap::new, Collectors.counting()));
		

	}
	
	private Region getRegion(String place) {
		for (Region r: this.regions.values())
			if (r.getPlaces().contains(place))
				return r;
		return null;
	}
	
	
}

