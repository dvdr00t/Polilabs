import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.List;
import java.util.stream.Collectors;

import it.polito.oop.ReaderTest.House;

public class TestApp {
	
	public static void main(String[] args) throws FileNotFoundException {
		
		House house = House.fromFile("Test", "test1.csv");
		System.out.println(house.toString());
	}

	

}
