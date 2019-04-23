package serializers;

import com.google.gson.*;
import model.*;

import java.lang.reflect.Type;

public class TableRowSerializer implements JsonSerializer {

    @Override
    public JsonElement serialize(Object o, Type type, JsonSerializationContext jsonSerializationContext) {
        final JsonObject jsonObject = new JsonObject();
        TableRow tableRow = (TableRow) o;

        Gson standardGson = new Gson();

        final GsonBuilder conditionResultBuilder = new GsonBuilder();
        conditionResultBuilder.registerTypeAdapter(ConditionResult.class, new ConditionResultSerializer());
        final Gson conditionResultGson = conditionResultBuilder.create();

        String conditionResultString = conditionResultGson.toJson(tableRow.getConditionResults());
        JsonArray jsonArray = standardGson.fromJson(conditionResultString, JsonArray.class);
        jsonObject.add("condition_results", jsonArray);

        jsonObject.addProperty("state", tableRow.getState().toString());
        jsonObject.addProperty("event", tableRow.getEvent().toString());

        final GsonBuilder actionBuilder = new GsonBuilder();
        actionBuilder.registerTypeAdapter(Update.class, new ActionSerializer());
        actionBuilder.registerTypeAdapter(HardwareAction.class, new ActionSerializer());
        actionBuilder.registerTypeAdapter(StateTransition.class, new ActionSerializer());
        final Gson actionGson = actionBuilder.create();

        String actionsString = actionGson.toJson(tableRow.getActions());
        jsonArray = standardGson.fromJson(actionsString, JsonArray.class);

        jsonObject.add("actions", jsonArray);

        return jsonObject;
    }
}
