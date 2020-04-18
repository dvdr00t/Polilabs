package university;

import java.util.logging.Logger;

/**
 * This class represents a university education system.
 * 
 * It manages students and courses.
 *
 */
public class University {
	
	/**
	 *	ATTRIBUTES 
	 */
	protected final static Logger logger = Logger.getLogger("University");
	
	//CONSTANTS
	private final int MAX_STUDENTS_NUMBER = 1000;
	private final int MAX_COURSES_NUMBER = 50;
	
	//UNIVERSITY INFORMATION
	private String universityName;
	private String rectorName;
	private String rectorSurname;
	
	//STUDENTS AND COURSES INFORMATION
	protected int studentsNumber = 0;
	private int coursesNumber = 0;
	protected Student[] students = new Student[MAX_STUDENTS_NUMBER];
	private Course[] courses = new Course[MAX_COURSES_NUMBER];
	
	

	/**
	 * Constructor
	 * @param name name of the university
	 */
	public University(String name){
		
		//ASSIGNING THE NAME PASSED IN THE CONSTRUCTOR TO THE UNIVERISTY NAME
		this.universityName = name;
	}
	
	/**
	 * Getter for the name of the university
	 * @return name of university
	 */
	public String getName(){
		
		//RETURNING THE NAME OF THE UNIVERSITY TO THE CALLER
		return this.universityName;
	}
	
	
	
	
	/**
	 * Defines the rector for the university
	 * 
	 * @param first
	 * @param last
	 */
	public void setRector(String first, String last){
		
		//ASSIGNING THE NAME AND THE SURNAME TO THE RECTOR
		this.rectorName = first;
		this.rectorSurname = last;
	}
	
	/**
	 * Retrieves the rector of the university
	 * 
	 * @return
	 */
	public String getRector(){
		
		//RETURNING THE FULL NAME OF THE RECTOR
		return (this.rectorName + " " + this.rectorSurname);
	}
	
	/**
	 * Enroll a student in the university
	 * 
	 * @param first first name of the student
	 * @param last last name of the student
	 * @return
	 */
	public int enroll(String first, String last){
		
		//CHECKING IF THERE IS SPACE FOR STUDENTS
		if (this.studentsNumber < MAX_STUDENTS_NUMBER) {
			
			//CREATING A NEW STUDENT
			Student studentTMP = new Student(first, last);
			studentTMP.setIdStudent(this.studentsNumber);
			
			//ASSIGNING THE NEW STUDENT TO THE ARRAY OF STUDENTS
			students[this.studentsNumber] = studentTMP;
			
			//ADDING INFORMATION TO THE LOGGER
			logger.info("New student enrolled: " + studentTMP.getIdStudent() + ", " + studentTMP.getNameStudent() + " " + studentTMP.getSurnameStudent());
			
			//INCREMENTING THE NUMBER OF STUDENTS
			this.studentsNumber++;
			
			
			
			//RETURNING THE NEW STUDENT ID
			return studentTMP.getIdStudent();
		}
		
		else {
			System.out.println("Ops! Seems like we have no more space for new students!");
			return -1;
		}
	}
	
	/**
	 * Retrieves the information for a given student
	 * 
	 * @param id the id of the student
	 * @return information about the student
	 */
	public String student(int id){
		
		//CHECKING IF THE STUDENT ID IS CORRECT
		if (id >= 10000 && id < 10000 + MAX_STUDENTS_NUMBER) {
			
			//RETURNING STUDENT INFORMATION
			return (id + " " + students[id-10000].getNameStudent() + " " + students[id-10000].getSurnameStudent());
		}
		
		else
			return "Ops, this student does not exist!";
	}
	
	
	
	
	
	
	/**
	 * Activates a new course with the given teacher
	 * 
	 * @param title title of the course
	 * @param teacher name of the teacher
	 * @return the unique code assigned to the course
	 */
	public int activate(String title, String teacher){
		
		//CHECKING IS THERE IS SPACE FOR NEW COURSES
		if (this.coursesNumber < MAX_COURSES_NUMBER) {
			
			//CREATING A NEW COURSE
			Course courseTMP = new Course(title, teacher);
			courseTMP.setIdCourse(this.coursesNumber);
			
			//ASSIGNING THE NEW COURSE TO THE COURSES ARRAY
			courses[this.coursesNumber] = courseTMP;
			
			//ADDING INFORMATION TO THE LOGGER
			logger.info("New course activated: " + courseTMP.getIdCourse() + ", " + courseTMP.getCourseName() + " with " + courseTMP.getTeacherName());
			
			//INCREMENTING THE NUMBER OF COURSE
			this.coursesNumber++;
			
			//RETURNING THE NEW COURSE
			return courseTMP.getIdCourse();
		}
		else {
			System.out.println("Ops! Seems like we have no more room for new courses!");
			return -1;
		}
	}
	
