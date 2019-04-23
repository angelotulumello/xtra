package model;

import com.google.gson.JsonObject;

import java.util.Vector;

public class Table {
    private State initialState;
    private Vector<TableRow> tableRows;
    private Vector<Condition> conditions;

    public Table(State initialState, Vector<TableRow> tableRows, Vector<Condition> conditions) {
        this.initialState = initialState;
        this.tableRows = tableRows;
        this.conditions = conditions;
    }

    public State getInitialState() {
        return initialState;
    }

    public void setInitialState(State initialState) {
        this.initialState = initialState;
    }

    public Vector<TableRow> getTableRows() {
        return tableRows;
    }

    public void setTableRows(Vector<TableRow> tableRows) {
        this.tableRows = tableRows;
    }

    public Vector<Condition> getConditions() {
        return conditions;
    }

    public void setConditions(Vector<Condition> conditions) {
        this.conditions = conditions;
    }
}
