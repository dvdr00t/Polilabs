package university;
import java.util.Arrays;
import java.util.Comparator;
import java.util.logging.Logger;

public class UniversityExt extends University {
	
	private final static Logger logger = Logger.getLogger("University");

	public UniversityExt(String name) {
		super(name);
		// Example of logging
		logger.info("Creating extended university object");
	}
	
	/*
	 * METHODS
	 */
	public String topThreeStudents() {
		
		//SORTING ARRAY BY AVERAGE
		Arrays.sort(this.students, Comparator.comparing(Student::getAvgExams));
		
		//SHOWING BEST STUDENTS
		String string = "\n";
		
		return string;
	}
}
