package university;

import java.util.logging.Logger;

public class UniversityExt extends University {
	
	private final static Logger logger = Logger.getLogger("University");

	public UniversityExt(String name) {
		super(name);
		// Example of logging
		logger.info("Creating extended university object");
	}
	
	public void exam(int studentId, int courseID, int grade) {
		
	}

	public String studentAvg(int studentId) {
		return null;
	}
	
	public String courseAvg(int courseId) {
		return null;
	}
	
	public String topThreeStudents() {
		return null;
	}
}
