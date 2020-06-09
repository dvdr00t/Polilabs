package clinic;

import java.io.IOException;
import java.io.Reader;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;
import java.util.stream.Collectors;

/**
 * Represents a clinic with patients and doctors.
 * 
 */
public class Clinic {
	
	/*
	 * ATTRIBUTES
	 */
	private List<Patient> patients;
	private List<Doctor> doctors;
	private Map<Patient, Doctor> assignement;
	
	
	public Clinic () {
		this.patients = new LinkedList<Patient>();
		this.doctors = new LinkedList<Doctor>();
		this.assignement = new LinkedHashMap<Patient, Doctor>();
	}

	/**
	 * Add a new clinic patient.
	 * 
	 * @param first first name of the patient
	 * @param last last name of the patient
	 * @param ssn SSN number of the patient
	 */
	public void addPatient(String first, String last, String ssn) {
		this.patients.add(new Patient(first, last, ssn));
	}


	/**
	 * Retrieves a patient information
	 * 
	 * @param ssn SSN of the patient
	 * @return the object representing the patient
	 * @throws NoSuchPatient in case of no patient with matching SSN
	 */
	public String getPatient(String ssn) throws NoSuchPatient {
		
		Patient p = this.patients.stream()
				.filter(x -> (x.getSSN().equals(ssn)))
				.findFirst()
				.orElseThrow(() -> new NoSuchPatient());
		
		return p.getLastName() + " " + p.getFirstName() + " (" + p.getSSN() + ")";
	}

	/**
	 * Add a new doctor working at the clinic
	 * 
	 * @param first first name of the doctor
	 * @param last last name of the doctor
	 * @param ssn SSN number of the doctor
	 * @param docID unique ID of the doctor
	 * @param specialization doctor's specialization
	 */
	public void addDoctor(String first, String last, String ssn, int docID, String specialization) {
		this.doctors.add(new Doctor(first, last, ssn, docID, specialization));
	}

	/**
	 * Retrieves information about a doctor
	 * 
	 * @param docID ID of the doctor
	 * @return object with information about the doctor
	 * @throws NoSuchDoctor in case no doctor exists with a matching ID
	 */
	public String getDoctor(int docID) throws NoSuchDoctor {
		
		Doctor d = this.doctors.stream()
				.filter(x -> (x.getDocID() == docID))
				.findFirst()
				.orElseThrow(() -> new NoSuchDoctor());
		
		return d.getLastName() + " " + d.getFirstName() + " (" + d.getSSN() + ") (" + d.getDocID() + "): " + d.getSpecialization();
	}
	
	/**
	 * Assign a given doctor to a patient
	 * 
	 * @param ssn SSN of the patient
	 * @param docID ID of the doctor
	 * @throws NoSuchPatient in case of not patient with matching SSN
	 * @throws NoSuchDoctor in case no doctor exists with a matching ID
	 */
	public void assignPatientToDoctor(String ssn, int docID) throws NoSuchPatient, NoSuchDoctor {
		
		//Retrieving patient
		Patient p = this.patients.stream()
				.filter(x -> (x.getSSN().equals(ssn)))
				.findFirst()
				.orElseThrow(() -> new NoSuchPatient());
		
		//Retrieving doctor
		Doctor d = this.doctors.stream()
				.filter(x -> (x.getDocID() == docID))
				.findFirst()
				.orElseThrow(() -> new NoSuchDoctor());
		
		//Assign doctor to patient
		this.assignement.put(p, d);
		
		//Increment the number of patient assigned
		d.incrementNumberOfPatient(1);
		
	}
	
	/**
	 * Retrieves the id of the doctor assigned to a given patient.
	 * 
	 * @param ssn SSN of the patient
	 * @return id of the doctor
	 * @throws NoSuchPatient in case of not patient with matching SSN
	 * @throws NoSuchDoctor in case no doctor has been assigned to the patient
	 */
	public int getAssignedDoctor(String ssn) throws NoSuchPatient, NoSuchDoctor {
		
		//Retrieving patient
		Patient p = this.patients.stream()
				.filter(x -> (x.getSSN().equals(ssn)))
				.findFirst()
				.orElseThrow(() -> new NoSuchPatient());
			
		return this.assignement.get(p).getDocID();
	}
	
	/**
	 * Retrieves the patients assigned to a doctor
	 * 
	 * @param id ID of the doctor
	 * @return collection of patient SSNs
	 * @throws NoSuchDoctor in case the {@code id} does not match any doctor 
	 */
	public Collection<String> getAssignedPatients(int id) throws NoSuchDoctor {
		
		//Retrieving doctor
		Doctor d = this.doctors.stream()
				.filter(x -> (x.getDocID() == id))
				.findFirst()
				.orElseThrow(() -> new NoSuchDoctor());
		
		Collection<String> toBeReturned = new LinkedList<String>();
		this.assignement.forEach((key, value) -> {
			if (value.getDocID() == d.getDocID())
				toBeReturned.add(key.getSSN() + "\n");
		});
		
		
		return toBeReturned;
		
	}


