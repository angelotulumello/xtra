package model.headerFieldEnums;

public enum EthField implements FieldEnum{
    XTRA_SRC_MAC,
    XTRA_DST_MAC,
    XTRA_ETH_TYPE;

    public static FieldEnum toEnum(String string) {
        switch (string) {
            case "src":
                return XTRA_SRC_MAC;
            case "dst":
                return XTRA_DST_MAC;
            case "type":
                return XTRA_ETH_TYPE;
        }
        return null;
    }
}
