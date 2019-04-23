package exceptions;

public class TooManyArgsException extends ParserException{
    public TooManyArgsException(String message) {
        super(message);
    }
}
