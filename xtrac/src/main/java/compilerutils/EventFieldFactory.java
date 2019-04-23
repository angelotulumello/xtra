package compilerutils;

import model.Register;
import model.eventFieldEnums.PktRcvd;
import model.eventFieldEnums.Timeout;

public class EventFieldFactory {
    private static EventFieldFactory ourInstance = new EventFieldFactory();

    public static EventFieldFactory getInstance() {
        return ourInstance;
    }

    private EventFieldFactory() {
    }

    public Register getEventField(String event, String fieldname) {
        switch (event)
        {
            case "timeout":
                return new Register(Timeout.toEnum(fieldname).toString());
            case "pktRcvd":
                return new Register(PktRcvd.toEnum(fieldname).toString());
        }
        return null;
    }
}
