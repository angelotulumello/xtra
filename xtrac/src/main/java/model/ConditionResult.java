package model;

public class ConditionResult {
    private Condition id;
    private Boolean result;

    public ConditionResult(Condition id, Boolean result) {
        this.id = id;
        this.result = result;
    }

    public Condition getId() {
        return id;
    }

    public Boolean getResult() {
        return result;
    }

    public void setId(Condition id) {
        this.id = id;
    }

    public void setResult(Boolean result) {
        this.result = result;
    }
}
