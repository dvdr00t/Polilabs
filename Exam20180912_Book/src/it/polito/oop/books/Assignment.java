package it.polito.oop.books;

import java.util.List;


public class Assignment {
	
	/*
	 * ATTRIBUTES
	 */
	private String ID;
	private ExerciseChapter exerciseChapter;
	private double totalScore;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param ID
	 * @param exerciseChapter
	 */
    public Assignment(String ID, ExerciseChapter exerciseChapter) {
		this.ID = ID;
		this.exerciseChapter = exerciseChapter;
	}

    /*
     * GETTERS for the assignment
     */
	public String getID() {
        return this.ID;
    }
    public ExerciseChapter getChapter() {
        return this.exerciseChapter;
    }
    
    /**
     * Add the student response to the question
     * 
     * @param q
     * @param answers
     * @return the value obtained
     */
    public double addResponse(Question q, List<String> answers) {
    	double N = q.numAnswers();
    	double FP = answers.size() - q.compareAnswers(answers);
    	double FN = q.getCorrectAnswers().size() - q.compareAnswers(answers);
    	double points = (N - FP - FN) / N;
    	
    	this.totalScore = this.totalScore + points;
    	
    	return points;
    }
    
    public double totalScore() {
        return this.totalScore;
        
    }

}
