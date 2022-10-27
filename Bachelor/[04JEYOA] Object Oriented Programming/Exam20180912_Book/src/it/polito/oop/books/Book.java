package it.polito.oop.books;

import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

public class Book {

	/*
	 * ATTRIBUTES
	 */
	private Map<String, Topic> topics;
	private List<Question> questions;
	private List<Chapter> chapters;

	/**
	 * CONSTRUCTOR
	 *
	 * @param void
	 * @return Book
	 */
	public Book() {
		this.topics = new LinkedHashMap<String, Topic>();
		this.questions = new LinkedList<Question>();
		this.chapters = new LinkedList<Chapter>();
	}


    /**
	 * Creates a new topic, if it does not exist yet, or returns a reference to the
	 * corresponding topic.
	 *
	 * @param keyword the unique keyword of the topic
	 * @return the {@link Topic} associated to the keyword
	 * @throws BookException
	 */
	public Topic getTopic(String keyword) throws BookException {

		//Throwing an exception is the keyrod is either null or empty
		if (keyword == null || keyword.equals(""))
			throw new BookException();

		if(this.topics.containsKey(keyword))
			return this.topics.get(keyword);

		Topic newTopic = new Topic(keyword);
		this.topics.put(keyword, newTopic);

		return newTopic;
	}

	/**
	 * Create a new question.
	 *
	 * @param question
	 * @param mainTopic
	 * @return Question the instance of the new question created
	 */
	public Question createQuestion(String question, Topic mainTopic) {
		Question q = new Question(question, mainTopic);
		this.questions.add(q);
        return q;
	}

	/**
	 * Create a new Theory Chapter
	 *
	 * @param title
	 * @param numPages
	 * @param text
	 * @return TheoryChapter the new theory chapter
	 */
	public TheoryChapter createTheoryChapter(String title, int numPages, String text) {
        TheoryChapter tc = new TheoryChapter(title, numPages, text);
        this.chapters.add(tc);
        return tc;
	}

	/**
	 * Create a new Exercise Chapter
	 *
	 * @param title
	 * @param numPages
	 * @return ExerciseChapter the new exercise chapter
	 */
	public ExerciseChapter createExerciseChapter(String title, int numPages) {
        ExerciseChapter ec = new ExerciseChapter(title, numPages);
        this.chapters.add(ec);
        return ec;
	}


	/**
	 * Get all the topics, both in exercise and theory chapter, sorted in
	 * alphabetical order and with no repetition.
	 *
	 * @return List of topics
	 */
	public List<Topic> getAllTopics() {
        return this.topics.values()
        		.stream()
        		.distinct()
        		.sorted()
        		.collect(Collectors.toList());
	}

	/**
	 * return true if all the topics specified in all exercise chapters
	 * are contained in at least one theory chapter.
	 *
	 * @return Boolean
	 */
	public boolean checkTopics() {

		List<Topic> exerciseTopics = new LinkedList<Topic>();
		List<Topic> theoryTopics = new LinkedList<Topic>();
		for (Chapter c: this.chapters) {
			if (c instanceof ExerciseChapter)
				exerciseTopics.addAll(c.getTopics());
			else
				theoryTopics.addAll(c.getTopics());
		}

		if (!theoryTopics.containsAll(exerciseTopics))
			return false;
		return true;
	}

    /**
     * builds a map having as key the number of answers and
     * as values the list of questions having that number of answers.
     * @return
     */
    public Map<Long,List<Question>> questionOptions(){
    	return this.questions.stream()
    			.collect(Collectors.groupingBy(q -> q.numAnswers(), Collectors.toList()));
    }

    /**
     * Create a new assignment.
     *
     * @param ID
     * @param chapter
     * @return Assignment the new assignment
     */
	public Assignment newAssignment(String ID, ExerciseChapter chapter) {
        return new Assignment(ID, chapter);
	}
}
