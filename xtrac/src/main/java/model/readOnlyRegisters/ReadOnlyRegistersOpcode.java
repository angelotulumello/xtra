package model.readOnlyRegisters;

public enum ReadOnlyRegistersOpcode {
    XTRA_TIMESTAMP;

    public static ReadOnlyRegistersOpcode toEnum(String string) {
        switch (string){
            case "currentTime":
                return XTRA_TIMESTAMP;
        }
        return null;
    }
}
