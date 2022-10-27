package main;

import java.util.Arrays;

import it.polito.oop.elective.ElectiveException;
import it.polito.oop.elective.ElectiveManager;

public class Example {

    public static void main(String[] args) throws ElectiveException {
        
        ElectiveManager electiveCourses = new ElectiveManager();
        electiveCourses.addNotifier(new ActivityLogger());

        electiveCourses.addCourse("VIQ", 3);
        electiveCourses.addCourse("AI", 1);
        electiveCourses.addCourse("OR", 1);
        
        System.out.println("Elective courses: " + electiveCourses.getCourses());
        
        electiveCourses.loadStudent("S1", 25.5);
        electiveCourses.loadStudent("S2", 27);
        electiveCourses.loadStudent("S3", 25.5);
        electiveCourses.loadStudent("S4", 28);
        electiveCourses.loadStudent("S5", 22);
        
        System.out.println("Students: " + electiveCourses.getStudents()); 
        System.out.println("Best students: " + electiveCourses.getStudents(27.0,30.0)); // [S2,S4]
        
        electiveCourses.requestEnroll("S1",Arrays.asList("VIQ","AI"));
        electiveCourses.requestEnroll("S2",Arrays.asList("VIQ","OR"));
        electiveCourses.requestEnroll("S3",Arrays.asList("AI","OR"));
        electiveCourses.requestEnroll("S4",Arrays.asList("AI","VIQ"));
        electiveCourses.requestEnroll("S5",Arrays.asList("AI","OR"));

        try {
            electiveCourses.addCourse("XX", 1);
            electiveCourses.requestEnroll("S7",Arrays.asList("AI","VIQ","OR","XX"));
        }catch(ElectiveException e) {
            System.out.println("Detected wrong enrollment"); // this is expected to be printed
        }
        
        System.out.println("Selections: " + electiveCourses.numberRequests() );
        
        
        long unsatisfied = electiveCourses.makeClasses();
                
        System.out.println("Not satisfied: " + unsatisfied);  // 1

        double success = electiveCourses.successRate(1);
        System.out.println("Success rate for first choice: " + success*100); // 60

        System.out.println("Classes: " + electiveCourses.getAssignments());  //  {XX=[], OR=[S3], AI=[S4], VIQ=[S2, S1]}

        System.out.println("Not assigned: " + electiveCourses.getNotAssigned());  // [S5]
        
    }    
}
