package it.polito.oop.books;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;


public class ExerciseChapter extends Chapter {

	/*
	 * ATTRIBUTES
	 */
	private List<Question> chapterQuestions;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param title
	 * @param numPages
	 */
    public ExerciseChapter(String title, int numPages) {
    	this.setTitle(title);
    	this.setNumPages(numPages);
    	this.chapterQuestions = new LinkedList<Question>();
	}
	
	/*
	 * GETTER and SETTER for this chapter
	 */
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
     * Add a question to the current chapter
     * 
     * @param question
     */
	public void addQuestion(Question question) {
		this.chapterQuestions.add(question);
	}	
	
	/**
	 * Get all the topics associated with the questions in this chapter,
	 * sorted in alphabetical order and without repetition.
	 * 
	 * @return List of topic
	 */
	public List<Topic> getTopics() {
        return this.chapterQuestions.stream()
        		.map(q -> q.getMainTopic())
        		.distinct()
        		.sorted()
        		.collect(Collectors.toList());
	};
}
