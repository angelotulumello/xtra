package serializers;

import com.google.gson.*;
import exceptions.WrongClassException;
import model.Condition;
import model.ConditionResult;

import java.lang.reflect.Type;

public class ConditionResultSerializer implements JsonSerializer {

    @Override
    public JsonElement serialize(Object o, Type type, JsonSerializationContext jsonSerializationContext) {
        final JsonObject jsonObject = new JsonObject();
        ConditionResult conditionResult = (ConditionResult) o;

        jsonObject.addProperty("id", conditionResult.getId().toString());
        jsonObject.addProperty("result", conditionResult.getResult().toString());

        return jsonObject;
    }
}
