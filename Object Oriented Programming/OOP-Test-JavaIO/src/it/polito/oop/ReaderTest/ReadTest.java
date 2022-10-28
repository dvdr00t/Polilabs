package it.polito.oop.ReaderTest;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.List;
import java.util.stream.Collectors;

public class ReadTest {

	
	
	/**
	 * Internal class that can be used to read the lines of
	 * a text file into a list of strings.
	 * 
	 * When reading a CSV file remember that the first line
	 * contains the headers, while the real data is contained
	 * in the following lines.
	 * 
	 * @param file the file name
	 * @return a list containing the lines of the file
	 * @throws FileNotFoundException 
	 */
	@SuppressWarnings("unused")
	public static List<String> readData (String fileName) throws FileNotFoundException {
		
		/**
		 * Creates a new FileReader given the name of the file to 
		 * read from 
		 * 
		 * @throw FileNotFoundException if the file is not found
		 */
		FileReader file = new FileReader(fileName);
		
		/*
		 * Create a new BufferedReader of characters given the source of characters
		 * to read from
		 */
		BufferedReader in = new BufferedReader(file);
		
		/*
		 * Return the BufferedReader converted into a LIST of stream
		 * 
		 * .lines() convert the BufferedReader into a STREAM of strings
		 */
		return in.lines()
				.collect(Collectors.toList());
	}
}
