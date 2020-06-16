package it.polito.oop.books.test;

import it.polito.oop.books.*;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;
import java.util.Arrays;
import java.util.HashSet;
import java.util.regex.Pattern;


public class TestR2 {

    private Book book;
    private Question q1;
    private Question q2;

    @Before
    public void setUp() throws BookException {
        book = new Book();
        q1 = book.createQuestion("q1?", book.getTopic("topic_q1"));
        q2 = book.createQuestion("q2?", book.getTopic("topic_q2"));
    }
    
	@Test
	public void testCreateQuestions() throws BookException {
			
            assertNotNull("Missing question",q1);

			assertEquals(q1.getQuestion(), "q1?");
			assertEquals(q2.getQuestion(), "q2?");
			assertEquals(q1.getMainTopic(), book.getTopic("topic_q1"));
			assertEquals(q2.getMainTopic(), book.getTopic("topic_q2"));
	}

    @Test
    public void testQuestionToString() throws BookException {
            
       String s = q1.toString();
       
       assertTrue("Question toString does not match expected pattern",
                   Pattern.matches("q1\\? *\\( *topic_q1 *\\)", s));
    }

    @Test
    public void testAnswers() throws BookException {
            
            q1.addAnswer("Correct", true);
            q1.addAnswer("Not correct", false);
            q1.addAnswer("Also incorrect", false);
            q1.addAnswer("Also correct", true);
            
            assertEquals(new HashSet<>(Arrays.asList("Also correct", "Correct")),q1.getCorrectAnswers());
            assertEquals(new HashSet<>(Arrays.asList("Also incorrect", "Not correct")),q1.getIncorrectAnswers());
    }

    @Test
    public void testNumAnswers() throws BookException {
            
            q1.addAnswer("Correct", true);
            q1.addAnswer("Not correct", false);
            q1.addAnswer("Also incorrect", false);
            q1.addAnswer("Also correct", true);
            
            assertEquals("Wrong number of answers",4,q1.numAnswers());
            assertEquals("Wrong number of answers",0,q2.numAnswers());            
    }
    
}