	/**
	 * Retrieve the information for a given course
	 * 
	 * @param code unique code of the course
	 * @return information about the course
	 */
	public String course(int code){
		
		//CHECKING IF THE COURSE CODE IS CORRECT 
		if (code >= 10 && code < 10 + MAX_COURSES_NUMBER) {
			
			//RETURNING COURSE INFORMATION
			return (code + ", " + courses[code-10].getCourseName() + ", " + courses[code-10].getTeacherName());
		}
		else 
			return "Ops! This course does not exist!";
	}
	
	
	
	
	
	
	
	
	/**
	 * Register a student to attend a course
	 * @param studentID id of the student
	 * @param courseCode id of the course
	 */
	public void register(int studentID, int courseCode){
		
		//LINKING COURSES AND STUDENTS
		courses[courseCode-10].enrollStudent(students[studentID-10000]);
		students[studentID-10000].attendCourse(courses[courseCode-10]);
		
		//ADDING INFORMATION TO THE LOGGER
		logger.info("Student " + students[studentID-10000].getIdStudent() + " signed up for course " + courses[courseCode-10].getCourseName());
		
	}
	
	
	/**
	 * Retrieve a list of attendees
	 * 
	 * @param courseCode unique id of the course
	 * @return list of attendees separated by "\n"
	 */
	public String listAttendees(int courseCode){
		
		//RETURNING THE LIST OF STUDENTS ENROLLED FOR THE COURSE
		return courses[courseCode-10].showEnrolled();
	}

	
	
	/**
	 * Retrieves the study plan for a student
	 * 
	 * @param studentID id of the student
	 * @return list of courses the student is registered for
	 */
	public String studyPlan(int studentID){
		
		//RETURNING THE LIST OF COURSES ATTENDED BY THE STUDENT
		return students[studentID-10000].showAttended();
	}
	
	
	/**
	 * Register an exam
	 * 
	 * @param studentID, courseCODE, grade
	 * @return void
	 */
	
	public void exam(int studentId, int courseId, int grade) {
		
		//REGISTERING THE EXAM FOR THE STUDENT
		students[studentId-10000].registerGrade(grade);
		students[studentId-10000].setAvgExams(students[studentId-10000].computeAvg());
		students[studentId-10000].setAvgExamsAward(students[studentId-10000].getAvgExams() + students[studentId-10000].computeAvgAward());
		
		//REGISTERING THE GRADE FOR THE COURSE
		courses[courseId-10].registerGrade(grade);
		
		//ADDING INFORMATION TO THE LOGGER
		logger.info("Student " + students[studentId-10000].getIdStudent() + " took an exam in course " + courses[courseId-10].getCourseName() + " with grade " + grade);
		
	}
	
	/**
	 * Showing average grades for a student
	 *
	 * @param studentID
	 * @return String
	 */
	
	public String studentAvg(int studentId) {
		
		//COMPUTING AVERAGE GRADES
		float avg = students[studentId-10000].getAvgExams();
		if (avg == -1) {
			return "Student " + studentId + " hasn't taken any exams.";
		}
		else {
			return "Student " + studentId + ": " + avg;
		}
	}
	
	/**
	 * Showing average grades for an exam
	 * 
	 * @param courseCODE
	 * @return String
	 */
	
	public String courseAvg(int courseCode) {
		
		//COMPUTING AVERAGE GRADES
		float avg = courses[courseCode-10].computeAvg();
		if (avg == -1) {
			return "No students ha take the exams in " + courses[courseCode-10].getCourseName();
		}
		else {
			return "The average for the course " + courses[courseCode-10].getCourseName() + " is: " + avg;
		}
	}
}













