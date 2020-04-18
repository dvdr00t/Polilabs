package university;
import java.util.Arrays;
import java.util.Comparator;
import java.lang.Float;
import java.util.HashMap;
import java.util.TreeMap;
import java.util.logging.Logger;

public class UniversityExt extends University {
	
	public UniversityExt(String name) {
		super(name);
		logger.info("Creating extended university object");
	}
	
	/*
	 * ATTRIBUTE
	 */
	HashMap<Integer, Float> map = new HashMap<Integer, Float>();

	
	/*
	 * METHODS
	 */
	public String topThreeStudents() {
		
		/*
		 * 			SORTING STUDENTS USING MAP
		 */
		/*
		//SORTING ARRAY BY AVERAGE EXAMS
		for (int i = 0; i < this.studentsNumber; i++)
			map.put(this.students[i].getIdStudent(), this.students[i].getAvgExamsAward());
		TreeMap<Integer, Float> treeMap = new TreeMap<>(map);
		
		//SHOWING BEST STUDENTS
		String str = "";
		for (int i = 0; i < this.studentsNumber; i++) {
			str = str + this.students[treeMap.lastKey() - 10000].getNameStudent() + " " + this.students[treeMap.lastKey() - 10000].getSurnameStudent();
			str = str + ": " + this.students[treeMap.lastKey() - 10000].getAvgExamsAward() + "\n";
			treeMap.remove(treeMap.lastKey());
		}
		*/
		
		
		/*
		 * 			SORTING STUDENTS USING COMPARABLE
		 */
		
		//COPYING THE ARRAY OF STUDENTS IN A NEW ARRAY
		Student[] tmp = Arrays.copyOf(students, this.studentsNumber);
		
		//SORTING THE ARRAY
		Arrays.sort(tmp, new Comparator<Student>() {
			@Override
			public int compare(Student s1, Student s2) {
				return (int) Math.signum(s2.getAvgExamsAward() - s1.getAvgExamsAward());
			}
		});
		
		//CREATING A NEW STRING
		StringBuffer top3 = new StringBuffer();
		
		//CHECKING NUMBER OF STUDENTS
		int top;
		if (this.studentsNumber > 3)
			top = 3;
		else 
			top = this.studentsNumber;
			
		//CREATING STRING TO RETURN
		for (int i = 0; i < top; i++)
			top3.append(tmp[i].getNameStudent()).append(" ").append(tmp[i].getSurnameStudent()).append(": ").append(tmp[i].getAvgExamsAward()).append("\n");
			
		
		return top3.toString();
	}

	
}
