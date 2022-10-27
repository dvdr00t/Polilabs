package it.polito.po.test;

import junit.framework.Test;
import junit.framework.TestSuite;

public class AllTests {

    public static void main(String[] args) {
        junit.textui.TestRunner.run(suite());
    }

    public static Test suite() {
        TestSuite suite = new TestSuite(AllTests.class.getName());
        //$JUnit-BEGIN$
        suite.addTestSuite(TestReq1.class);
        suite.addTestSuite(TestReq2.class);
        suite.addTestSuite(TestReq3.class);
        suite.addTestSuite(TestReq4.class);
        suite.addTestSuite(TestReq5.class);
        //$JUnit-END$
        return suite;
    }
}
