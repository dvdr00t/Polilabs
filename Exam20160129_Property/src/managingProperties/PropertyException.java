package managingProperties;

public class PropertyException extends Exception {
    private static final long serialVersionUID = 1L;

    public PropertyException(){}
    public PropertyException(String msg){
        super(msg);
    }
}
