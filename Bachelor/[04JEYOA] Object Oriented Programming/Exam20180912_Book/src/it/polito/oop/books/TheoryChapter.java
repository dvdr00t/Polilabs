package it.polito.oop.books;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collector;
import java.util.stream.Collectors;


public class TheoryChapter extends Chapter{
	
	/*
	 * ATTRIBUTES
	 */
	private String text;
	private List<Topic> chapterTopics;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param title
	 * @param numPages
	 * @param text
	 */
	public TheoryChapter (String title, int numPages, String text) {
		this.setTitle(title);
		this.setNumPages(numPages);
		this.text = text;
		this.chapterTopics = new LinkedList<Topic>();
	}
	

    
    /*
     * GETTERS and SETTER for the chapter
     */
	public String getText() {
		return this.text;
	}
    public void setText(String newText) {
    	this.text = newText;
    }
    public String getTitle() {
        return super.getTitle();
    }
    public void setTitle(String newTitle) {
    	super.setTitle(newTitle);
    }
    public int getNumPages() {
        return super.getNumPages();
    }
    public void setNumPages(int newPages) {
    	super.setNumPages(newPages);
    }
    
    
    /**
     * Add a particular topic (and all its subtopics) to this chapter.
     * 
     * @param topic
     */
    public void addTopic(Topic topic) {
    	this.chapterTopics.add(topic);
    	for (Topic t: topic.getSubTopics())
    		addTopic(t);
    }
    
    /**
     * Get all the topics of this chapter without repetition and sorted in 
     * alphabetical order
     * 
     * @return
     */
	public List<Topic> getTopics() {
        return this.chapterTopics.stream()
        		.distinct()
        		.sorted()
        		.collect(Collectors.toList());
	}
    
}
