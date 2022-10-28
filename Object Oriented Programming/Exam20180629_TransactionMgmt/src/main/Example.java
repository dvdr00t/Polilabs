package main;
import java.util.*;

import transactions.*;
public class Example {

	static void print(Object obj) {System.out.println(obj);}

	public static void main(String[] args) throws TMException {
	TransactionManager tm = new TransactionManager();
	List<String> list1;
	
	//R1
	print("R1");
	list1 = tm.addRegion("reg1", "pl3", "pl1", "pl5");
		print(list1); //[pl1, pl3, pl5] 
	list1 = tm.addRegion("reg2", "pl4", "pl8", "pl6");
	list1= tm.addRegion("reg7", "pl4", "pl7", "pl7", "pl2");
		print(list1); //[pl2, pl7]
	tm.addCarrier("c4", "reg1", "reg7");
	tm.addCarrier("c5", "reg2", "reg1", "reg7");
	list1 = tm.addCarrier("c3", "reg2", "reg3", "reg2", "reg1");
		print(list1); //[reg1, reg2]
	list1 = tm.getCarriersForRegion("reg1");
		print(list1); //[c3, c4, c5]
		
	//R2
	print("R2");
	tm.addRequest("req1", "pl6", "product4"); tm.addOffer("o2", "pl1", "product4");
	tm.addRequest("req2", "pl5", "product8"); tm.addOffer("o3", "pl4", "product8");
	tm.addRequest("req4", "pl3", "product6"); tm.addOffer("o4", "pl5", "product6");
	tm.addOffer("o5", "pl3", "product6");
		try{tm.addOffer("o6", "pl9", "product5"); //pl9 undef
		}catch(TMException ex) {print("pl9 undef");}
		try{tm.addOffer("o5", "pl7", "product10"); //o5 duplicated
		}catch(TMException ex) {print("o5 duplicated");}
	tm.addRequest("req6", "pl5", "product6"); tm.addOffer("o1", "pl5", "product6");
	tm.addRequest("req0", "pl7", "product1"); tm.addOffer("o6", "pl1", "product1");
	tm.addRequest("req5", "pl1", "product2"); tm.addOffer("o7", "pl6", "product2");
	tm.addOffer("o9", "pl6", "product2"); tm.addRequest("req9", "pl8", "product2");
		try{tm.addRequest("req12", "pl10", "product5"); //pl10 undef
		}catch(TMException ex) {print("pl10 undef");}
		try{tm.addRequest("req5", "pl7", "product10"); //req5 duplicated
		}catch(TMException ex) {print("req5 duplicated");}
		
	//R3
	print("R3");
	tm.addTransaction("t1", "c3", "req1", "o2"); //regions reg2, reg1
		try{tm.addTransaction("t7", "c4", "req2", "o3"); //reg1, reg2 not handled by c4
		}catch(TMException ex) {print("carrier doesn't handle reg2");} 
	tm.addTransaction("t5", "c4", "req4", "o4");
		try{tm.addTransaction("t8", "c4", "req2", "o5"); //productIds product8, product6
		}catch(TMException ex) {print("mismatch in productIds");} 
		try{tm.addTransaction("t9", "c3", "req1", "o9"); 
		}catch(TMException ex) {print("req1 already handled");} 
		try{tm.addTransaction("t10", "c3", "req9", "o2"); 
		}catch(TMException ex) {print("o2 already handled");} 
	tm.addTransaction("t3", "c3", "req6", "o1");
	tm.addTransaction("t4", "c4", "req0", "o6");
	tm.addTransaction("t6", "c3", "req5", "o7");

	boolean b;
	b = tm.evaluateTransaction("t1",6); print(b); //true
	b = tm.evaluateTransaction("t5",12); print(b); //false
	tm.evaluateTransaction("t5",4);
	tm.evaluateTransaction("t3",8);
	tm.evaluateTransaction("t4",7);

//R4
	print("R4");
	SortedMap<Long, List<String>> m1 = tm.deliveryRegionsPerNT();
		print(m1); //{3=[reg1], 1=[reg2, reg7]}
	SortedMap<String, Integer> m2 = tm.scorePerCarrier(5);
		print(m2); //{c3=14, c4=7}
	SortedMap<String, Long> m3 = tm.nTPerProduct();
		print(m3); //{product1=1, product2=1, product4=1, product6=2}
}

}
