package serializers;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonSerializationContext;
import com.google.gson.JsonSerializer;
import compilerutils.OperandType;
import model.Condition;

import java.lang.reflect.Type;

public class ConditionSerializer implements JsonSerializer{
    @Override
    public JsonElement serialize(Object o, Type type, JsonSerializationContext jsonSerializationContext) {
        final JsonObject jsonObject = new JsonObject();
        Condition condition = (Condition) o;

        jsonObject.addProperty("id", condition.toString());
        jsonObject.addProperty("opcode", condition.getOpcode().toString());
        jsonObject.addProperty("op0", condition.getOp0().toString());
        jsonObject.addProperty("op0Type", OperandType.toEnum(condition.getOp0()).toString());
        jsonObject.addProperty("op1", condition.getOp1().toString());
        jsonObject.addProperty("op1Type", OperandType.toEnum(condition.getOp1()).toString());

        return jsonObject;
    }
}
