package model;

import model.opcodes.ConditionOpcode;
import model.opcodes.Opcode;

import java.util.Objects;

public class Condition {
    private int id;
    private Operand op0;
    private Operand op1;
    private ConditionOpcode opcode;

    public Condition(int id, Operand op0, Operand op1, ConditionOpcode opcode) {
        this.id = id;
        this.op0 = op0;
        this.op1 = op1;
        this.opcode = opcode;
    }

    public int getId() {
        return id;
    }

    public Operand getOp0() {
        return op0;
    }

    public Operand getOp1() {
        return op1;
    }

    public Opcode getOpcode() {
        return opcode;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setOp0(Operand op0) {
        this.op0 = op0;
    }

    public void setOp1(Operand op1) {
        this.op1 = op1;
    }

    public void setOpcode(ConditionOpcode opcode) {
        this.opcode = opcode;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Condition condition = (Condition) o;
        return Objects.equals(op0, condition.op0) &&
                Objects.equals(op1, condition.op1) &&
                opcode == condition.opcode;
    }

    @Override
    public int hashCode() {

        return Objects.hash(op0, op1, opcode);
    }

    @Override
    public String toString() {
        return Integer.toString(id);
    }
}
