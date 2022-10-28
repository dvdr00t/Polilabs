package managingProperties;

public class Request {
	
	/*
	 * ATTRIBUTES
	 */
	private enum State {
		PENDING,
		ASSIGNED,
		COMPLETED
	};
	private State state;
	private int id;
	private String profession;
	private String workerID;
	private Owner requirent;
	private Apartment apartment;
	private int cost;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param state
	 * @param id
	 */
	public Request(int id, Owner requirent, Apartment apartment, String profession) {
		this.state = State.PENDING;
		this.id = id;
		this.requirent = requirent;
		this.apartment = apartment;
		this.profession = profession;
		
		
	}

	/*
	 * GETTERS AND SETTERS
	 */
	public State getState() {
		return state;
	}
	public int getId() {
		return id;
	}
	public Owner getRequirent() {
		return requirent;
	}
	public Apartment getApartment() {
		return apartment;
	}
	public String getWorkerID() {
		return workerID;
	}
	public void setWorkerID(String workerID) {
		this.workerID = workerID;
	}
	public String getProfession() {
		return profession;
	}
	public int getCost() {
		return cost;
	}
	public void setCost(int cost) {
		this.cost = cost;
	}

	/*
	 * GETTERS AND SETTERS FOR THE STATE
	 */
	public boolean isPending() {
		if (this.state == State.PENDING)
			return true;
		return false;
	}
	public boolean isCompleted() {
		if (this.state == State.COMPLETED)
			return true;
		return false;
	}
	public boolean isAssigned() {
		if (this.state == State.ASSIGNED)
			return true;
		return false;
	}
	public void setAssigned() {
		this.state = State.ASSIGNED;
	}
	public void setCompleted() {
		this.state = State.COMPLETED;
	}
	
	

}
