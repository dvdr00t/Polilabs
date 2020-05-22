package diet;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

/**
 * Represents the main class in the
 * take-away system.
 * 
 * It allows adding restaurant, users, and creating orders.
 *
 */
public class Takeaway {
	
	/*
	 * Attributes
	 */
	private List<Restaurant> listRestaurants;
	private List<User> listUsers;
	
	/**
	 * Constructor of the class
	 * 
	 * @param void
	 */
	public Takeaway () {
		this.listRestaurants = new LinkedList<Restaurant>();
		this.listUsers = new LinkedList<User>();
	}

	/**
	 * Adds a new restaurant to the take-away system
	 * 
	 * @param r the restaurant to be added
	 */
	public void addRestaurant(Restaurant r) {
		
		//Checking if the Restaurant already exists
		if (this.listRestaurants.contains(r))
			return;
		
		//Adding the new Restaurant
		this.listRestaurants.add(r);
	}
	
	/**
	 * Returns the collections of restaurants
	 * 
	 * @return collection of added restaurants
	 */
	public Collection<String> restaurants() {
		
		//Creating a new array of users to sort it
		Restaurant[] toBeSorted = new Restaurant[this.listRestaurants.size()];
		this.listRestaurants.toArray(toBeSorted);
		
		//Sorting array;
		Arrays.sort(toBeSorted);
		
		//Creating the collection to be returned to the user
		Collection<String> toBeReturned = new LinkedList<String>();
		
		//Adding elements to the collection
		for (Restaurant u: toBeSorted)
			toBeReturned.add(u.getName());
		
		return toBeReturned;
	}
	
	/**
	 * Define a new user
	 * 
	 * @param firstName first name of the user
	 * @param lastName  last name of the user
	 * @param email     email
	 * @param phoneNumber telephone number
	 * @return
	 */
	public User registerUser(String firstName, String lastName, String email, String phoneNumber) {
		
		//Creating the new user to be returned
		User newUser = new User(firstName, lastName, email, phoneNumber);
		
		//Adding the user to the list of users
		this.listUsers.add(newUser);
		
		return newUser;
	}
	
	/**
	 * Gets the collection of registered users
	 * 
	 * @return the collection of users
	 */
	public Collection<User> users(){
		
		//Creating a new array of users to sort it
		User[] toBeSorted = new User[this.listUsers.size()];
		this.listUsers.toArray(toBeSorted);
		
		//Sorting array;
		Arrays.sort(toBeSorted);
		
		//Creating the collection to be returnd to the user
		Collection<User> toBeReturned = new LinkedList<User>();
		
		//Adding elements to the collection
		for (User u: toBeSorted)
			toBeReturned.add(u);
		
		return toBeReturned;
	}
	
	/**
	 * Create a new order by a user to a given restaurant.
	 * 
	 * The order is initially empty and is characterized
	 * by a desired delivery time. 
	 * 
	 * @param user				user object
	 * @param restaurantName	restaurant name
	 * @param h					delivery time hour
	 * @param m					delivery time minutes
	 * @return
	 */
	public Order createOrder(User user, String restaurantName, int h, int m) {
		
		//Searching the restaurant in the list
		int index = 0;
		for (Restaurant r: this.listRestaurants) {
			if (r.getName().equals(restaurantName))
				break;
			index++;
		}
		
		
		//Checking the existence of the restaurant
		if (index == this.listRestaurants.size()) 
			return null;
		Restaurant restaurant = this.listRestaurants.get(index);
		
		//Getting the right delivering time
		String deliveringTime;
		
		//Fixing hours and minutes
		if (h < 10)
			deliveringTime = "0" + h + ":";
		else
			deliveringTime = h + ":";
		
		if (m < 10)
			deliveringTime = deliveringTime + "0" + m;
		else
			deliveringTime = deliveringTime + m;
			
		//Calling the method that retrieve the right delivering time
		String deliveringHour = restaurant.getOrderScheduleTime(deliveringTime);
		
		//Creating the new order
		Order order = new Order(user, restaurant, deliveringHour);
		
		//Adding the new order to the restaurant
		restaurant.addOrder(order);
	
		return order;
	}
	
	/**
	 * Retrieves the collection of restaurant that are open
	 * at the given time.
	 * 
	 * @param time time to check open
	 * 
	 * @return collection of restaurants
	 */
	public Collection<Restaurant> openedRestaurants(String time){
		
		//Creating the new collection to be returned
		Collection<Restaurant> toBeReturned = new TreeSet<Restaurant>();
		
		//Adding the valid elements
		for (Restaurant r: this.listRestaurants)
			if (r.itsOpened(time))
				toBeReturned.add(r);
		
		return toBeReturned;
		
	}

	
	
}
