package it.polito.oop.books;

import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Topic implements Comparable<Topic> {
	
	/*
	 * ATTRIBUTES
	 */
	private String keyword;
	private Map<String, Topic> subtopics;

	/**
	 * CONSTRUCTOR
	 * 
	 * @param keyword
	 */
	public Topic(String keyword) {
		this.keyword = keyword;
		this.subtopics = new LinkedHashMap<String, Topic>();
	}
	
	/**
	 * GETTERS of keyword
	 * 
	 * @return String the keyword
	 */
	public String getKeyword() {
        return this.keyword;
	}
	
	@Override
	public String toString() {
	    return this.keyword;
	}

	/**
	 * Add a subtopic to the current topic. it returns TRUE 
	 * if the subtopic was added or FALSE if the subtopic was not 
	 * added because already listed as a subtopic.
	 * 
	 * @param topic
	 * @return
	 */
	public boolean addSubTopic(Topic topic) {
        
		if (this.subtopics.containsValue(topic))
			return false;
		
		this.subtopics.put(topic.getKeyword(), topic);
		return true;
		
	}

	/**
	 * Returns a sorted list of subtopics. Topics in the list *MAY* be modified without
	 * affecting any of the Book topic.
	 * 
	 * @param void
	 * @return List containing all the subtopics
	 */
	public List<Topic> getSubTopics() {
        return this.subtopics.values()
        		.stream()
        		.sorted()
        		.collect(Collectors.toList());
	}

	@Override
	public int compareTo(Topic o) {
		return this.keyword.compareTo(o.getKeyword());
	}
}
