package clinic;

public class Doctor extends Person{
	
	/*
	 * ATTRIBUTES
	 */
	private int docID;
	private String specialization;
	private Integer numberOfPatient;

	/**
	 * CONSTRUCTOR
	 * 
	 * @param firstName
	 * @param lastName
	 * @param SSN
	 * @param docID
	 * @param specialization
	 */
	public Doctor(String firstName, String lastName, String SSN, int docID, String specialization) {
		super(firstName, lastName, SSN);
		this.docID = docID;
		this.specialization = specialization;
		this.numberOfPatient = 0;
	}

	
	/*
	 * GETTERS
	 */
	public int getDocID() {
		return docID;
	}
	public String getSpecialization() {
		return specialization;
	}
	public Integer getNumberOfPatient() {
		return this.numberOfPatient;
	}
	
	/*
	 * METHODS
	 */
	
	/**
	 * Increment the number of patients assigned to this doctors of a value number
	 * 
	 * @param value value of the increment
	 */
	public void incrementNumberOfPatient(Integer value) {
		this.numberOfPatient = this.numberOfPatient + value;
	}
	
	/**
	 * Decrement the number of patients assigned to this doctors of a value number
	 * 
	 * @param value value of the decrement
	 */
	public void decrementNumberOfPatient(Integer value) {
		this.numberOfPatient = this.numberOfPatient - value;
	}

	
	

}
