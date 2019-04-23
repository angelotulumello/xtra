package model.headerFieldEnums;

public enum TCPField implements FieldEnum{
    XTRA_TCP_SPORT,
    XTRA_TCP_DPORT,
    XTRA_TCP_SEQNO,
    XTRA_TCP_ACKNO,
    XTRA_TCP_WIN,
    XTRA_TCP_FLAGS,
    XTRA_TCP_CHKSUM,
    XTRA_TCP_TIMESTAMP,
    XTRA_TCP_TS_ECHOREPLY,
    XTRA_TCP_WIN_SCALE,
    XTRA_TCP_MAX_MSS;

    public static FieldEnum toEnum(String string) {
        switch (string) {
            case "sport":
                return XTRA_TCP_SPORT;
            case "dport":
                return XTRA_TCP_DPORT;
            case "seqNo":
                return XTRA_TCP_SEQNO;
            case "ackNo":
                return XTRA_TCP_ACKNO;
            case "window":
                return XTRA_TCP_WIN;
            case "flags":
                return XTRA_TCP_FLAGS;
            case "checksum":
                return XTRA_TCP_CHKSUM;
            case "timestamp":
                return XTRA_TCP_TIMESTAMP;
            case "timestampEchoReply":
                return XTRA_TCP_TS_ECHOREPLY;
            case "mss":
                return XTRA_TCP_MAX_MSS;
            case "windowScale":
                return XTRA_TCP_WIN_SCALE;
        }
        return null;
    }
}
