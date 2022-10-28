package clinic;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

public class Doctor extends Person{
	
	/*
	 * ATTRIBUTES
	 */
	private int docID;
	private String specialization;
	private Integer numberOfPatient;
	private List<Patient> assignedPatient;

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
		this.assignedPatient = new LinkedList<Patient>();
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
	private void incrementNumberOfPatient(Integer value) {
		this.numberOfPatient = this.numberOfPatient + value;
	}
	
	/**
	 * Decrement the number of patients assigned to this doctors of a value number
	 * 
	 * @param value value of the decrement
	 */
	private void decrementNumberOfPatient(Integer value) {
		this.numberOfPatient = this.numberOfPatient - value;
	}
	
	/**
	 * Add a new patient to the list of patient assigned to this doctor
	 * 
	 * @param p patient to be assigned
	 */
	public void addPatient(Patient p) {
		this.assignedPatient.add(p);
		incrementNumberOfPatient(1);
	}
	
	/**
	 * Get a particular patient from the list of patients assigned to this doctor
	 * 
	 * @param SSN of the patient
	 * @return Patient
	 * @throws NoSuchPatient
	 */
	public Patient getPatient(String SSN) throws NoSuchPatient {
		return this.assignedPatient.stream()
				.filter(x -> (x.getSSN().equals(SSN)))
				.findFirst()
				.orElseThrow(() -> new NoSuchPatient());
	}
	
	/**
	 * Get all the patients assigned to this doctor
	 * 
	 * @return a collection of patients
	 */
	public Collection<Patient> getAllPatients() {
		Collection<Patient> toBeReturned = new LinkedList<Patient>(this.assignedPatient);
		return toBeReturned;
	}
	
	
	
	

}
