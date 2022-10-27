package it.polito.po.test;

import org.junit.runner.RunWith;
import org.junit.runners.Suite.SuiteClasses;
import org.junit.runners.Suite;

@RunWith(Suite.class)
@SuiteClasses({ 
    TestReq1.class, TestReq2.class, TestReq3.class, TestReq4.class
})
public class AllTests { }
