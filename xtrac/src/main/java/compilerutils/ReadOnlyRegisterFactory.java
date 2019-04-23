package compilerutils;

import exceptions.UndeclaredRegisterException;
import model.Register;
import model.readOnlyRegisters.ReadOnlyRegistersOpcode;

public class ReadOnlyRegisterFactory {
    private static ReadOnlyRegisterFactory ourInstance = new ReadOnlyRegisterFactory();

    public static ReadOnlyRegisterFactory getInstance() {
        return ourInstance;
    }

    private ReadOnlyRegisterFactory() {
    }

    public Register getReadOnlyRegister(String name) throws UndeclaredRegisterException {
        ReadOnlyRegistersOpcode opcode = ReadOnlyRegistersOpcode.toEnum(name);

        if(opcode == null)
            throw new UndeclaredRegisterException("The ReadOnlyRegister '"+name+"' doesn't exist!");

        return new Register(opcode.toString());
    }
}
