package model;

import model.opcodes.StateTransitionOpcode;

import java.util.Objects;

public class StateTransition extends Action {
    private State op0;

    public StateTransition(State op0) {
        this.op0 = op0;
    }

    public State getOp0() {
        return op0;
    }

    public void setOp0(State op0) {
        this.op0 = op0;
    }

    public StateTransitionOpcode getOpcode() {
        return StateTransitionOpcode.XTRA_STATE_TRANSITION;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        StateTransition that = (StateTransition) o;
        return Objects.equals(op0, that.op0);
    }

    @Override
    public int hashCode() {

        return Objects.hash(op0);
    }
}
