package compilerutils;

import exceptions.TooManyArgsException;
import exceptions.UndeclaredStateException;
import exceptions.UnknownOpcodeException;
import model.Action;
import model.HardwareAction;
import model.Operand;
import model.StateTransition;
import model.opcodes.HardwareActionOpcode;
import model.opcodes.Opcode;
import model.opcodes.StateTransitionOpcode;
import parser.xtraParser;

import java.util.HashMap;
import java.util.List;
import java.util.Vector;

public class ActionsFactory {
    private HashMap<Opcode, Integer> argsMap= new HashMap<>();

    private static ActionsFactory ourInstance = new ActionsFactory();

    public static ActionsFactory getInstance() {
        return ourInstance;
    }

    private ActionsFactory() {
        argsMap.put(HardwareActionOpcode.XTRA_SENDPKT, 2);
        argsMap.put(HardwareActionOpcode.XTRA_SETTIMER, 3);
        argsMap.put(StateTransitionOpcode.XTRA_STATE_TRANSITION, 1);
        argsMap.put(HardwareActionOpcode.XTRA_STOREPKT, 1);
        argsMap.put(HardwareActionOpcode.XTRA_SETFIELD, 3);
        argsMap.put(HardwareActionOpcode.XTRA_DELETEPKT, 1);
    }

    public Action getStateTransition(List<xtraParser.TermContext> terms) throws TooManyArgsException, UndeclaredStateException {
            if (terms.size() != argsMap.get(StateTransitionOpcode.XTRA_STATE_TRANSITION))
                throw new TooManyArgsException("Too many args for the function 'setNextState', got: "+terms.size()+
                        ", expected: "+argsMap.get(StateTransitionOpcode.XTRA_STATE_TRANSITION));
            return new StateTransition(StateFactory.getInstance().referenceState(terms.get(0).getText()));
    }

    public boolean isAStateTransition(String name)
    {
        return name.equals("setNextState");
    }

    public Action getAction(String name, Vector<Operand> args) throws UnknownOpcodeException, TooManyArgsException {
        HardwareActionOpcode opcode = HardwareActionOpcode.toHardwareActionOpcode(name);

        if(args.size() != argsMap.get(opcode))
            throw new TooManyArgsException("Too many args for the function '"
                    +name+"', got: "+args.size()+
                    ", expected: "+argsMap.get(opcode));
        switch (argsMap.get(opcode))
        {
            case 1:
                return new HardwareAction(args.get(0), opcode);

            case 2:
                return new HardwareAction(args.get(0), args.get(1), opcode);

            case 3:
                return new HardwareAction(args.get(0), args.get(1), args.get(2), opcode);

            default:
                return null;
        }
    }
}
