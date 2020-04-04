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
	private int[] exams = new int[MAX_ATTENDED_COURSE];
	private int nExams = 0;
	
	//COURSES ATTENDED BY THE STUDEND
	private Course[] attendedCourses = new Course[MAX_ATTENDED_COURSE];
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
	
	
	/**
	 * Register the grade fo the exam
	 * 
	 * @param grade
	 * @return void
	 */
	
	void registerGrade (int grade) {
		
		//REGISTERING THE NEW GRADE IN THE FIRST AVAILABLE POSITION
		for (int i = 0; i < exams.length; i++) {
			if (exams[i] == 0) {
				exams[i] = grade;
				this.nExams++;
				System.out.println("Exam has been registered with success!");
				return;
			}
		}
		
		System.out.println("Could not register the exam...");
		return;
	}
	
	/**
	 * Computing average grades
	 * 
	 * @param void
	 * @return average
	 */

	float computeAvg () {
		
		float sum = 0;
		
		//CHECKING IF THE STUDENT HAS TAKES AT LEAST ONE EXAM
		if (this.nExams != 0) {
			
			//COMPUTING THE AVG
			for (int i = 0; i < this.nExams; i++) {
				sum = sum + this.exams[i];
			}
			return (sum/this.nExams);
		}
		
		//RETURNING THE FLAG VALUE FOR NO EXAMS TAKES
		else
			return -1;
	}
	
	
}
