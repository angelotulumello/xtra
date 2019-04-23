package model;

import org.omg.CORBA.TIMEOUT;

import java.util.Vector;

public enum Event {
    XTRA_PKT_RCVD,
    XTRA_TIMEOUT;

    public static Event toEvent(String name)
    {
        switch (name)
        {
            case "pktRcvd":
                return XTRA_PKT_RCVD;

            case "timeout":
                return XTRA_TIMEOUT;

            default:
                return null; // todo: exception
        }
    }

    public static Vector<Event> getEvents()
    {
        Vector<Event> events = new Vector<>();
        events.add(XTRA_PKT_RCVD);
        events.add(XTRA_TIMEOUT);

        return events;
    }
}
