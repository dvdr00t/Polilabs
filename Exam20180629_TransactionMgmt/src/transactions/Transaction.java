package transactions;

public class Transaction {
	
	/*
	 * ATTRIBUTES
	 */
	private String transactionID;
	private Carrier carrier;
	private Request request;
	private Offer offer;
	private String productID;
	private Integer score;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param transactionID
	 * @param carrier
	 * @param request
	 * @param offer
	 */
	public Transaction(String transactionID, Carrier carrier, Request request, Offer offer) {
		this.transactionID = transactionID;
		this.carrier = carrier;
		this.request = request;
		this.offer = offer;
		this.score = 0;
		this.productID = request.getProductId();
	}

	/*
	 * GETTERS
	 */
	public String getTransactionID() {
		return transactionID;
	}
	public Carrier getCarrier() {
		return carrier;
	}
	public Request getRequest() {
		return request;
	}
	public Offer getOffer() {
		return offer;
	}
	public Integer getScore() {
		return score;
	}
	public void setScore(Integer score) {
		this.score = score;
	}
	public String getProductID() {
		return productID;
	}
	
	
	
	
	
	
	

}
