package warehouse;

import java.util.Collection;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Warehouse {
	
	/*
	 * ATTRIBUTES
	 */
	private Map<String, Product> products;
	private Map<String, Supplier> suppliers;
	private Map<String, Order> orders;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param void
	 */
	public Warehouse () {
		this.products = new LinkedHashMap<String, Product>();
		this.suppliers = new LinkedHashMap<String, Supplier>();
		this.orders = new LinkedHashMap<String, Order>();
	}
	
	/**
	 * Add a new product to this warehouse
	 * 
	 * @param code
	 * @param description
	 * @return the new product add
	 */
	public Product newProduct(String code, String description){
		Product newProduct = new Product(code, description);
		this.products.put(code, newProduct);
		return newProduct;
	}
	
	/**
	 * Retrieve all products
	 * 
	 * @return the collection of all the products
	 */
	public Collection<Product> products(){
		return new LinkedList<Product>(this.products.values());
	}

	/**
	 * Retrieve the product associated with the code given
	 * 
	 * @param code
	 * @return the product
	 */
	public Product findProduct(String code){
		if (this.products.containsKey(code))
			return this.products.get(code);
		return null;
	}
	
	/**
	 * Add a new supplier to the warehouse 
	 * 
	 * @param code
	 * @param name
	 * @return the new supplier add
	 */
	public Supplier newSupplier(String code, String name){
		Supplier newSupplier = new Supplier(code, name);
		this.suppliers.put(code, newSupplier);
		return newSupplier;
	}
	
	/**
	 * Retrieve the supplier associated with the code given
	 * 
	 * @param code
	 * @return the supplier
	 */
	public Supplier findSupplier(String code){
		if (this.suppliers.containsKey(code))
			return this.suppliers.get(code);
		return null;
	}

	/**
	 * Generate a new order given the supplier, the product and the amount of product
	 * ordered. It automatically generates a code in the form ORDn.
	 * 
	 * @param prod
	 * @param quantity
	 * @param supp
	 * @return
	 * @throws InvalidSupplier
	 */
	public Order issueOrder(Product prod, int quantity, Supplier supp) throws InvalidSupplier {
		
		//Checking if the supplier supply the product
		if (!this.suppliers.get(supp.getCodice()).supplies().contains(prod))
			throw new InvalidSupplier();
		
		//Generating the code
		int n = this.orders.size() + 1;
		String code = "ORD" + n;
		
		//Adding the order
		Order newOrder = new Order(prod, supp, quantity, code);
		this.orders.put(code, newOrder);
		return newOrder;
	}

	/**
	 * Retrieve the specific order by the code given
	 * 
	 * @param code
	 * @return the order
	 */
	public Order findOrder(String code){
		if (this.orders.containsKey(code))
			return this.orders.get(code);
		return null;
	}
	
	/**
	 * Retrieve all the pending orders in alphabetical order
	 * 
	 * @return the list of pending orders
	 */
	public List<Order> pendingOrders(){
		return this.orders.values().stream()
				.filter(o -> !o.delivered())
				.sorted(new Comparator<Order>() {

					@Override
					public int compare(Order o1, Order o2) {
						return o1.getProduct().getCode().compareTo(o2.getProduct().getCode());
					}
					
				})
				.collect(Collectors.toList());
	}

	/**
	 * Retrieve all the order grouped by the product code
	 * 
	 * @return
	 */
	public Map<String,List<Order>> ordersByProduct(){
	    return this.orders.values()
	    		.stream()
	    		.collect(Collectors.groupingBy(o -> o.getProduct().getCode()));
	}
	
	/**
	 * Retrieve all the order by supplier
	 * @return
	 */
	public Map<String,Long> orderNBySupplier(){
	    return this.orders.values()
	    		.stream()
	    		.filter(o -> o.delivered())
	    		.collect(Collectors.groupingBy(o -> o.getSupplier().getNome(),
	    										TreeMap::new,
	    										Collectors.counting()));
	}
	
	/**
	 * Retrieve the list of product with the delivered order value
	 * 
	 * @return
	 */
	public List<String> countDeliveredByProduct(){
		List<String> toBeReturned = new LinkedList<String>();
	    this.products.values()
		    .stream()
		    .sorted(new Comparator<Product>() {

				@Override
				public int compare(Product o1, Product o2) {
					if (o1.getDeliveredOrders() <= o2.getDeliveredOrders())
						return 1;
					else
						return -1;
				}
		    	
		    })
		    .forEach(p -> {
		    	toBeReturned.add(p.getCode() + " - " + p.getDeliveredOrders());
		    });
	    
	    return toBeReturned;
	}
}
