package hydraulic;

/**
 * Represents the generic abstract element of an hydraulics system.
 * It is the base class for all elements.
 *
 * Any element can be connect to a downstream element
 * using the method {@link #connect(Element) connect()}.
 */
public abstract class Element {
	
	/*
	 * ATTRIBUTE FOR ABSTRACT ELEMENT
	 */
	private String name;			//NAME OF THE ELEMENTS
	private Element output;			//OUTPUT LINKED WITH THE ELEMENT
	private Element input;			//INPUT LINKED WITH THE ELEMENT
	private double inputFlow = 0;
	private double outputFlow = 0;
	protected String className;
	
	public String getClassName() {
		return className;
	}
	/*
	 *  GETTERS AND SETTERS METHOD FOR FLOW
	 */
	public double getInputFlow() {
		return this.inputFlow;
	}
	public void setInputFlow(double inputFlow) {
		this.inputFlow = inputFlow;
	}
	public double getOutputFlow() {
		return this.outputFlow;
	}
	public void setOutputFlow(double outputFlow) {
		this.outputFlow = outputFlow;
	}
	public Element getInput() {
		return input;
	}
	public void setInput(Element input) {
		this.input = input;
	}
	
	
	
	/**
	 * Constructor
	 * @param name the name of the element
	 */
	public Element(String name){
		this.name = name;
	}

	/**
	 * getter method
	 * @return the name of the element
	 */
	public String getName(){
		return this.name;
	}
	
	/**
	 * Connects this element to a given element.
	 * The given element will be connected downstream of this element
	 * @param elem the element that will be placed downstream
	 */
	public void connect(Element elem){
		
		//CHECKING IF THIS ELEMENT IS NOT A SINK
		if (this instanceof Sink)
			return;
		
		//CONNECTING THIS OUTPUT WITH THE ELEMENT GIVEN
		this.output = elem;
		
		//CONNECTING THE INPUT WITH THE ELEMENT
		elem.setInput(this);
	}
	
	/**
	 * Retrieves the element connected downstream of this
	 * @return downstream element
	 */
	public Element getOutput(){
		return this.output;
	}
	
	/*
	 * TO BE IMPLEMENTED IN SUBCLASSES
	 */
	public abstract void simulate(Double inFlow, SimulationObserverExt observer, boolean enableMaxFlowCheck);
	public abstract void simulate(Double inFlow, SimulationObserver observer);
	public abstract String layout(String layoutString);
	
}
