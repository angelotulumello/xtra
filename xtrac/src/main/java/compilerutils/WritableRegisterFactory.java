package compilerutils;

import model.Register;

import java.util.HashMap;

public class WritableRegisterFactory {
    private static WritableRegisterFactory ourInstance = new WritableRegisterFactory();
    private HashMap<String, Register> registers;
    private int counter;

    public static WritableRegisterFactory getInstance() {
        return ourInstance;
    }

    private WritableRegisterFactory() {
        registers = new HashMap<>();
        counter = 0;
    }

    public Register allocateRegister(String name)
    {
        Register register = registers.get(name);
        if(register == null)
        {
            register = new Register("XTRA_CUSTOM"+counter);
            counter++;
            registers.put(name, register);
        }
        return register;
    }

    public Register getRegister(String name)
    {
        return registers.get(name);
    }

    public boolean isPresent(String name)
    {
        return registers.containsKey(name);
    }
}
