import static org.junit.Assert.*;

import java.util.Collection;
import java.util.regex.Pattern;

import org.junit.Test;

import clinic.Clinic;
import clinic.NoSuchDoctor;
import clinic.NoSuchPatient;

public class ExampleTest {

	@Test
	public void test() throws NoSuchPatient, NoSuchDoctor {
		Clinic hospital = new Clinic();

		hospital.addPatient("Alice", "Green", "ALCGRN");
		hospital.addPatient("Robert", "Smith", "RBTSMT");
		hospital.addPatient("Steve", "Moore", "STVMRE");
		hospital.addPatient("Susan", "Madison", "SSNMDS");
		
		String alice = hospital.getPatient("ALCGRN");
		Pattern p = Pattern.compile("green\\s+alice\\s+\\(\\s*ALCGRN\\s*\\)",Pattern.CASE_INSENSITIVE);
		assertNotNull("Missing patient",alice);
		assertTrue("Wrong patient format: " + alice,p.matcher(alice).matches());

		hospital.addDoctor("George", "Sun","SNUGRG", 14,"Physician");
		hospital.addDoctor("Kate", "Love", "LVOKTA",86,"Physician");
		
		String kate = hospital.getDoctor(86);
		assertNotNull("Missing doctor",kate);
		assertTrue("Missing doctor's id",kate.contains("86"));
		assertTrue("Missing doctor's specialization",kate.contains("Physician"));

		hospital.assignPatientToDoctor("SSNMDS", 86);
		hospital.assignPatientToDoctor("ALCGRN", 14);
		hospital.assignPatientToDoctor("RBTSMT", 14);
		hospital.assignPatientToDoctor("STVMRE", 14);

		int susanDoc = hospital.getAssignedDoctor("SSNMDS");
		assertEquals("Wrong doctor for Susan",86,susanDoc);

		Collection<String> patients = hospital.getAssignedPatients(14);

		assertNotNull("Missing George's patients",patients);
		
		try {
			hospital.getAssignedPatients(-1);
			fail("Invalid id should raise an exception");
		} catch (Exception ex) {
			// ok
		}
		
		Collection<Integer> busy = hospital.busyDoctors();
		
		assertNotNull("Missing busy doctors",busy);
		assertEquals("Too many busy doctors detected",1,busy.size());
		assertTrue("Missing busy doctor",busy.contains(14));
	}

}
