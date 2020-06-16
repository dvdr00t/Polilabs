package it.polito.oop.books.test;

import it.polito.oop.books.*;

import static org.junit.Assert.*;
import org.junit.Test;

import java.util.Arrays;

public class TestR1 {

    @Test
    public void testTopics() throws BookException {
        Book book = new Book();
        Topic Foo = book.getTopic("Foo");
        Topic foo = book.getTopic("foo");
        Topic Bar = book.getTopic("Bar");
        Topic Foo2 = book.getTopic("Foo");

        assertNotSame("Different keywords", Foo, Bar);
        assertNotSame("Different keywords", Foo, foo);
        assertSame("Same keywords", Foo, Foo2);
    }

    @Test
    public void testTopicKeywords() throws BookException {
        Book book = new Book();
        Topic Foo = book.getTopic("Foo");

        assertNotNull("Missing topic",Foo);
        assertEquals(Foo.getKeyword(), "Foo");
        assertEquals(Foo.getKeyword(), Foo.toString());
    }

    @Test
    public void testSubTopics() throws BookException {
        Book book = new Book();
        Topic foo = book.getTopic("Foo");
        Topic bar = book.getTopic("Bar");

        assertNotNull("Missing topic",foo);
        foo.addSubTopic(book.getTopic("sub2"));
        foo.addSubTopic(book.getTopic("sub1"));
        foo.addSubTopic(book.getTopic("sub1"));
        foo.addSubTopic(book.getTopic("sub2"));

        assertNotNull("Missing sub topics of Foo",foo.getSubTopics());
        assertEquals(Arrays.asList(book.getTopic("sub1"), book.getTopic("sub2")), foo.getSubTopics());
        assertFalse(bar.getSubTopics().contains(book.getTopic("sub1")));
        assertFalse(bar.getSubTopics().contains(book.getTopic("sub2")));
    }

    @Test(expected = BookException.class)
    public void testTopicKeywordsNull() throws BookException {
        Book book = new Book();
        Topic t = book.getTopic(null);
        assertNull(t);
    }

    @Test(expected = BookException.class)
    public void testTopicKeywordsEmpty() throws BookException {
        Book book = new Book();
        Topic t = book.getTopic("");
        assertNull(t);
    }

}