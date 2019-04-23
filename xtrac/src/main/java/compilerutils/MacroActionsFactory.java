package compilerutils;

import exceptions.AlreadyDefinedException;
import exceptions.UndefinedMacroActionException;
import model.Action;

import java.util.HashMap;
import java.util.Vector;

public class MacroActionsFactory {
    private HashMap<String, Vector<Action>> macroActions;
    private static MacroActionsFactory ourInstance = new MacroActionsFactory();

    public static MacroActionsFactory getInstance() {
        return ourInstance;
    }

    private MacroActionsFactory() {
        macroActions = new HashMap<>();
    }

    public void createMacroAction(String name, Vector<Action> actions) throws AlreadyDefinedException {
        Vector<Action> acts = new Vector<>();
        if(macroActions.containsKey(name))
            throw new AlreadyDefinedException("The macro action "+ name + "was already defined");

        acts.addAll(actions);
        macroActions.put(name, acts);
    }

    public Vector<Action> getMacroAction(String name) throws UndefinedMacroActionException {
        if(!macroActions.containsKey(name))
            throw new UndefinedMacroActionException(name);
        return macroActions.get(name);
    }
}
