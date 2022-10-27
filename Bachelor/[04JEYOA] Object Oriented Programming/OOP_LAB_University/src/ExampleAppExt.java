import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.FileHandler;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import university.*;

public class ExampleAppExt {
	public static void main(String[] args) throws SecurityException, FileNotFoundException, IOException {
		
		String universityName = "Politecnico di Torino";
		
		UniversityExt poli = new UniversityExt(universityName);
		
		poli.setRector("Guido", "Saracco");
		
		System.out.println("Rector of " + poli.getName() + " : " + poli.getRector()); // Guido Saracco
		
		int s1 = poli.enroll("Mario","Rossi");
		int s2 = poli.enroll("Giuseppe","Verdi");
		
		System.out.println("Enrolled students " + s1 + ", " + s2); // 10000, 10001
		System.out.println("s1 = " + poli.student(s1)); // 10000 Mario Rossi
		
		int macro = poli.activate("Macro Economics", "Paul Krugman");
		int oop = poli.activate("Object Oriented Programming", "James Gosling");
		
		System.out.println("Activated courses " + macro + " and " + oop); // 10 and 11
		poli.register(s1, macro);
		poli.register(s2, macro);
		poli.register(s2, oop);
		
		System.out.println(poli.listAttendees(macro));
		// 10000 Mario Rossi
		// 10001 Giuseppe Verdi
		
		System.out.println(poli.studyPlan(s2));
		// 10,Macro Economics,Paul Krugman
		// 11,Object Oriented Programming,Marco Torchiano
		
		poli.exam(s1, macro, 27);
		poli.exam(s2, macro, 30);
		poli.exam(s2, oop, 28);
		
		System.out.println(poli.studentAvg(s2)); // 29.0
		
		System.out.println(poli.courseAvg(macro)); // 28.5
		
		System.out.println("Best students:\n" + poli.topThreeStudents());
	}
}
