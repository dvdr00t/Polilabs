package transactions;

public class Request {
	
	/*
	 * ATTRIBUTES
	 */
	private String requestId;
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
	public Request(String requestId, String placeName, String productId) {
		this.requestId = requestId;
		this.placeName = placeName;
		this.productId = productId;
	}

	/*
	 * GETTERS
	 */
	public String getRequestId() {
		return requestId;
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
