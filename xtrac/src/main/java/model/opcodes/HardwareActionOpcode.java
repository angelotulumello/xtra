package model.opcodes;

import exceptions.UnknownOpcodeException;
import model.HardwareAction;
import model.opcodes.Opcode;

public enum HardwareActionOpcode implements Opcode {
    XTRA_SENDPKT,
    XTRA_SETTIMER,
    XTRA_STOREPKT,
    XTRA_SETFIELD,
    XTRA_DELETEPKT;

    public static HardwareActionOpcode toHardwareActionOpcode(String string) throws UnknownOpcodeException {
        switch (string)
        {
            case "sendPacket":
                return XTRA_SENDPKT;

            case "setTimer":
                return XTRA_SETTIMER;

            /*case "removeTimer":
                return XTRA_REMOVETIMER;

            case "removeTimers":
                return XTRA_REMOVETIMERS;*/

            case "storePacket":
                return XTRA_STOREPKT;

            case "setField":
                return XTRA_SETFIELD;

            case "deletePacket":
                return XTRA_DELETEPKT;

            default:
                throw new UnknownOpcodeException("The action "+string+" wasn't declared.");

        }
    }
}

