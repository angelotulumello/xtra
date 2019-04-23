package compilerutils;

import exceptions.UndeclaredStateException;
import model.State;

import java.util.HashMap;

public class StateFactory {
    private static StateFactory ourInstance = new StateFactory();
    private int counter;
    private HashMap<String, State> states;
    private HashMap<String, State> pendingStates; // states referenced not declared yet

    public static StateFactory getInstance() {
        return ourInstance;
    }

    private StateFactory() {
        states = new HashMap<>();
        pendingStates = new HashMap<>();
        counter = 0;
    }

    public State declareState(String name)
    {
        State state = states.get(name);
        if (state == null)
        {
            state = new State(counter);
            states.put(name, state);
            counter++;
        }
        pendingStates.remove(name);
        return state;
    }

    public State referenceState(String name)
    {
        State state = states.get(name);
        if (state == null)
        {
            state = new State(counter);
            states.put(name, state);
            pendingStates.put(name, state);
            counter++;
        }
        return state;
    }

    public void checkPendings() throws UndeclaredStateException {
        if(!pendingStates.isEmpty()) {
            String states = new String();
            for (String name : pendingStates.keySet()) {
                states += name+", ";
            }
            if(pendingStates.keySet().size() > 1)
                throw new UndeclaredStateException("The state "+states+"wasn't declared!");
            else
                throw new UndeclaredStateException("The states "+states+"weren't declared!");
        }
    }
}
