package managingProperties;

import java.util.LinkedList;
import java.util.List;

public class Owner {
	
	/*
	 * ATTRIBUTES
	 */
	private String ownerID;
	private List<String> apartments;
	private int amountToPayForRequests;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param ownerID
	 * @param apartments
	 */
	public Owner(String ownerID, String[] apartments) {
		this.ownerID = ownerID;
		this.amountToPayForRequests = 0;
		
		//Creating the list with all the apartments for this owner
		this.apartments = new LinkedList<String>();
	
		//Adding all the apartments
		for (String s: apartments)
			this.apartments.add(s);
	}

	/*
	 * GETTER
	 */
	public String getOwnerID() {
		return ownerID;
	}
	
	/**
	 * Get the total amount of money this owner has to pay for the requests
	 * 
	 * @return Integer
	 */
	public int getAmountToPayForRequests() {
		return amountToPayForRequests;
	}
	
	/**
	 * Add a new amount to the total amount of money this owner has to pay for the requests
	 * 
	 * @param amount
	 */
	public void addAmount(int amount) {
		this.amountToPayForRequests = this.amountToPayForRequests + amount;
	}
	
	
}
