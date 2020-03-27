package university;

public class Course {
	
	/*
	 * 	ATTRIBUTES 
	 */
	
	//CONSTANTS
	private final int MAX_ENROLLED_STUDENTS = 100;

	//COURSE INFOMATION
	private String courseName;
	private String teacherName;
	private int idCourse;
	
	//STUDENTS ENROLLED FOR THE COURSE
	private int enrolledStudentNumber = 0;
	private Student[] enrolledStudent = new Student[100];
	
	/*
	 * 	CONSTRUCTOR
	 */
	public Course(String courseName, String teacherName) {
		
		this.courseName = courseName;
		this.teacherName = teacherName;
	}
	
	
	/*
	 * 	GETTERS AND SETTERS FOR ID 
	 */
	public int getIdCourse() {
		return idCourse;
	}
	void setIdCourse(int idCourse) {
		this.idCourse = idCourse + 10;
	}



	/*
	 * 	GETTERS FOR INFORMATIONS ABOUT THE COURSE
	 */
	
	public String getCourseName() {
		return courseName;
	}
	public String getTeacherName() {
		return teacherName;
	}
	
	/*
	 * 	METHODS
	 */
	
	public void enrollStudent (Student student) {
		
		//CHECKING IF THE COURSE IS FULL OF STUDENTS
		if (this.enrolledStudentNumber < MAX_ENROLLED_STUDENTS) {
			
			//ASSIGNING NEW STUDENT IN THE COURSE
			this.enrolledStudent[this.enrolledStudentNumber] = student;
			this.enrolledStudentNumber++;
			
		}
		else 
			System.out.println("Ops! Seems like this course is full of students!");
		
	}
	
	public String showEnrolled () {
		
		//CREATING THE STRING CONTAINING THE LIST OF STUDENTS
		String enrolledList = new String("\n");
		
		for (int i = 0; i < this.enrolledStudentNumber; i++) {
			enrolledList = (enrolledList + enrolledStudent[i].getIdStudent() + " " + enrolledStudent[i].getNameStudent() + " " + enrolledStudent[i].getSurnameStudent() + "\n");
		}
		
		enrolledList = (enrolledList + "\n");
		return enrolledList;
		
	}
	
	
}
