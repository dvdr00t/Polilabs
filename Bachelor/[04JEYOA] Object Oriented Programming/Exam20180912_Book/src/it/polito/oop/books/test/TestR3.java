package it.polito.oop.books.test;

import it.polito.oop.books.*;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;
import java.util.List;

public class TestR3 {

    Book book;
    Question q1;
    Question q2;

    @Before
    public void setUp() throws BookException {
        book = new Book();

        book.getTopic("1").addSubTopic(book.getTopic("1.1"));
        book.getTopic("1").addSubTopic(book.getTopic("1.2"));
        book.getTopic("1.2").addSubTopic(book.getTopic("1.2.1"));

        q1 = book.createQuestion("q121", book.getTopic("1.2.1"));
        q2 = book.createQuestion("q11", book.getTopic("1.1"));
    }

    @Test
    public void testTheoryChapter() throws BookException {
        TheoryChapter tc = book.createTheoryChapter("Theory", 42, "Lorem ipsum");

        assertEquals(tc.getNumPages(), 42);
        tc.setNumPages(7);
        assertEquals(tc.getNumPages(), 7);

        assertEquals(tc.getTitle(), "Theory");
        tc.setTitle("Theory2");
        assertEquals(tc.getTitle(), "Theory2");

        assertEquals(tc.getText(), "Lorem ipsum");
        tc.setText("");
        assertEquals(tc.getText(), "");

    }

    @Test
    public void testTheoryChapterTopics() throws BookException {
        TheoryChapter tc = book.createTheoryChapter("Theory", 42, "Lorem ipsum");

        tc.addTopic(book.getTopic("1"));
        List<Topic> expectedTopics = Arrays.asList(book.getTopic("1"), book.getTopic("1.1"), book.getTopic("1.2"), book.getTopic("1.2.1"));
        
        assertEquals(expectedTopics,tc.getTopics());
        tc.addTopic(book.getTopic("1.2"));

        assertEquals(expectedTopics,tc.getTopics());
    }

    @Test
    public void testTheoryChapterTopicsNoDups() throws BookException {
        TheoryChapter tc = book.createTheoryChapter("Theory", 42, "Lorem ipsum");

        tc.addTopic(book.getTopic("1"));
        tc.addTopic(book.getTopic("1.2"));

        List<Topic> expectedTopics = Arrays.asList(book.getTopic("1"), book.getTopic("1.1"), book.getTopic("1.2"), book.getTopic("1.2.1"));
        assertEquals(expectedTopics,tc.getTopics());

    }

    @Test
    public void testExerciseChapter() throws BookException {
        ExerciseChapter ec = book.createExerciseChapter("Excercise", 17);

        ec.addQuestion(q1);
        ec.addQuestion(q2);

        assertEquals(Arrays.asList(book.getTopic("1.1"), book.getTopic("1.2.1")), ec.getTopics());
    }

}