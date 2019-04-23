package compilerutils;

import model.Condition;
import model.Operand;
import model.opcodes.ConditionOpcode;

import java.util.HashMap;

public class ConditionFactory {
    private static ConditionFactory ourInstance = new ConditionFactory();
    private int counter;
    private HashMap<Condition, Integer> conditions;

    public static ConditionFactory getInstance() {
        return ourInstance;
    }

    private ConditionFactory(){
        counter = 0;
        conditions = new HashMap<>();
    }

    public Condition createCondition(Operand op0, Operand op1, ConditionOpcode opcode)
    {
        Condition tempCondition = new Condition(counter, op0, op1, opcode);
        conditions.put(tempCondition, counter);
        counter++;
        return  tempCondition;
    }

    public boolean isPresent(Operand op0, Operand op1, ConditionOpcode opcode)
    {
        Condition tempCondition = new Condition(counter, op0, op1, opcode);
        return conditions.containsKey(tempCondition);
    }

    public Condition getCondition(Operand op0, Operand op1, ConditionOpcode opcode)
    {
        Condition tempCondition = new Condition(counter, op0, op1, opcode);
        tempCondition.setId(conditions.get(tempCondition));
        return tempCondition;
    }
}
