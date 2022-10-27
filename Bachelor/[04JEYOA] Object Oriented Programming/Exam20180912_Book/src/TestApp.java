import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import java.util.stream.Collectors;

import org.junit.Test;

import it.polito.oop.books.*;

public final class TestApp {

    @Test
    public void test() throws BookException {
        Book oop = new Book();
//R1
        assertNotNull(oop.getTopic("Java"));
        oop.getTopic("Java").addSubTopic(oop.getTopic("Classes"));
        oop.getTopic("Java").addSubTopic(oop.getTopic("Lambda expressions"));
        oop.getTopic("Java").addSubTopic(oop.getTopic("Generics"));
        oop.getTopic("Classes").addSubTopic(oop.getTopic("Inheritance"));
        oop.getTopic("Classes").addSubTopic(oop.getTopic("Interfaces"));
        oop.getTopic("Classes").addSubTopic(oop.getTopic("Abstract classes"));
        oop.getTopic("Interfaces").addSubTopic(oop.getTopic("Functional Interfaces"));
             
        assertNotNull(oop.getTopic("Java"));
        assertEquals("Java",oop.getTopic("Java").getKeyword());
        
        assertNotNull(oop.getTopic("Java").getSubTopics());
        assertEquals(Arrays.asList("Classes", "Generics", "Lambda expressions"),
                oop.getTopic("Java").getSubTopics().stream().map(Topic::getKeyword).collect(Collectors.toList()));
        
//R2
        Question q1;
        q1 = oop.createQuestion("What is Java", oop.getTopic("Java"));
        assertNotNull(q1);
        q1.addAnswer("A 1958 song by Allen Toussaint", false);
        q1.addAnswer("A brand of Russian cigarettes", false);
        q1.addAnswer("A breed of chicken from the United States", false);
        q1.addAnswer("A British merchant and migrant ship (active: 1813-1939)", false);
        q1.addAnswer("A slang term for coffee", true);
        q1.addAnswer("A villain appearing in the DC Comics series Metamorpho", true);

        assertEquals(new TreeSet<>(Arrays.asList("A villain appearing in the DC Comics series Metamorpho", 
                "A slang term for coffee")),q1.getCorrectAnswers());

        assertEquals(new TreeSet<>(Arrays.asList("A 1958 song by Allen Toussaint",
                        "A brand of Russian cigarettes",
                        "A breed of chicken from the United States",
                        "A British merchant and migrant ship (active: 1813-1939)")),
                    q1.getIncorrectAnswers());
        
        assertEquals(6,q1.numAnswers());
    
//R3        
        TheoryChapter theory_1 = oop.createTheoryChapter("Java Fundamentals", 42,
                "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        theory_1.addTopic(oop.getTopic("Java"));

        assertEquals("Java Fundamentals",theory_1.getTitle());
        assertNotNull(theory_1.getTopics());
        assertEquals(Arrays.asList( "Abstract classes",
                                    "Classes",
                                    "Functional Interfaces", 
                                    "Generics",
                                    "Inheritance",
                                    "Interfaces",
                                    "Java",
                                    "Lambda expressions"),
                    theory_1.getTopics().stream().map(Topic::getKeyword).collect(Collectors.toList()));

        
        ExerciseChapter ex_1 = oop.createExerciseChapter("Problems", 42);
        ex_1.addQuestion(q1);

        Question q2;
        q2 = oop.createQuestion("What are Functional Interfaces", oop.getTopic("Functional Interfaces"));
        q2.addAnswer("Foo", true);
        q2.addAnswer("Bar", false);
        ex_1.addQuestion(q2);
        
        Question q2b;
        q2b = oop.createQuestion("What are Functional Interfaces", oop.getTopic("Functional Interfaces"));
        q2b.addAnswer("Foo", true);
        q2b.addAnswer("Bar", false);
        ex_1.addQuestion(q2b);

        assertNotNull(ex_1.getTopics());
        assertEquals(Arrays.asList( "Functional Interfaces", 
                                    "Java"),
                    ex_1.getTopics().stream().map(Topic::getKeyword).collect(Collectors.toList()));


//R4
        assertNotNull(oop.getAllTopics());
        assertEquals(Arrays.asList( "Abstract classes",
                                    "Classes",
                                    "Functional Interfaces", 
                                    "Generics",
                                    "Inheritance",
                                    "Interfaces",
                                    "Java",
                                    "Lambda expressions"),
                    oop.getAllTopics().stream().map(Topic::getKeyword).collect(Collectors.toList()));


        assertTrue(oop.checkTopics());
        
        
        Question qx = oop.createQuestion("Foo", oop.getTopic("Zap"));
        ex_1.addQuestion(qx);
        System.out.println(oop.checkTopics()); // false
        assertFalse(oop.checkTopics());

        Map<Long,List<Question>> qo = oop.questionOptions();
        assertNotNull(qo);
        assertTrue(qo.keySet().containsAll(Arrays.asList(0L,2L,6L)));
        assertEquals("What is Java",qo.get(6L).get(0).getQuestion());

//R5        
       
        Assignment e = oop.newAssignment("s123456", ex_1);
        ArrayList<String> response = new ArrayList<>();
        response.add("A slang term for coffee");
        double score = e.addResponse(q1, response);  
        assertEquals(0.8333,score,0.0001);

        response = new ArrayList<>();
        response.add("Foo");
        score = e.addResponse(q2, response);
        assertEquals(1.0,score,0.0001);

        assertEquals(1.8333,e.totalScore(),0.0001);
    }
}
