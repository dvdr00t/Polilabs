package it.polito.oop.elective;

import java.util.LinkedList;
import java.util.List;

public class Student {
	
	/*
	 * ATTRIBUTES
	 */
	private String id;
	private double grade;
	private Course enrolledIn;
	private List<Course> preferences;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param id
	 * @param grade
	 */
	public Student(String id, double grade) {
		this.id = id;
		this.grade = grade;
		this.preferences = new LinkedList<Course>();
	}

	/*
	 * GETTERS
	 */
	public String getId() {
		return id;
	}
	public double getGrade() {
		return grade;
	}
	public Course getEnrolledIn() {
		return enrolledIn;
	}
	public void setEnrolledIn(Course enrolledIn) {
		this.enrolledIn = enrolledIn;
	}

	/**
	 * Add all the courses selected by the student to this student courses select
	 * 
	 * @param toBeAdded
	 */
	public void addCourses(List<Course> toBeAdded) {
		this.preferences.addAll(toBeAdded);
	}
	
	/**
	 * Get all the courses selected by this student
	 * 
	 * @return List<Course> list of the preferences selected by this student
	 */
	public List<Course> getPreferences() {
		return new LinkedList<Course>(this.preferences); 
				
	}
	
	/**
	 * Return true if the student is enrolled to its "value" choice
	 * 
	 * @param value
	 * @return
	 */
	public boolean isEnrolled (int value) {
		if (this.enrolledIn != null)
			if (value <= this.preferences.size())
				if (this.preferences.get(value-1) == this.enrolledIn)
					return true;
		return false;
	}
	
    public int choiceNo(String course) {
        for(int i=0; i<preferences.size(); ++i) {
            if(preferences.get(i).getName().equals(course)) return i+1;
        }
        return -1;
    }
}
