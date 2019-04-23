package compilerutils;

import model.HeaderField;
import model.headerFieldEnums.EthField;
import model.headerFieldEnums.IPv4Field;
import model.headerFieldEnums.TCPField;
import model.headerFieldEnums.UdpField;

public class HeaderFieldFactory {
    private static HeaderFieldFactory ourInstance = new HeaderFieldFactory();

    public static HeaderFieldFactory getInstance() {
        return ourInstance;
    }

    private HeaderFieldFactory() {
    }

    public HeaderField getHeaderField(String headerType, String fieldName)
    {
        switch (headerType) {
            case "eth":
                return new HeaderField(EthField.toEnum(fieldName).toString());

            case "ipv4":
                return new HeaderField(IPv4Field.toEnum(fieldName).toString());

            case "tcp":
                return new HeaderField(TCPField.toEnum(fieldName).toString());

            case "udp":
                return new HeaderField(UdpField.toEnum(fieldName).toString());
        }
        return null;
    }
}
