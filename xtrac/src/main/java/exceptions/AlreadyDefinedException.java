package exceptions;

public class AlreadyDefinedException extends ParserException{
    public AlreadyDefinedException(String message) {
        super(message);
    }
}
