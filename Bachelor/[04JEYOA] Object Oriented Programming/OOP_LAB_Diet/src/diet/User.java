package diet;

/**
 * Represent a take-away system user
 *  
 */
public class User implements Comparable<User>{
	
	/*
	 * Attributes
	 */
	private String firstName;
	private String lastName;
	private String email;
	private String phoneNumber;
	


	/**
	 * Constructor of the user
	 * 
	 * @param firstName first name of the user
	 * @param lastName  last name of the user
	 * @param email     email
	 * @param phoneNumber telephone number
	 */
	public User(String firstName, String lastName, String email, String phoneNumber) {
		this.firstName = firstName;
		this.lastName = lastName;
		this.email = email;
		this.phoneNumber = phoneNumber;
	}
		
	/**
	 * get user's last name
	 * @return last name
	 */
	public String getLastName() {
		return this.lastName;
	}
	
	/**
	 * get user's first name
	 * @return first name
	 */
	public String getFirstName() {
		return this.firstName;
	}
	
	/**
	 * get user's email
	 * @return email
	 */
	public String getEmail() {
		return this.email;
	}
	
	/**
	 * get user's phone number
	 * @return  phone number
	 */
	public String getPhone() {
		return this.phoneNumber;
	}
	
	/**
	 * change user's email
	 * @param email new email
	 */
	public void SetEmail(String email) {
		this.email = email;
	}
	
	/**
	 * change user's phone number
	 * @param phone new phone number
	 */
	public void setPhone(String phone) {
		this.phoneNumber = phone;
	}

	@Override
	public int compareTo(User o) {
		
		//Case in which the last names are the same
		if (this.lastName.equals(o.getLastName())) {
			return this.firstName.compareTo(o.getFirstName());
		}
		
		return this.lastName.compareTo(o.getLastName());
	}
	
	@Override
	public String toString() {
		return this.firstName + " " + this.lastName;
	}
	
}
