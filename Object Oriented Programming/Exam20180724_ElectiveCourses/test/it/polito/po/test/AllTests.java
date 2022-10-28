package it.polito.po.test;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ 
    TestR1_Courses.class, 
    TestR2_Preferences.class, 
    TestR3_Classes.class, 
    TestR4_Notifiche.class,
    TestR5_Stats.class 
    })
public class AllTests {
}
