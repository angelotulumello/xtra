package compilerutils;

import model.Event;

import java.util.HashMap;
import java.util.Vector;

public class EventOracle {
    private static EventOracle ourInstance = new EventOracle();
    private HashMap<Event, Integer> map;

    public static EventOracle getInstance() {
        return ourInstance;
    }

    private EventOracle() {
        map = new HashMap<>();
    }

    public void resetMap()
    {
        map = new HashMap<>();

        map.put(Event.XTRA_PKT_RCVD, 42);
        map.put(Event.XTRA_TIMEOUT, 42);
    }

    public boolean isPresent(Event event)
    {
        return map.containsKey(event);
    }

    public void remove(Event event)
    {
        map.remove(event);
    }

    public Vector<Event> getRemaining()
    {
        return new Vector<>(map.keySet());
    }
}
