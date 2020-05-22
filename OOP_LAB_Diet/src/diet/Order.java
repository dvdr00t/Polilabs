package diet;

import java.util.Map;
import java.util.TreeMap;

/**
 * Represents an order in the take-away system
 */
public class Order implements Comparable<Order>{
	
	/*
	 * Attributes
	 */
	private User orderingUser;
	private Restaurant orderingRestaurant;
	private String delivererHour;
	
	private Map<Menu, Integer> contents;
	private OrderStatus status;
	private PaymentMethod method;
	
	
	/**
	 * Constructor for the order
	 * 
	 * @param user			User that made the order
	 * @param restaurant	Restaurant to which the order is call
	 * @param deliveringTime String containing the delivering time
	 */
	public Order(User user, Restaurant restaurant, String deliveringTime) {
		this.orderingUser = user;
		this.orderingRestaurant = restaurant;
		this.delivererHour = deliveringTime;
		
		this.contents = new TreeMap<>();
		this.status = OrderStatus.ORDERED;
		this.method = PaymentMethod.CASH;
	}
 
	/**
	 * Defines the possible order status
	 */
	public enum OrderStatus {
		ORDERED, READY, DELIVERED;
	}
	/**
	 * Defines the possible valid payment methods
	 */
	public enum PaymentMethod {
		PAID, CASH, CARD;
	}
		
	/**
	 * Total order price
	 * @return order price
	 */
	public double Price() {
		return -1.0;
	}
	
	/**
	 * define payment method
	 * 
	 * @param method payment method
	 */
	public void setPaymentMethod(PaymentMethod method) {
		this.method = method;
	}
	
	/**
	 * get payment method
	 * 
	 * @return payment method
	 */
	public PaymentMethod getPaymentMethod() {
		return this.method;
	}
	
	/**
	 * change order status
	 * @param newStatus order status
	 */
	public void setStatus(OrderStatus newStatus) {
		this.status = newStatus;
	}
	
	/**
	 * get current order status
	 * @return order status
	 */
	public OrderStatus getStatus(){
		return this.status;
	}
	
	/**
	 * Add a new menu with the relative order to the order.
	 * The menu must be defined in the {@link Food} object
	 * associated the restaurant that created the order.
	 * 
	 * @param menu     name of the menu
	 * @param quantity quantity of the menu
	 * @return this order to enable method chaining
	 */
	public Order addMenus(String menu, int quantity) {
		
		//Retrieving the menu
		Menu toBeAdded = this.orderingRestaurant.getMenu(menu);
		
		//Adding the menu
		if (toBeAdded != null)
			this.contents.put(toBeAdded, (Integer) quantity);

		return this;
	}
	
	/**
	 * Converts to a string as:
	 * <pre>
	 * RESTAURANT_NAME, USER_FIRST_NAME USER_LAST_NAME : DELIVERY(HH:MM):
	 * 	MENU_NAME_1->MENU_QUANTITY_1
	 * 	...
	 * 	MENU_NAME_k->MENU_QUANTITY_k
	 * </pre>
	 */
	@Override
	public String toString() {
		
		String toBeReturned = this.orderingRestaurant.getName() + ", "
				+ this.orderingUser.getFirstName() + " " + this.orderingUser.getLastName() + " : "
				+ "(" + this.delivererHour + "):\n";
		
		for (Menu m: this.contents.keySet())
			toBeReturned = toBeReturned + "\t" + m.getName() + "->" + this.contents.get(m) + "\n";
	
		return toBeReturned;
	}

	@Override
	public int compareTo(Order o) {
		
		//If the restaurant names are equal:
		if (this.orderingRestaurant.getName().equals(o.orderingRestaurant.getName())) {
			
			//If the user first names are equal:
			if (this.orderingUser.getFirstName().equals(o.orderingUser.getFirstName())) {
				
				//If the user last names are equal:
				if (this.orderingUser.getLastName().equals(o.orderingUser.getLastName())) {
					
					//Comparing hours
					return this.delivererHour.compareTo(o.delivererHour);
				}
				
				//Comparing last names
				return this.orderingUser.getLastName().compareTo(o.orderingUser.getLastName());
			}
			
			//Comparing first names
			return this.orderingUser.getFirstName().compareTo(o.orderingUser.getFirstName());
		}
		
		//Comparing restaurant names
		return this.orderingRestaurant.getName().compareTo(o.orderingRestaurant.getName());
	}
	
}
