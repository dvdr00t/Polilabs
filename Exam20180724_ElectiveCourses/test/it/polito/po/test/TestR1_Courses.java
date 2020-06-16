package it.polito.po.test;

import static org.junit.Assert.*;

import java.util.Collection;
import java.util.Set;
import java.util.SortedSet;

import org.junit.Before;
import org.junit.Test;

import it.polito.oop.elective.ElectiveManager;

public class TestR1_Courses {
    
    private ElectiveManager manager;
    
    @Before
    public void setUp() {
        manager = new ElectiveManager();
    }

    @Test
    public void testAddCourse() {
        manager.addCourse("VIQ", 90);
        Set<String> courses = manager.getCourses();
        
        assertNotNull("Missing course set",courses);
        assertEquals("Missing course",1,courses.size());
    }

    @Test
    public void testGetCourses() {
        manager.addCourse("VIQ", 90);
        manager.addCourse("AI", 70);
        manager.addCourse("OR", 180);
        SortedSet<String> courses = manager.getCourses();
        
        assertNotNull("Missing course set",courses);
        assertEquals("Missing courses",3,courses.size());
        assertTrue("Missing course VIQ",courses.contains("VIQ"));
        assertTrue("Missing course AI",courses.contains("AI"));
        assertTrue("Missing course OR",courses.contains("OR"));
    }
    
    @Test
    public void testLoadStudent() {
        manager.loadStudent("S1", 29.9);
        
        Collection<String> students = manager.getStudents();
        
        assertNotNull("Missing student list",students);
        assertEquals("Missing student ",1,students.size());
    }

    @Test
    public void testGetStudent() {
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S33", 25.7);
        
        Collection<String> students = manager.getStudents();
        
        assertNotNull("Missing student list",students);
        assertEquals("Missing student ",3,students.size());
        assertTrue("Missing student S1",students.contains("S1"));
        assertTrue("Missing student S2",students.contains("S2"));
        assertTrue("Missing student S33",students.contains("S33"));
    }

    @Test
    public void testGetStudentInterval() {
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S33", 25.7);
        manager.loadStudent("S21", 21.3);
        manager.loadStudent("S5", 28.2);
        
        Collection<String> students = manager.getStudents(25,30);
        
        assertNotNull("Missing student list for interval",students);
        assertEquals("Missing students ",3,students.size());
        assertTrue("Missing student S1",students.contains("S1"));
        assertTrue("Missing student S33",students.contains("S33"));
        assertTrue("Missing student S5",students.contains("S5"));
    }

    @Test
    public void testGetStudentInterval2() {
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S33", 25);
        manager.loadStudent("S21", 21.3);
        manager.loadStudent("S5", 28.2);
        
        Collection<String> students = manager.getStudents(25,30);
        
        assertNotNull("Missing student list for interval",students);
        assertEquals("Missing students ",3,students.size());
        assertTrue("Missing student S1",students.contains("S1"));
        assertTrue("Missing student S33",students.contains("S33"));
        assertTrue("Missing student S5",students.contains("S5"));
    }

    @Test
    public void testGetStudentInterval3() {
        manager.loadStudent("S1", 29.9);
        manager.loadStudent("S2", 24.3);
        manager.loadStudent("S33", 25);
        manager.loadStudent("S21", 21.3);
        manager.loadStudent("S5", 28.2);
        
        Collection<String> students = manager.getStudents(18,21);
        
        assertNotNull("Missing student list for interval",students);
        assertEquals("Missing students ",0,students.size());
    }

}