package model;

import java.util.Vector;

public class TableRow {
    private Vector<ConditionResult> conditionResults;
    private Vector<Action> actions;
    private State state;
    private Event event;

    public TableRow(Vector<ConditionResult> conditionResults, Vector<Action> actions, State state, Event event) {
        this.conditionResults = conditionResults;
        this.actions = actions;
        this.state = state;
        this.event = event;
    }

    public Vector<ConditionResult> getConditionResults() {
        return conditionResults;
    }

    public void setConditionResults(Vector<ConditionResult> conditionResults) {
        this.conditionResults = conditionResults;
    }

    public Vector<Action> getActions() {
        return actions;
    }

    public void setActions(Vector<Action> actions) {
        this.actions = actions;
    }

    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    public Event getEvent() {
        return event;
    }

    public void setEvent(Event event) {
        this.event = event;
    }


}
