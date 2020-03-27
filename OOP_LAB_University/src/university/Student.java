package university;

public class Student {
	
	/*
	 * 	ATTRIBUTES
	 */
	
	//CONSTANTS
	private final int MAX_ATTENDED_COURSE = 25;
	
	//STUDENTS INFORMATIONS
	private String nameStudent;
	private String surnameStudent;
	private int idStudent;
	
	//COURSES ATTENDED BY THE STUDEND
	private Course[] attendedCourses = new Course[25];
	private int attendedCoursesNumber = 0;
	
	
	/*
	 * 	CONSTRUCTOR
	 */
	
	public Student (String name, String surname) {
		
		//ASSIGNING INITIAL VALUES TO THE NEW STUDENT
		this.nameStudent = name;
		this.surnameStudent = surname;
	}

	
	/**
	 * 
	 * GETTERS AND SETTERS FOR STUDENTS ID
	 * 
	 */
	public int getIdStudent() {
		return idStudent;
	}
	
	void setIdStudent(int enrolledNumber) {
		this.idStudent = enrolledNumber + 10000;
	}
	
	
	/*
	 * 	GETTERS OF STUDENTS NAME
	 */
	public String getNameStudent() {
		return nameStudent;
	}

	public String getSurnameStudent() {
		return surnameStudent;
	}

	
	/*
	 * 	METHODS
	 */
	
	public void attendCourse (Course course) {
		
		//CHEKCING IF THE STUDENT CAN ATTEND OTHER COURSES
		if (this.attendedCoursesNumber < MAX_ATTENDED_COURSE) {
			
			//ASSIGING THE COURSE TO THE STUDENT
			this.attendedCourses[this.attendedCoursesNumber] = course;
			attendedCoursesNumber++;
			
		}
		else
			System.out.println("Ops! Seems like this student has a lot of work to do yet!");
	}
	
	public String showAttended () {
		
		//CREATING THE STRING CONTAINING THE LIST OF COURSES
		String attendedList = new String("\n");
		
		//LINKING TOGHETER ALL THE INFORMATIONS
		for (int i = 0; i < this.attendedCoursesNumber; i++) {
			attendedList = (attendedList +this.attendedCourses[i].getIdCourse() + " " + this.attendedCourses[i].getCourseName() + " " + this.attendedCourses[i].getTeacherName() + "\n");
		}
		
		attendedList = attendedList + "\n";
		
		return attendedList;
	}
	
	

}
