import hydraulic.*;

import static org.junit.Assert.*;
import org.junit.Test;

public class ExampleTestExt {
	
	@Test
	public void testAll(){
		
		HSystemExt s = new HSystemExt();
		
		assertNotNull("Apparently not implemented yet",s.getElements());
		assertEquals("Initially no elements are present in the system",0,s.getElements().length);
	
		// 1) the elements of the system are defiend and added
		Source src = new Source("Src");
		s.addElement(src);
		Tap r = new Tap("R");
		s.addElement(r);
		Multisplit t = new Multisplit("MS",3);
		s.addElement(t);
		Sink sink1 = new Sink("sink A");
		s.addElement(sink1);
		Sink sink2 = new Sink("sink B");
		s.addElement(sink2);
		Sink sink3 = new Sink("sink C");
		s.addElement(sink3);

		assertEquals("Src",src.getName());
		assertEquals("sink B",sink2.getName());
		assertArrayEquals(new Element[] {src,r,t,sink1,sink2,sink3}, s.getElements());
		
		// 2) elements are then connected
		src.connect(r);
		r.connect(t);
		t.connect(sink1,0);
		t.connect(sink2,1);
		t.connect(sink3,2);
		
		assertSame("Output of src should be r",r,src.getOutput());
		
		// 3) simulation parameters are then defined
		src.setFlow(20);
		r.setOpen(true);
		t.setProportions(.25,.35,.40);
		
		// 4) simulation starts
		PrintingObserverExt obs = new PrintingObserverExt();
		s.simulate(obs);
		assertEquals("Expected 5 notifications",6,obs.getCount());
		
		// 5) print the system layout
		System.out.println(s.layout());
		
		// 6) delete the tap
		s.deleteElement("R");
		assertSame("Output of src should be t",t,src.getOutput());
		System.out.println(s.layout());
		
		// 7) simulate with check of simulation parameters against 
		//	  the maximum flow rate of elements
		// WARNING: first make elements classes extend ElementExt, then uncomment the following lines
//		t.setMaxFlow(20);
//		sink1.setMaxFlow(10);
//		sink2.setMaxFlow(15);
//		sink3.setMaxFlow(5); // should raise error message, inFlow 8.0 but maxFlow 5.0 
		s.simulate(obs,true);
	}
}
