package clinic;

public abstract class Person {
	
	/*
	 * ATTRIBUTES
	 */
	private String firstName;
	private String lastName;
	private String SSN;
	
	
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param firstName
	 * @param lastName
	 * @param sSN
	 */
	public Person(String firstName, String lastName, String SSN) {
		super();
		this.firstName = firstName;
		this.lastName = lastName;
		this.SSN = SSN;
	}
	
	
	/*
	 * GETTERS AND SETTERS
	 */
	
	public String getFirstName() {
		return firstName;
	}
	public String getLastName() {
		return lastName;
	}
	public String getSSN() {
		return SSN;
	}

	
	
}
