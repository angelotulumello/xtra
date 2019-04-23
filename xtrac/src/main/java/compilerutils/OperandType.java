package compilerutils;

import model.Constant;
import model.Operand;
import model.Register;

public enum OperandType {
    XTRA_REGISTER,
    XTRA_CONSTANT,
    XTRA_HEADER_FIELD;

    public static OperandType toEnum(Operand operand)
    {
        if (operand.getClass() == Register.class)
            return XTRA_REGISTER;
        else if (operand.getClass() == Constant.class)
            return XTRA_CONSTANT;
        else return XTRA_HEADER_FIELD;
    }
}
