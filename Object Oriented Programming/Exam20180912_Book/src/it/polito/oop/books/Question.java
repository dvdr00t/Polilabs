package it.polito.oop.books;

import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class Question {
	
	/*
	 * ATTRIBUTES
	 */
	private String text;
	private Topic mainTopic;
	private List<Answer> answers;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param text
	 * @param mainTopic
	 */
	public Question(String text, Topic mainTopic) {
		this.text = text;
		this.mainTopic = mainTopic;
		this.answers = new LinkedList<Answer>();
	}
	
	/**
	 * GETTER for question text
	 * 
	 * @return
	 */
	public String getQuestion() {
		return this.text;
	}
	
	/**
	 * GETTER for main topic
	 * 
	 * @return
	 */
	public Topic getMainTopic() {
		return this.mainTopic;
	}

	/**
	 * Add an answer to the set of answers for the current
	 * question.
	 * 
	 * @param answer
	 * @param correct
	 */
	public void addAnswer(String answer, boolean correct) {
		this.answers.add(new Answer(answer, correct));
	}
	
    @Override
    public String toString() {
        return this.text + " (" + this.mainTopic + ")";
    }
    
    /**
     * Get the number of answers added for this question.
     * 
     * @return
     */
	public long numAnswers() {
	    return this.answers.size();
	}
	
	/**
	 * Get all the correct answers for this question. 
	 * 
	 * @return Set<String>
	 */
	public Set<String> getCorrectAnswers() {
		return this.answers.stream()
				.filter(a -> a.isCorrect() == true)
				.map(a -> a.getText())
				.collect(Collectors.toSet());
	}
	
	/**
	 * Get all the incorrect answers for this question. 
	 * 
	 * @return Set<String>
	 */
	public Set<String> getIncorrectAnswers() {
		return this.answers.stream()
				.filter(a -> a.isCorrect() == false)
				.map(a -> a.getText())
				.collect(Collectors.toSet());
	}
	
	/**
	 * Given a set of answer, it counts how many of them are correct
	 * 
	 * @param answers
	 * @return
	 */
	public int compareAnswers(List<String> answers) {
		int count = 0;
		for (String s: answers)
			for (Answer a: this.answers)
				if (s.equals(a.getText()) && a.isCorrect())
					++count;
		return count;
	}
}
