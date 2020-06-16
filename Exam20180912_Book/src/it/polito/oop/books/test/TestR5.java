package it.polito.oop.books.test;

import it.polito.oop.books.*;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import org.junit.Before;
import org.junit.Test;

public class TestR5 {

    Book book;
    TheoryChapter tc1;
    TheoryChapter tc2;
    ExerciseChapter ec;
    List<Question> qlist;

    @Before
    public void setUp() throws BookException {
        book = new Book();

        tc1 = book.createTheoryChapter("Theory", 42, "Lorem ipsum");
        tc2 = book.createTheoryChapter("Theory2", 99, "Lorem ipsum");
        ec = book.createExerciseChapter("Excercise", 17);

        tc1.addTopic(book.getTopic("1"));

        
        qlist = new LinkedList<>();
        long r = 5381;
        for (int i = 1; i < 10; ++i) {
            r = (r * 33 + i) % 999999;
            book.getTopic("t" + i / 10).addSubTopic(book.getTopic("t" + i / 2 + "." + (1 + r % 10)));
            Question q = book.createQuestion("q" + i, book.getTopic("t" + i / 2));

            q.addAnswer("a0", true);
            q.addAnswer("a1", false);
            for (int a = 0; a < r % 3; ++a)
                q.addAnswer("t" + a, true);
            for (int a = 0; a < r % 5; ++a)
                q.addAnswer("f" + a, false);

            qlist.add(q);
        }

        for (Question q : qlist) {
            ec.addQuestion(q);
            tc1.addTopic(q.getMainTopic());
        }


    }

    @Test
    public void testResponseScore() throws BookException {

            Assignment a = book.newAssignment("007", ec);
                
            Question q = qlist.get(0);
            double score;
            if(q.getCorrectAnswers().contains("t0")) {
                score = a.addResponse(q, Arrays.asList("a0", "t0"));
            }else {
                score = a.addResponse(q, Arrays.asList("a0"));
            }
            assertEquals("Question " + q.getQuestion() + " wrong score",1.0,score,0.001);

            q = qlist.get(1);
            if(q.getCorrectAnswers().contains("t0")) {
                score = a.addResponse(q, Arrays.asList("a0", "t0"));
            }else {
                score = a.addResponse(q, Arrays.asList("a0"));
            }
            assertEquals("Question " + q.getQuestion() + " wrong score",0.875,score,0.001);

            q = qlist.get(2);
            if(q.getCorrectAnswers().contains("t0")) {
                score = a.addResponse(q, Arrays.asList("a0", "t0"));
            }else {
                score = a.addResponse(q, Arrays.asList("a0"));
            }
            assertEquals("Question " + q.getQuestion() + " wrong score",1.0,score,0.001);
    }

    @Test
	public void testTotalScore() throws BookException {

			Assignment a = book.newAssignment("007", ec);
			for (Question q : qlist) {
	            if(q.getCorrectAnswers().contains("t0")) {
	                a.addResponse(q, Arrays.asList("a0", "t0"));
	            }else {
	                a.addResponse(q, Arrays.asList("a0"));
	            }
			}
			assertEquals("Wrong total score", 8.532, a.totalScore(), 0.001);
			Assignment a2 = book.newAssignment("008", ec);
			for (Question q : qlist) {
                if(q.getIncorrectAnswers().contains("f1")) {
                    a2.addResponse(q, Arrays.asList("a0", "a1", "f1"));
                }else {
                    a2.addResponse(q, Arrays.asList("a0", "a1"));
                }
			    
			}
			assertEquals("Wrong total score", 4.566, a2.totalScore(), 0.001);

	}

}