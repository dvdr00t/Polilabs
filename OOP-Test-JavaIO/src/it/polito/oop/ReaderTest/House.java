package it.polito.oop.ReaderTest;

import java.io.FileNotFoundException;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class House {
	
	/*
	 * ATTRIBUTES
	 */
	private String name;
	private List<Person> people;
	
	/**
	 * CONSTRUCTOR with the list of people to add
	 * 
	 * @param name
	 * @param people
	 */
	public House(String name, List<Person> people) {
		this.name = name;
		this.people = people;
	}
	
	/** 
	 * CONSTRUCT with only the name of the house
	 * 
	 * @param name
	 */
	public House(String name) {
		this.name = name;
		this.people = new LinkedList<Person>();
	}

	/*
	 * GETTERS
	 */
	public String getName() {
		return name;
	}
	public List<Person> getPeople() {
		return people;
	}
	
	/**
	 * Add a new person to the list of people in the given house
	 * 
	 * @param person
	 */
	public static void addPersonToTheHouse(House house, Person person) {
		house.addPerson(person);
	}
	
	/**
	 * Add a person to this house
	 * 
	 * @param person
	 */
	private void addPerson(Person person) {
		this.people.add(person);
	}
	
	/**
	 * Factory methods that creates a new house by loading its data from a file.
	 * 
	 * The file must be a CSV file and it must contain the following fields:
	 * <ul>
	 * <li>{@code "Name"},
	 * <li>{@code "Surname"},
	 * <li>{@code "Year"},
	 * </ul>
	 * 
	 * The fields are separated by a semicolon (';').
	 * 
	 * @param name
	 *            the name of the house
	 * @param file
	 *            the path of the file
	 * @return House
	 * 			  a new instance of this class          
	 * @throws FileNotFoundException 
	 *  
	 */
	public static House fromFile(String houseName, String filePath) throws FileNotFoundException {
		
		//Creating the object that will be returned
		House house = new House(houseName);
		
		/**
		 * METHOD A: surrounded with TRY-and-CATCH declaration
		 * 
		 * The method ReadTest.readData try to read the data from file BUT
		 * if it catches any exception, it prints the stack trace of the exception
		 * 
		 */
//		try {
//			List<String> data = ReadTest.readData(filePath);
//		} catch (FileNotFoundException e) {
//			e.printStackTrace();
//		}
		
		/**
		 * METHOD B: let the caller class throw the exception
		 * 
		 * @throw FileNotFoundException
		 */
		List<String> data = ReadTest.readData(filePath);
		
		/*
		 * Creating the set of headers given from the file
		 * 
		 * .remove(index) return a list of String that correspond to the given position
		 * 		of the index
		 * .split(RegEx) split the list in an array of string
		 */
		String[] headers = data.remove(0).split(";");
		
		/*
		 * Creating a map that stores as:
		 * 
		 *  KEYS: the header string
		 *  VALUES: the position in the .csv file
		 */
		Map<String, Integer> map = new LinkedHashMap<String, Integer>();
		for (int i = 0; i < headers.length; i++)
			map.put(headers[i], i);
		
		/*
		 * READING EACH LINE
		 */
		data.forEach(line -> {
			
			//Splitting each line in order to retrieve the data
			String[] values = line.split(";");
			
			//Retrieving the data
			String name = values[map.get("NOME")];
			String surname = values[map.get("COGNOME")];
			String year = values[map.get("ANNO")];
			
			addPersonToTheHouse(house, new Person(name, surname, Integer.parseInt(year)));
		});
		
		
		return house;
		
	}
	
	
	public String toString() {
		List<String> list = this.people
				.stream()
				.map(p -> p.toString())
				.collect(Collectors.toList());
		
		String s = this.name + " {";
		for (String add: list)
			s = s + add + " -o- ";
		return s;
	}
	
	

}
