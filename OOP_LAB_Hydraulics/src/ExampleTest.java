import hydraulic.*;

import static org.junit.Assert.*;
import org.junit.Test;

/**
 * 
 * This class contains a basic test case that can be used to detect 
 * the most obvious defects.
 *
 */
public class ExampleTest {
	
	@Test
	public void testAll(){
		
		HSystem s = new HSystem();
		
		assertNotNull("Apparently not implemented yet",s.getElements());
		assertEquals("Initially no elements are present in the system",0,s.getElements().length);
	
		// 1) the elements of the system are defined and added
		Source src = new Source("Src");
		s.addElement(src);
		Tap r = new Tap("R");
		s.addElement(r);
		Split t = new Split("T");
		s.addElement(t);
		Sink sink1 = new Sink("sink A");
		s.addElement(sink1);
		Sink sink2 = new Sink("sink B");
		s.addElement(sink2);

		assertEquals("Src",src.getName());
		assertEquals("sink B",sink2.getName());
		assertArrayEquals(new Element[] {src,r,t,sink1,sink2}, s.getElements());
		

		
		// 2) elements are then connected
		src.connect(r);
		r.connect(t);
		t.connect(sink1,0);
		t.connect(sink2,1);
		
		assertSame("Output of src should be r",r,src.getOutput());
		
		// 3) simulation parameters are then defined
		src.setFlow(20);
		r.setOpen(true);
		
		// 4) simulation starts
		PrintingObserver obs = new PrintingObserver();
		s.simulate(obs);
		assertEquals("Expected 5 notifications",5,obs.getCount());
	}
}
