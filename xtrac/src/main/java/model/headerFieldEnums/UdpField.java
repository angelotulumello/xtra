package model.headerFieldEnums;

public enum UdpField implements FieldEnum{
    XTRA_UDP_SPORT,
    XTRA_UDP_DPORT,
    XTRA_UDP_LENGTH,
    XTRA_UDP_CHKSUM,
    XTRA_UDP_P1,
    XTRA_UDP_P2,
    XTRA_UDP_P3,
    XTRA_UDP_P4;

    public static FieldEnum toEnum(String string) {
        switch (string) {
            case "sport":
                return XTRA_UDP_SPORT;
            case "dport":
                return XTRA_UDP_DPORT;
            case "checksum":
                return XTRA_UDP_CHKSUM;
            case "length":
                return XTRA_UDP_LENGTH;
            case "p1":
                return XTRA_UDP_P1;
            case "p2":
                return XTRA_UDP_P2;
            case "p3":
                return XTRA_UDP_P3;
            case "p4":
                return XTRA_UDP_P4;
        }
        return null;
    }
}
