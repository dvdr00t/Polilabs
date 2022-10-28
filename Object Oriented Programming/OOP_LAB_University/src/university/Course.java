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
	private int[] grades = new int[MAX_ENROLLED_STUDENTS];
	private int nGrades = 0;
	
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
	
	/**
	 * Register the grade
	 * 
	 * @param grade
	 * @return void
	 */
	
	void registerGrade(int grade) {
		
		//REGISTERING THE GRADE IN THE FOIRST AVAILABLE POSITION
		for (int i = 0; i < grades.length; i++) {
			if (grades[i] == 0) {
				grades[i] = grade;
				++this.nGrades;
				System.out.println("Grade has been registered with success!");
				return;
			}
		}

		System.out.println("Grades has not been registered...");
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
		if (this.nGrades != 0) {
			
			//COMPUTING THE AVG
			for (int i = 0; i < this.nGrades; i++) {
				sum = sum + this.grades[i];
			}
			return (sum/this.nGrades);
		}
		
		//RETURNING THE FLAG VALUE FOR NO EXAMS TAKES
		else
			return -1;
	}

}