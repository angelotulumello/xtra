package model.eventFieldEnums;

public enum Timeout {
    XTRA_CONTEXT_PARAM0,
    XTRA_CONTEXT_PARAM1;

    public static Timeout toEnum(String string) {
        switch (string) {
            case "data0":
                return XTRA_CONTEXT_PARAM0;
            case "data1":
                return XTRA_CONTEXT_PARAM1;
        }
        return null;
    }
}
