package clinic;

public class NoSuchPatient extends Exception {
	private static final long serialVersionUID = 1L;
	public NoSuchPatient() {}
	public NoSuchPatient(String msg) {
		super(msg);
	}
}
