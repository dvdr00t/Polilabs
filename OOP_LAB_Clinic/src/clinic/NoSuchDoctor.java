package clinic;

public class NoSuchDoctor extends Exception {
	private static final long serialVersionUID = 1L;
	public NoSuchDoctor() {}
	public NoSuchDoctor(String msg) {
		super(msg);
	}

}
