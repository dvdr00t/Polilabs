package warehouse;

import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Product {
	
	/*
	 * ATTRIBUTES
	 */
	private String productCode;
	private String productDescription;
	private Integer quantity;
	private Map<String, Supplier> productSuppliers;
	private Map<String, Order> pendingOrders;
	private int deliveredOrders;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param productCode
	 * @param productDescription
	 */
	public Product(String productCode, String productDescription) {
		this.productCode = productCode;
		this.productDescription = productDescription;
		
		this.quantity = 0;
		this.deliveredOrders = 0;
		this.productSuppliers = new LinkedHashMap<String, Supplier>();
		this.pendingOrders = new LinkedHashMap<String, Order>();
	}
	/*
	 * GETTERS AND SETTERS
	 */
	public String getCode() {
		return this.productCode;
	}
	public String getDescription() {
		return this.productDescription;
	}
	public int getDeliveredOrders() {
		return this.deliveredOrders;
	}
	
	/**
	 * Set the amount of products in the warehouse for this product
	 * 
	 * @param quantity
	 */
	public void setQuantity(int quantity){
		this.quantity = Integer.valueOf(quantity);
	}

	/**
	 * Decrease by one unit the quantity of this product
	 * 
	 * @return void
	 */
	public void decreaseQuantity(){
		this.quantity--;
	}

	/**
	 * Get the amount of products in the warehouse for this product
	 * 
	 * @return the amount of products
	 */
	public int getQuantity(){
		return this.quantity;
	}

	public void addSupplier(Supplier supplier) {
		this.productSuppliers.put(supplier.getCodice(), supplier);
	}
	
	/**
	 * Retrieve the list of supplier that supply this product
	 * 
	 * @return the list of supplier sorted by name
	 */
	public List<Supplier> suppliers(){
		return this.productSuppliers
				.values()
				.stream()
				.sorted(new Comparator<Supplier>() {

					@Override
					public int compare(Supplier o1, Supplier o2) {
						return o1.getCodice().compareTo(o2.getCodice());
					}
					
				})
				.collect(Collectors.toList());
	}

	/**
	 * Add a new order to the list
	 * 
	 * @param order
	 */
	public void addOrder (Order order) {
		this.pendingOrders.put(order.getCode(), order);
	}
	
	/**
	 * Remove an order from the list
	 * 
	 * @param order
	 */
	public void removeOrder (Order order) {
		this.pendingOrders.remove(order.getCode());
		this.deliveredOrders++;
	}
	
	/**
	 * Retrieve all the pending orders for this product
	 * 
	 * @return the list of pending orders
	 */
	public List<Order> pendingOrders(){
		return this.pendingOrders.values()
				.stream()
				.sorted(new Comparator<Order>() {

					@Override
					public int compare(Order o1, Order o2) {
						if (o1.getQuantity() >= o2.getQuantity())
							return -1;
						else 
							return 1;
					}
	
				})
				.collect(Collectors.toList());
	}
}
