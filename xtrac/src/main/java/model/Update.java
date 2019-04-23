package model;

import model.opcodes.UpdateOpcode;

import java.util.Objects;

public class Update extends Action {
    private UpdateOpcode opcode;
    private Operand op0;
    private Operand op1;
    private Register op2;

    public Update(UpdateOpcode opcode, Operand op0, Operand op1, Register op2) {
        this.opcode = opcode;
        this.op0 = op0;
        this.op1 = op1;
        this.op2 = op2;
    }

    public UpdateOpcode getOpcode() {
        return opcode;
    }

    public void setOpcode(UpdateOpcode opcode) {
        this.opcode = opcode;
    }

    public Operand getOp0() {
        return op0;
    }

    public void setOp0(Operand op0) {
        this.op0 = op0;
    }

    public Operand getOp1() {
        return op1;
    }

    public void setOp1(Operand op1) {
        this.op1 = op1;
    }

    public Register getOp2() {
        return op2;
    }

    public void setOp2(Register op2) {
        this.op2 = op2;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Update update = (Update) o;
        return opcode == update.opcode &&
                Objects.equals(op0, update.op0) &&
                Objects.equals(op1, update.op1) &&
                Objects.equals(op2, update.op2);
    }

    @Override
    public int hashCode() {

        return Objects.hash(opcode, op0, op1, op2);
    }
}
