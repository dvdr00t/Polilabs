package warehouse;

public class Order {
	
	/*
	 * ATTRIBUTES
	 */
	private Product product;
	private Supplier supplier;
	private int quantity;
	private String code;
	private boolean delivered;
	
	/**
	 * CONSTUCTOR
	 * 
	 * @param product
	 * @param supplier
	 * @param quantity
	 * @param code
	 */
	public Order(Product product, Supplier supplier, int quantity, String code) {
		this.product = product;
		this.supplier = supplier;
		this.quantity = quantity;
		this.code = code;
		this.delivered = false;
		
		//Adding the order to the list of orders for this product
		product.addOrder(this);
	}

	/**
	 * Get the code of the order
	 * 
	 * @return the code
	 */
	public String getCode(){
		return this.code;
	}
	
	/**
	 * Get the quantity of the order
	 * 
	 * @return the quantity
	 */
	public int getQuantity() {
		return this.quantity;
	}
	
	/**
	 * Get the product
	 * 
	 * @return
	 */
	public Product getProduct() {
		return this.product;
	}
	
	public Supplier getSupplier() {
		return this.supplier;
	}
	
	/**
	 * Checks if the product has been delivered yet
	 * 
	 * @return
	 */
	public boolean delivered(){
		return this.delivered;
	}

	public void setDelivered() throws MultipleDelivery {
		
		//Checking if the order is pending
		if (this.delivered)
			throw new MultipleDelivery();
		
		//Setting the new product quantity in the warehouse
		this.product.setQuantity(this.product.getQuantity() + this.quantity);
		
		//Removing the order from the list of order for the product
		this.product.removeOrder(this);
		
		this.delivered = true;
	}

	/**
	 * Convert this order into a string
	 * 
	 * @return String
	 */
	public String toString(){
		return "Order" + this.code + " for " + this.quantity + " of " + this.product.getCode() 
				+ " : " + this.product.getDescription() + " from " + this.supplier.getNome();
	}
}
