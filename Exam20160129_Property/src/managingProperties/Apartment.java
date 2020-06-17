package managingProperties;

public class Apartment {
	
	/*
	 * ATTRIBUTES
	 */
	private Building building;
	private int apartmentID;
	private boolean occupied;
	private Owner owner;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param building
	 * @param apartmentsID
	 */
	public Apartment(Building building, int apartmentsID) {
		this.building = building;
		this.apartmentID = apartmentsID;
		this.occupied = false;
		this.owner = null;
	}

	/*
	 * GETTERS
	 */
	public Building getBuilding() {
		return building;
	}
	public int getApartmentsID() {
		return apartmentID;
	}
	public boolean isOccupied() {
		return occupied;
	}
	public Owner getOwner() {
		return owner;
	}
	public void setOwner(Owner owner) {
		this.owner = owner;
		this.occupied = true;
	}
	
	
	
	

}
