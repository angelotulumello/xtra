package serializers;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonSerializationContext;
import com.google.gson.JsonSerializer;
import compilerutils.OperandType;
import model.Action;
import model.HardwareAction;
import model.StateTransition;
import model.Update;

import java.lang.reflect.Type;

public class ActionSerializer implements JsonSerializer {
    @Override
    public JsonElement serialize(Object o, Type type, JsonSerializationContext jsonSerializationContext) {
        JsonObject jsonObject = new JsonObject();

        if (o.getClass() == Update.class) {
            Update update = (Update) o;
            jsonObject.addProperty("opcode", update.getOpcode().toString());
            jsonObject.addProperty("op0", update.getOp0().toString());
            jsonObject.addProperty("op0Type", OperandType.toEnum(update.getOp0()).toString());
            jsonObject.addProperty("op1", update.getOp1().toString());
            jsonObject.addProperty("op1Type", OperandType.toEnum(update.getOp1()).toString());
            jsonObject.addProperty("op2", update.getOp2().toString());
            jsonObject.addProperty("op2Type", OperandType.toEnum(update.getOp2()).toString());
            // useless because op2 must be a register!
        }
        else if (o.getClass() == HardwareAction.class)
        {
            HardwareAction hardwareAction = (HardwareAction) o;

            jsonObject.addProperty("opcode", hardwareAction.getOpcode().toString());
            jsonObject.addProperty("op0", hardwareAction.getOp0().toString());
            jsonObject.addProperty("op0Type", OperandType.toEnum(hardwareAction.getOp0()).toString());
            if(hardwareAction.getOp1() != null) {
                jsonObject.addProperty("op1", hardwareAction.getOp1().toString());
                jsonObject.addProperty("op1Type", OperandType.toEnum(hardwareAction.getOp1()).toString());
            }
            if(hardwareAction.getOp2() != null) {
                jsonObject.addProperty("op2", hardwareAction.getOp2().toString());
                jsonObject.addProperty("op2Type", OperandType.toEnum(hardwareAction.getOp2()).toString());
            }
        }
        else if (o.getClass() == StateTransition.class)
        {
            StateTransition stateTransition = (StateTransition) o;

            jsonObject.addProperty("opcode", stateTransition.getOpcode().toString());
            jsonObject.addProperty("op0", stateTransition.getOp0().toString());
        }

        return jsonObject;
    }
}
