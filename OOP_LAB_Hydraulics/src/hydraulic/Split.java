package hydraulic;

/**
 * Represents a split element, a.k.a. T element
 * 
 * During the simulation each downstream element will
 * receive a stream that is half the input stream of the split.
 */

public class Split extends Element {
	
	/*
	 * ATTRIBUTE FOR THE SPLIT ELEMENT
	 */
	Element[] outputs = new Element[2];		//OUTPUT IS SPLITTED IN TWO

	/**
	 * Constructor
	 * @param name
	 */
	public Split(String name) {
		super(name);
		//TODO: complete
	}
    
	/**
	 * returns the downstream elements
	 * @return array containing the two downstream element
	 */
    public Element[] getOutputs(){
        return this.outputs;
    }

    /**
     * connect one of the outputs of this split to a
     * downstream component.
     * 
     * @param elem  the element to be connected downstream
     * @param noutput the output number to be used to connect the element
     */
	public void connect(Element elem, int noutput){
		
		//CONNECTING ELEMENT WITH ITS OUTPUT
		this.outputs[noutput] = elem;
	}
}
