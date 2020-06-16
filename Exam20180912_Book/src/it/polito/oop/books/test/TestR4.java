package it.polito.oop.books.test;

import it.polito.oop.books.*;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

public class TestR4 {
    Book book;
    Question q11;
    Question q121;
    Question q2;

    TheoryChapter tc1;
    TheoryChapter tc2;
    ExerciseChapter ec;

    @Before
    public void setUp() throws BookException {
        book = new Book();

        book.getTopic("1").addSubTopic(book.getTopic("1.1"));
        book.getTopic("1").addSubTopic(book.getTopic("1.2"));
        book.getTopic("1.2").addSubTopic(book.getTopic("1.2.1"));
        book.getTopic("2").addSubTopic(book.getTopic("2.1"));
        book.getTopic("2").addSubTopic(book.getTopic("2.2"));

        q121 = book.createQuestion("q121", book.getTopic("1.2.1"));
        q11 = book.createQuestion("q11", book.getTopic("1.1"));
        q2 = book.createQuestion("q2", book.getTopic("2.1"));

        tc1 = book.createTheoryChapter("Theory", 42, "Lorem ipsum");
        tc2 = book.createTheoryChapter("Theory2", 99, "Lorem ipsum");
        ec = book.createExerciseChapter("Excercise", 17);

        tc1.addTopic(book.getTopic("1"));

        ec.addQuestion(q121);
        ec.addQuestion(q11);

    }

    @Test
    public void testCheckChapters() throws BookException {

        assertTrue(book.checkTopics());

        ec.addQuestion(q2);
        assertFalse(book.checkTopics());

        tc2.addTopic(book.getTopic("2"));
        assertTrue(book.checkTopics());
    }
    
    <T> List<String> b2s(List<T> l){
        return l==null?null:l.stream().map(T::toString).collect(Collectors.toList());
    }
    <T> List<String> b2s(List<T> l,Function<T,String> extractor){
        return l==null?null:l.stream().map(extractor).collect(Collectors.toList());
    }


    @Test
    public void testAllTopics() throws BookException {
        tc2.addTopic(book.getTopic("2"));

        assertEquals(Arrays.asList("1","1.1","1.2","1.2.1","2","2.1","2.2"),
                     b2s(book.getAllTopics()));
        assertFalse(b2s(book.getAllTopics()).contains("3"));
    }

    @Test
    public void testQuestionOptions() throws BookException {
        tc2.addTopic(book.getTopic("2"));
        ec.addQuestion(q2);

        Map<Long, Set<Question>> good_result = new HashMap<>();
        good_result.put(0L, new HashSet<>(Arrays.asList(q11, q121, q2)));

        Map<Long, Set<Question>> cooked_result = new HashMap<>();
        Map<Long, List<Question>> raw_result = book.questionOptions();

        for (Long i:raw_result.keySet()) {
            cooked_result.put(i, new HashSet<>(raw_result.get(i)));
        }
        assertEquals(good_result, cooked_result);

        good_result.clear();
        q11.addAnswer("a1", true);
        q11.addAnswer("a2", false);
        q11.addAnswer("a3", false);
        good_result.put(3L, new HashSet<>(Arrays.asList(q11)));
        q121.addAnswer("b1", true);
        q121.addAnswer("b2", false);
        q2.addAnswer("c1", true);
        q2.addAnswer("c2", false);
        good_result.put(2L, new HashSet<>(Arrays.asList(q2, q121)));
        cooked_result.clear();
        raw_result = book.questionOptions();
        for (Long i:raw_result.keySet()) {
            cooked_result.put(i, new HashSet<>(raw_result.get(i)));
        }
        assertEquals(good_result, cooked_result);
    }

}