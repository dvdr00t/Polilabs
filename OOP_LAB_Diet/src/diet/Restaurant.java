package diet;
import java.util.HashMap;
import java.util.Map;

import diet.Order.OrderStatus;

/**
 * Represents a restaurant in the take-away system
 *
 */
public class Restaurant {
	
	/**
	 * ADT used to define an array of schedules window.
	 * Each position of the array contains two string: one is the opening hour
	 * and one is the closing hour of the restaurant.
	 */
	private class ScheduleWindow {
		private String openingHour;
		private String closingHour;
		
		public ScheduleWindow (String openingHour, String closingHour) {
			this.openingHour = openingHour;
			this.closingHour = closingHour;
		}
		public String getOpeningHour() {
			return openingHour;
		}
		public String getClosingHour() {
			return closingHour;
		}
	}
	
	/*
	 * Attributes
	 */
	private String nameRestaurant;
	private Food foodRestaurant;
	private ScheduleWindow[] schedule;
	

	
	/**
	 * Constructor for a new restaurant.
	 * 
	 * Materials and recipes are taken from
	 * the food object provided as argument.
	 * 
	 * @param name	unique name for the restaurant
	 * @param food	reference food object
	 */
	public Restaurant(String name, Food food) {
		this.nameRestaurant = name;
		this.foodRestaurant = food;
	}
	
	/**
	 * gets the name of the restaurant
	 * 
	 * @return name
	 */
	public String getName() {
		return this.nameRestaurant;
	}
	
	/**
	 * Define opening hours.
	 * 
	 * The opening hours are considered in pairs.
	 * Each pair has the initial time and the final time
	 * of opening intervals.
	 * 
	 * for a restaurant opened from 8:15 until 14:00 and from 19:00 until 00:00, 
	 * is thoud be called as {@code setHours("08:15", "14:00", "19:00", "00:00")}.
	 * 
	 * @param hm a list of opening hours
	 */
	public void setHours(String ... hm) {
		
		//Creating the schedule windows
		this.schedule = new ScheduleWindow[hm.length/2];
		
		//Adding the schedule hours
		int firstIndex = 0;
		int secondIndex = 1;
		for (int count = 0; count < hm.length/2; count++) {
			this.schedule[count] = new ScheduleWindow(hm[firstIndex], hm[secondIndex]);
			
			firstIndex += 2;
			secondIndex += 2;
		}
	}
	
	public Menu getMenu(String name) {
		return null;
	}
	
	/**
	 * Creates a new menu
	 * 
	 * @param name name of the menu
	 * 
	 * @return the newly created menu
	 */
	public Menu createMenu(String name) {
		
		//Creating the menu object
		Menu menu = new Menu(name, this.foodRestaurant.collection);
		
		return menu;
	}

	/**
	 * Find all orders for this restaurant with 
	 * the given status.
	 * 
	 * The output is a string formatted as:
	 * <pre>
	 * Napoli, Judi Dench : (19:00):
	 * 	M6->1
	 * Napoli, Ralph Fiennes : (19:00):
	 * 	M1->2
	 * 	M6->1
	 * </pre>
	 * 
	 * The orders are sorted by name of restaurant, name of the user, and delivery time.
	 * 
	 * @param status the status of the searched orders
	 * 
	 * @return the description of orders satisfying the criterion
	 */
	public String ordersWithStatus(OrderStatus status) {
		return null;
	}
}
