package transactions;

public class Offer {
	
	/*
	 * ATTRIBUTES
	 */
	private String offerId;
	private String placeName;
	private String productId;
	private enum State {
		PENDING,
		BOUNDED
	};
	private State state;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param requestId
	 * @param placeName
	 * @param productId
	 */
	public Offer(String offerId, String placeName, String productId) {
		this.offerId = offerId;
		this.placeName = placeName;
		this.productId = productId;
	}

	/*
	 * GETTERS
	 */
	public String getOfferId() {
		return offerId;
	}
	public String getPlaceName() {
		return placeName;
	}
	public String getProductId() {
		return productId;
	}
	
	public void setBounded() {
		this.state = State.BOUNDED;
	}
	public boolean isBounded() {
		if (this.state == State.BOUNDED)
			return true;
		return false;
	}
}
