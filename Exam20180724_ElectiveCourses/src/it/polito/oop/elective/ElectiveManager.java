package it.polito.oop.elective;

import java.util.Collection;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.stream.Collectors;

/**
 * Manages elective courses enrollment.
 * 
 *
 */
public class ElectiveManager {
	
	/*
	 * ATTRIBUTES
	 */
	private Map<String, Course> courses;
	private Map<String, Student> students;
	private List<Notifier> listeners;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param void
	 */
	public ElectiveManager() {
		this.courses = new LinkedHashMap<String, Course>();
		this.students = new LinkedHashMap<String, Student>();
		this.listeners =  new LinkedList<Notifier>();
	}


    /**
     * Define a new course offer.
     * A course is characterized by a name and a number of available positions.
     * 
     * @param name : the label for the request type
     * @param availablePositions : the number of available positions
     */
    public void addCourse(String name, int availablePositions) {
        this.courses.put(name, new Course(name, availablePositions));
    }
    
    /**
     * Returns a list of all defined courses
     * @return
     */
    public SortedSet<String> getCourses(){
    	
    	/*
    	 * Converting a STREAM to a COLLECTION undefined by a default function in Eclipse
    	 * 
    	 * 
    	 * 
    	 */
        return this.courses.values()
        		.stream()
        		.map(c -> c.getName())
        		.sorted()
        		.collect(Collectors.toCollection(TreeSet::new));
     }
    
    /**
     * Adds a new student info.
     * 
     * @param id : the id of the student
     * @param gradeAverage : the grade average
     */
    public void loadStudent(String id, double gradeAverage){
    	this.students.put(id, new Student(id, gradeAverage));
    }

    /**
     * Lists all the students.
     * 
     * @return : list of students ids.
     */
    public Collection<String> getStudents(){
    	return this.students.keySet();
    }
    
    /**
     * Lists all the students with grade average in the interval.
     * 
     * @param inf : lower bound of the interval (inclusive)
     * @param sup : upper bound of the interval (inclusive)
     * @return : list of students ids.
     */
    public Collection<String> getStudents(double inf, double sup){
        return this.students.values()
        		.stream()
        		.filter(s -> s.getGrade() >= inf && s.getGrade() <= sup)
        		.map(Student::getId)
        		.collect(Collectors.toList());
    }


    /**
     * Adds a new enrollment request of a student for a set of courses.
     * <p>
     * The request accepts a list of course names listed in order of priority.
     * The first in the list is the preferred one, i.e. the student's first choice.
     * 
     * @param id : the id of the student
     * @param selectedCourses : a list of of requested courses, in order of decreasing priority
     * 
     * @return : number of courses the user expressed a preference for
     * 
     * @throws ElectiveException : if the number of selected course is not in [1,3] or the id has not been defined.
     */
    public int requestEnroll(String id, List<String> courses)  throws ElectiveException {
    	
    	if (!this.students.containsKey(id))
    		throw new ElectiveException();
    	
    	if (courses.size() < 1 || courses.size() > 3)
    		throw new ElectiveException();
    	
    	if (!courses.stream().allMatch(this.courses::containsKey))
    		throw new ElectiveException();
    	
    	//Adding to the student preferences the list of courses selected    	
    	this.students.get(id).addCourses(courses.stream().map(this.courses::get).collect(Collectors.toList()));
    	
    	//Registering the enrollment to all the notifiers
    	this.listeners.forEach(l -> {
    		l.requestReceived(id);
    	});
    	
        return courses.size();
    }
    
    /**
     * Returns the number of students that selected each course.
     * <p>
     * Since each course can be selected as 1st, 2nd, or 3rd choice,
     * the method reports three numbers corresponding to the
     * number of students that selected the course as i-th choice. 
     * <p>
     * In case of a course with no requests at all
     * the method reports three zeros.
     * <p>
     * 
     * @return the map of list of number of requests per course
     */
    public Map<String,List<Long>> numberRequests(){
    	return null;
    }
    
    
    /**
     * Make the definitive class assignments based on the grade averages and preferences.
     * <p>
     * Student with higher grade averages are assigned to first option courses while they fit
     * otherwise they are assigned to second and then third option courses.
     * <p>
     *  
     * @return the number of students that could not be assigned to one of the selected courses.
     */
    public long makeClasses() {
        
    	this.students.values()
    		.stream()
    		.sorted(new Comparator<Student>() {

				@Override
				public int compare(Student o1, Student o2) {
					if (o1.getGrade() >= o2.getGrade())
						return -1;
					else
						return 1;
				}
    			
    		})
    		.forEach(s -> {
    			s.getPreferences().stream()
    				.filter(c -> !c.isFull())
    				.findFirst().ifPresent(c -> {
    					c.addStudent(s);
    					this.listeners.forEach(l -> {
    						l.assignedToCourse(s.getId(), c.getName());
    					});
    				});	
    				
    		});
    	
    	return this.students.values().stream().filter(s -> s.getEnrolledIn() == null).count();
    }
    
    
    /**
     * Returns the students assigned to each course.
     * 
     * @return the map course name vs. student id list.
     */
    public Map<String,List<String>> getAssignments(){
        return this.courses.values()
        		.stream()
        		.collect(Collectors.toMap(c -> c.getName(), c -> c.getEnrolled()
        				.stream()
        				.map(s -> s.getId())
        				.collect(Collectors.toList())));
    }
    
    
    /**
     * Adds a new notification listener for the announcements
     * issues by this course manager.
     * 
     * @param listener : the new notification listener
     */
    public void addNotifier(Notifier listener) {
        this.listeners.add(listener);
    }
    
    /**
     * Computes the success rate w.r.t. to first 
     * (second, third) choice.
     * 
     * @param choice : the number of choice to consider.
     * @return the success rate (number between 0.0 and 1.0)
     */
    public double successRate(int choice){
        return this.students.values()
        		.stream()
        		.filter(s -> s.isEnrolled(choice))
        		.count()/(double) this.students.size();
    }

    
    /**
     * Returns the students not assigned to any course.
     * 
     * @return the student id list.
     */
    public List<String> getNotAssigned(){
        return this.students.values()
        		.stream()
        		.filter(s -> s.getEnrolledIn() == null)
        		.map(s -> s.getId())
        		.collect(Collectors.toList());
    }
    
    
}
