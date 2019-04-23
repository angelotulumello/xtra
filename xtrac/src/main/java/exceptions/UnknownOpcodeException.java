package exceptions;

public class UnknownOpcodeException extends ParserException{
    public UnknownOpcodeException(String message) {
        super(message);
    }
}
