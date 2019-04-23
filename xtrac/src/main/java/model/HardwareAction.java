package model;

import model.opcodes.HardwareActionOpcode;

import java.util.Objects;

public class HardwareAction extends Action{
    private Operand op0;
    private Operand op1;
    private Operand op2;
    private HardwareActionOpcode opcode;

    public HardwareAction(Operand op0, Operand op1, Operand op2, HardwareActionOpcode opcode) {
        this.op0 = op0;
        this.op1 = op1;
        this.op2 = op2;
        this.opcode = opcode;
    }

    public HardwareAction(Operand op0, HardwareActionOpcode opcode) {
        this.op0 = op0;
        this.op1 = null;
        this.op2 = null;
        this.opcode = opcode;
    }

    public HardwareAction(Operand op0, Operand op1, HardwareActionOpcode opcode) {
        this.op0 = op0;
        this.op1 = op1;
        this.op2 = null;
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

    public Operand getOp2() {
        return op2;
    }

    public void setOp2(Operand op2) {
        this.op2 = op2;
    }

    public HardwareActionOpcode getOpcode() {
        return opcode;
    }

    public void setOpcode(HardwareActionOpcode opcode) {
        this.opcode = opcode;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        HardwareAction that = (HardwareAction) o;
        return Objects.equals(op0, that.op0) &&
                Objects.equals(op1, that.op1) &&
                Objects.equals(op2, that.op2) &&
                opcode == that.opcode;
    }

    @Override
    public int hashCode() {

        return Objects.hash(op0, op1, op2, opcode);
    }
}
