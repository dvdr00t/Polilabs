package it.polito.oop.ReaderTest;

public class Person {
	
	/*
	 * ATTRIBUTES
	 */
	private String name;
	private String surname; 
	private Integer year;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param name
	 * @param surname
	 * @param year
	 */
	public Person(String name, String surname, Integer year) {
		this.name = name;
		this.surname = surname;
		this.year = year;
	}

	
	/*
	 * GETTERS
	 */
	public String getName() {
		return name;
	}
	public String getSurname() {
		return surname;
	}
	public Integer getYear() {
		return year;
	}
	
	public String toString() {
		return this.name + " " + this.surname + " " + this.year;
	}

	
	
}
