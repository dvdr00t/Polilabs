package warehouse;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class Supplier {
	
	/*
	 * ATTRIBUTES
	 */
	private String supplierCode;
	private String supplierName;
	private Map<String, Product> supplies;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param supplierCode
	 * @param supplierName
	 */
	public Supplier(String supplierCode, String supplierName) {
		this.supplierCode = supplierCode;
		this.supplierName = supplierName;
		this.supplies = new LinkedHashMap<String, Product>();
	}
	
	/*
	 * GETTERS
	 */
	public String getCodice(){
		return this.supplierCode;
	}
	public String getNome(){
		return this.supplierName;
	}
	
	/**
	 * Add a new product to the list of product supplied by this
	 * supplier
	 * 
	 * @param product
	 */
	public void newSupply(Product product){
		this.supplies.put(product.getCode(), product);
		product.addSupplier(this);
	}
	
	/**
	 * Retrieve all the products supplied by this supplier
	 * 
	 * @return
	 */
	public List<Product> supplies(){
		return new LinkedList<Product>(this.supplies.values());
	}
}
