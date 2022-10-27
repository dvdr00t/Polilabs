package clinic;

public class Patient extends Person {
	
	/*
	 * ATTRIBUTE
	 */
	private Doctor assignedDoctor;

	/**
	 * CONSTRUCTOR
	 * 
	 * @param firstName
	 * @param lastName
	 * @param SSN
	 */
	public Patient(String firstName, String lastName, String SSN) {
		super(firstName, lastName, SSN);
	}
	
	/*
	 * METHODS
	 */
	
	/**
	 * Assign a doctor to this patient
	 * 
	 * @param doctor doctor to be assigned
	 */
	public void assignDoctor(Doctor doctor) {
		this.assignedDoctor = doctor;
	}
	
	/**
	 * Get the assigned doctor to this patient
	 * 
	 * @return void
	 */
	public Doctor getAssignedDoctor() {
		return this.assignedDoctor;
	}
	
	
}
