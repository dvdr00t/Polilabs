package it.polito.oop.books;

public class Answer {

	/*
	 * ATTRIBUTES
	 */
	private String text;
	private boolean correct;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param text
	 * @param result
	 */
	public Answer(String text, boolean correct) {
		this.text = text;
		this.correct = correct;
	}

	public String getText() {
		return text;
	}

	public boolean isCorrect() {
		return correct;
	}
	
	
	
	
}
