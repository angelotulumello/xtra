package model;

import com.google.gson.Gson;

import java.util.Objects;

public class State {
    private int label;

    public State(int label) {
        this.label = label;
    }

    public int getName() {
        return label;
    }

    public void setName(int label) {
        this.label = label;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        State state = (State) o;
        return label == state.label;
    }

    @Override
    public int hashCode() {

        return Objects.hash(label);
    }

    @Override
    public String toString() {
        return Integer.toString(label);
    }
}
