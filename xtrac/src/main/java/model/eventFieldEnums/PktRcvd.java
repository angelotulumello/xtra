package model.eventFieldEnums;

public enum PktRcvd {
    XTRA_CONTEXT_PARAM0;

    public static PktRcvd toEnum(String string) {
        switch (string) {
            case "port":
                return XTRA_CONTEXT_PARAM0;
        }
        return null;
    }
}
