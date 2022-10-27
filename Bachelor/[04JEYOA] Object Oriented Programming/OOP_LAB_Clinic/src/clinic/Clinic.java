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
import java.util.Map.Entry;
import java.util.function.Supplier;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.DoubleStream;

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
	
	
	/**
	 * Constructor
	 * 
	 * @param void
	 */
	public Clinic () {
		this.patients = new LinkedList<Patient>();
		this.doctors = new LinkedList<Doctor>();
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
		
		/*
		 * Retrieve an element from a stream that matched the satisfied
		 * condition or throw an exception 
		 * 
		 * .filter() evaluate the condition
		 * .findFirst() stop the execution on the first occurrence
		 * .orElseThrown() thrown a new Exception
		 */
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
		
		/*
		 * Retrieve an element from a stream that matched the satisfied
		 * condition or throw an exception 
		 * 
		 * .filter() evaluate the condition
		 * .findFirst() stop the execution on the first occurrence
		 * .orElseThrown() thrown a new Exception
		 */
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
		p.assignDoctor(d);
		
		//Increment the number of patient assigned
		d.addPatient(p);
		
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
			
		return p.getAssignedDoctor().getDocID();
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
		
		/*
		 * Transform a COLLECTION of objects (List<Object> list1) into a new COLLECTION
		 * of an attribute of that object (List<Attribute> list2)
		 * 
		 * .getAllPatients() return a collection of Patient (List<Patient>)
		 * .stream() transform the COLLECTION into a STREAM
		 * .map() require a lambda function that map all the objects into a new element (an attribute in this case)
		 * .collect(Collectors.toList()) back-transform the STREAM into a COLLECTION 
		 */
		
		return d.getAllPatients()
				.stream()
				.map(p -> p.getSSN())
				.collect(Collectors.toList());
		
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
		
		/*
		 * Transform a COLLECTION of objects (List<Object> list1) into a new COLLECTION
		 * of an attribute of that object (List<Attribute> list2)
		 * 
		 * .getAllPatients() return a collection of Patient (List<Patient>)
		 * .stream() transform the COLLECTION into a STREAM
		 * .filter() evaluate the condition on the attribute
		 * .map() require a lambda function that map all the objects into a new element (an attribute in this case)
		 * .collect(Collectors.toList()) back-transform the STREAM into a COLLECTION 
		 */
		Collection<Integer> idleDoctors = this.doctors
				.stream()
				.filter(d -> d.getNumberOfPatient() == 0)
				.map(d -> d.getDocID())
				.collect(Collectors.toList());
				
		return idleDoctors;

	}

	/**
	 * Retrieves the collection of doctors having a number of patients larger than the average.
	 * 
	 * @return  the collection of doctors' ids
	 */
	public Collection<Integer> busyDoctors(){
		
		
//		/*
//		 * Given the map of patient associate with their doctors, we want to create a new map 
//		 * that has:
//		 * 
//		 *  KEYS: every doctor
//		 *  VALUE: the number of patients he/She has
//		 */
//		Map<Doctor, Long> bIndex = this.assignement
//				.values()
//				.stream()
//				.collect(Collectors.groupingBy(d -> d, Collectors.counting()));
//		
//		//Adding the doctors that are not present in the map (they have zero patients assigned) 
//		this.doctors.forEach(d -> {
//			if (!this.assignement.containsValue(d))
//				bIndex.put(d, (long) 0);
//		});
		
		/*
		 * Transform a given COLLECTION into a new MAP using the Collectors.toMap(keyMapper, valueMapper)
		 * 
		 * .stream() transform the COLLECTION into a STREAM
		 * .collect(Collectors.toMap()) transform the STREAM into a MAP
		 */
		Map<Doctor, Integer> bIndex = this.doctors
				.stream()
				.collect(Collectors.toMap(d -> d, d -> d.getNumberOfPatient()));
		
		/*
		 * Given a MAP, compute the average value of the value set in the MAP
		 */
		double avg = bIndex.values()
				.stream()
				.mapToDouble(Integer::doubleValue)
				.average()
				.orElse(0);
		
		
		/*
		 * Given a LIST, compute the average value of an attribute of the objects in the LIST
		 * 
		 * .stream() transform the COLLECTION into a STREAM
		 * .map() require a lambda function that map all the objects into a new element (an attribute in this case)
		 * .collect(Collectors.toList()) back-transform the STREAM into a new COLLECTION
		 * 
		 * .stream() transform the COLLECTION into a STREAM
		 * .mapToDouble() require a lambda function that map all the objects (Integer in this case) into a new 
		 * 				element (Double in this case)
		 * .average() compute the average value
		 * .orElse() assign zero by default
		 * 
		 */
		double avg1 = this.doctors
				.stream()
				.map(d -> d.getNumberOfPatient())
				.collect(Collectors.toList())
				
				.stream()
				.mapToDouble(x -> x)
				.average()
				.orElse(0);
				
		//Retrieving all the doctors that has a number of patients lower than the average
		Collection<Integer> busyDoctors = bIndex.keySet()
				.stream()
				.filter(d -> d.getNumberOfPatient() > avg1)
				.map(d -> d.getDocID())
				.collect(Collectors.toList());
		
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
		 * Generating a new collection of String given a collection of doctors.
		 * 
		 * .sorted() is in charge of the sorting part
		 * 		Comparator.comparing() retrieve the attribute of the objects to compare
		 * 					.reversed() sort the attributes in a decreasing order
		 * .map(Lambda -> Function) map all the objects to a new specified type
		 * .collect(Collectors.toSomething()) create a new collection from the stream
		 */
		return this.doctors
				.stream()
				.sorted(Comparator.comparing(Doctor::getNumberOfPatient).reversed())
				.map(d -> String.format("%3d", d.getNumberOfPatient()) + " : " + d.getDocID() + " " + d.getLastName() + " " +d.getFirstName() + "\n")
				.collect(Collectors.toList());
		
		
		
	}
	
	/**
	 * Retrieves the number of patients per (their doctor's)  specialty
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
		
		/*
		 * Given a COLLECTION, create a MAP that has:
		 * 
		 *  KEYS: the element of the collection
		 *  VALUES: an attribute of the element in the collection
		 *  
		 * and then transform the MAP into a new MAP that has:
		 * 
		 *  KEYS: the DISTINCT value in the previous map
		 *  VALUES: the counting relative to each value
		 *  
		 * and the transform the MAP into a SORTED COLLECTION of string.
		 * 
		 * 
		 * -------------------------------------------------
		 * FROM COLLLECTION TO MAP:
		 * 
		 * .stream() transform the COLLECTION into a STREAM
		 * .collect(Collectors.ToMap()) transform the STREAM into a MAP
		 * 
		 * -------------------------------------------------
		 * FROM MAP TO A NEW MAP (with keys the values of the previous one)
		 * 
		 * .values() retrieves all the values of the previous one (.keySet() may be used 
		 * 				if the keys of the previous one are required instead the values)
		 * .stream() transform the SET into a STREAM
		 * .collect(Collectors.groupingBy()) transform the STREAM into a MAP
		 * 
		 * -------------------------------------------------
		 * FROM MAP TO COLLECTION
		 * 
		 * .entrySet() retrieve the entries of the MAP in the form of Set<Entry<Keys, Values>>
		 * .stream() transform the SET into a STREAM
		 * .sorted() a comparator is provided in order to compare the entries in the stream (of the map)
		 * .map() a lambda function is required in order to transform all the entries into a new element (String)
		 * .collect(Collectors.toList()) transform the STREAM into a COLLECTION
		 * 
		 */
		return this.patients
					.stream()
					.collect(Collectors.toMap(p -> p, p -> p.getAssignedDoctor().getSpecialization()))
					
					.values()
					.stream()
					.collect(Collectors.groupingBy(s -> s, Collectors.counting()))
					
					.entrySet()
					.stream()
					.sorted(new Comparator<Map.Entry<String, Long>>() {

						@Override
						public int compare(Entry<String, Long> o1, Entry<String, Long> o2) {
							if (o1.getValue() > o2.getValue())
								return -1;
							else if (o1.getValue() < o2.getValue())
								return 1;
							else
								return o1.getKey().compareTo(o2.getKey());
						}
						
					})
					.map(e -> String.format("%3d", e.getValue()) + " - " + e.getKey())
					.collect(Collectors.toList());
	}
	
}
