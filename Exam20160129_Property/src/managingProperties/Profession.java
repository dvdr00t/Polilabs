package managingProperties;

import java.util.LinkedList;
import java.util.List;

public class Profession {
	
	/*
	 * ATTRIBUTES 
	 */
	private String professionName;
	private List<String> workersID;
	
	/**
	 * CONSTRUCTOR
	 * 
	 * @param professionName
	 */
	public Profession(String professionName) {
		this.professionName = professionName;
		this.workersID = new LinkedList<String>();
	};
	
	/*
	 * GETTER
	 */
	public String getProfessionName() {
		return professionName;
	}

	/**
	 * Add a new worker to this profession if the worker is not already present in the list
	 * 
	 * @param workerID
	 * @throws PropertyException
	 */
	public void addWorker(String workerID) throws PropertyException {
		
		//Checking if the workerID already exists
		if (this.workersID.contains(workerID))
			throw new PropertyException();
		
		//Adding to the list of workers the new workerID
		this.workersID.add(workerID);
	}
	
	/**
	 * Get the number of worker for this profession
	 * 
	 * @return int the number of worker for this profession
	 */
	public int getNumberOfWorker() {
		return this.workersID.size();
	}
	
	/**
	 * Checks if the worker works in this profession
	 * 
	 * @param workerID
	 * @return
	 */
	public boolean hasWorker (String workerID) {
		if (this.workersID.contains(workerID))
			return true;
		return false;
	}

}
