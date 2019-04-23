package model.headerFieldEnums;

public enum IPv4Field implements FieldEnum{
    XTRA_IPv4_SRC,
    XTRA_IPv4_DST,
    XTRA_IPv4_TTL,
    XTRA_IPv4_PROTO,
    XTRA_IPv4_CHKSUM;

    public static FieldEnum toEnum(String string) {
        switch (string) {
            case "src":
                return XTRA_IPv4_SRC;
            case "dst":
                return XTRA_IPv4_DST;
            case "ttl":
                return XTRA_IPv4_TTL;
            case "proto":
                return XTRA_IPv4_PROTO;
            case "checksum":
                return XTRA_IPv4_CHKSUM;

        }
        return null;
    }
}
