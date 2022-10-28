package it.polito.oop.elective;

public class ElectiveException extends Exception {
    private static final long serialVersionUID = 1L;
    public ElectiveException() {}
    public ElectiveException(String msg) {
        super(msg);
    }
}