	/**
	 * Loads data about doctors and patients from the given stream.
	 * <p>
	 * The text file is organized by rows, each row contains info about
	 * either a patient or a doctor.</p>
	 * <p>
	 * Rows containing a patient's info begin with letter {@code "P"} followed by first name,
	 * last name, and SSN. Rows containing doctor's info start with letter {@code "M"},
	 * followed by badge ID, first name, last name, SSN, and specialization.<br>
	 * The elements on a line are separated by the {@code ';'} character possibly
	 * surrounded by spaces that should be ignored.</p>
	 * <p>
	 * In case of error in the data present on a given row, the method should be able
	 * to ignore the row and skip to the next one.<br>

	 * 
	 * @param readed linked to the file to be read
	 * @throws IOException in case of IO error
	 */
	public void loadData(Reader reader) throws IOException {
		// TODO Complete method
		
	}


	/**
	 * Retrieves the collection of doctors that have no patient at all.
	 * The doctors are returned sorted in alphabetical order
	 * 
	 * @return the collection of doctors' ids
	 */
	public Collection<Integer> idleDoctors(){
		
		/*
		
		//Creating a copy of the collection containing the doctors
		Collection<Doctor> idleDoctors = new LinkedList<Doctor>(this.doctors);
		
		//Removing from the copy all the doctors that are assigned
		idleDoctors.removeAll(this.assignement.values());
		
		//Getting for each of this doctor, the codID and add it to the collection
		Collection<Integer> toBeReturned = new LinkedList<Integer>();
		idleDoctors.forEach(d -> {
			toBeReturned.add(d.getDocID());
		});
		
		return toBeReturned;
		
		*/
		
		//Creating a copy of the collection containing the doctors
		Collection<Integer> idleDoctors = this.doctors
				.stream()
				.filter(d -> d.getNumberOfPatient() == 0)
				.flatMap(m -> m.getDocID())
				.collect(Collectors.toList());
				
		

	}

	/**
	 * Retrieves the collection of doctors having a number of patients larger than the average.
	 * 
	 * @return  the collection of doctors' ids
	 */
	public Collection<Integer> busyDoctors(){
		
		/*
		 * Given the map of patient associate with their doctors, we want to create a new map 
		 * that has:
		 * 
		 *  KEYS: every doctor
		 *  VALUE: the number of patients he/She has
		 */
		Map<Doctor, Long> bIndex = this.assignement
				.values()
				.stream()
				.collect(Collectors.groupingBy(d -> d, Collectors.counting()));
		
		//Adding the doctors that are not present in the map (they have zero patients assigned) 
		this.doctors.forEach(d -> {
			if (!this.assignement.containsValue(d))
				bIndex.put(d, (long) 0);
		});
		
		//Computing the average value from the new map created (average number of patients for doctors)
		double avg = bIndex.values()
				.stream()
				.mapToDouble(Long::doubleValue)
				.average()
				.orElse(0);
				
		//Retrieving all the doctors that has a number of patients lower than the avg
		Collection<Integer> busyDoctors = new LinkedList<Integer>();
		bIndex.forEach((key, value) -> {
			if (value > avg)
				busyDoctors.add(key.getDocID());
		});
		
		return busyDoctors;
	}

	/**
	 * Retrieves the information about doctors and relative number of assigned patients.
	 * <p>
	 * The method returns list of strings formatted as "{@code ### : ID SURNAME NAME}" where {@code ###}
	 * represent the number of patients (printed on three characters).
	 * <p>
	 * The list is sorted by decreasing number of patients.
	 * 
	 * @return the collection of strings with information about doctors and patients count
	 */
	public Collection<String> doctorsByNumPatients(){
		
		/*
		 * Given the map of patient associate with their doctors, we want to create a new map 
		 * that has:
		 * 
		 *  KEYS: every doctor
		 *  VALUE: the number of patients he/She has
		 */
		Map<Doctor, Long> bIndex = this.assignement
				.values()
				.stream()
				.collect(Collectors.groupingBy(d -> d, Collectors.counting()));
		
		//Adding the doctors that are not present in the map (they have zero patients assigned) 
		this.doctors.forEach(d -> {
			if (!this.assignement.containsValue(d))
				bIndex.put(d, (long) 0);
		});

		//Creating the collection storing the strings
		Collection<String> doctorsByNumPatients = new LinkedList<String>();
		bIndex.forEach((key, value) -> {
			doctorsByNumPatients.add(Long.toString(value) + " : " + key.getDocID() + " " + key.getLastName() + " " + key.getFirstName() + "\n");
		});
		
		
		return doctorsByNumPatients;
	}
	
	/**
	 * Retrieves the number of patients per (their doctor's)  speciality
	 * <p>
	 * The information is a collections of strings structured as {@code ### - SPECIALITY}
	 * where {@code SPECIALITY} is the name of the speciality and 
	 * {@code ###} is the number of patients cured by doctors with such speciality (printed on three characters).
	 * <p>
	 * The elements are sorted first by decreasing count and then by alphabetic speciality.
	 * 
	 * @return the collection of strings with speciality and patient count information.
	 */
	public Collection<String> countPatientsPerSpecialization(){
		// TODO Complete method
		return null;
	}
	
}
