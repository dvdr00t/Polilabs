package it.polito.oop.elective;

import java.util.LinkedList;
import java.util.List;

public class Course {
	
	/*
	 * ATTRIBUTES
	 */
	private String name;
	private int size;
	private List<Student> enrolled;
	
	
	/**
	 * CONSTUCTOR
	 * 
	 * @param name
	 * @param size
	 */
	public Course(String name, int size) {
		this.name = name;
		this.size = size;
		this.enrolled = new LinkedList<Student>();
	}


	/*
	 * GETTERS
	 */
	public String getName() {
		return name;
	}
	public int getSize() {
		return size;
	}	
	
	/**
	 * Add a student to he list of students enrolled for this courses
	 * 
	 * @param s
	 */
	public void addStudent(Student s) {
		this.enrolled.add(s);
		s.setEnrolledIn(this);
	}
	
	public List<Student> getEnrolled() {
		return new LinkedList<Student>(this.enrolled);
	}
	
	/**
	 * Checks if the course is full of students enrolled
	 * 
	 * @return boolean
	 */
	public boolean isFull() {
		if (this.enrolled.size() == this.size)
			return true;
		return false;
	}

}
