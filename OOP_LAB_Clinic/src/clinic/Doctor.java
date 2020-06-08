package clinic;

public class Doctor extends Person {
	
	/*
	 * ATTRIBUTES
	 */
	private int docID;
	private String specialization;

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
	
	
	
	

}
