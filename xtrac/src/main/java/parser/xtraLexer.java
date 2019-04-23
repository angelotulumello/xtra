// Generated from /home/giacomo/IdeaProjects/XTRAC/src/main/java/xtra.g4 by ANTLR 4.7
package parser;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class xtraLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.7", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, COMMA=2, PLUS=3, MINUS=4, MUL=5, DIV=6, EQ=7, LEQ=8, GEQ=9, NEQ=10, 
		GRET=11, LESS=12, LPARENT=13, RPARENT=14, LBRACE=15, RBRACE=16, SEMICOLON=17, 
		ASSIGNMENT=18, MAX=19, MIN=20, INITIAL_MODIFIER=21, EVENT=22, EVENT_FIELD=23, 
		REGISTER_DECLARER=24, STATE_DECLARER=25, CONDITIONS_DECLARER=26, EVENT_DECLARER=27, 
		CUSTOM_EVENT_DECLARER=28, MACRO_ACTION_DECLARER=29, ETH=30, IPV4=31, TCP=32, 
		UDP=33, ETH_FIELD=34, IPV4_FIELD=35, TCP_FIELD=36, UDP_FIELD=37, ID=38, 
		NUMBER=39, WS=40, COMMENT=41, LINE_COMMENT=42;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	public static final String[] ruleNames = {
		"T__0", "COMMA", "PLUS", "MINUS", "MUL", "DIV", "EQ", "LEQ", "GEQ", "NEQ", 
		"GRET", "LESS", "LPARENT", "RPARENT", "LBRACE", "RBRACE", "SEMICOLON", 
		"ASSIGNMENT", "MAX", "MIN", "INITIAL_MODIFIER", "EVENT", "EVENT_FIELD", 
		"REGISTER_DECLARER", "STATE_DECLARER", "CONDITIONS_DECLARER", "EVENT_DECLARER", 
		"CUSTOM_EVENT_DECLARER", "MACRO_ACTION_DECLARER", "ETH", "IPV4", "TCP", 
		"UDP", "ETH_FIELD", "IPV4_FIELD", "TCP_FIELD", "UDP_FIELD", "ID", "NUMBER", 
		"WS", "COMMENT", "LINE_COMMENT", "LETTER", "DIGIT"
	};

	private static final String[] _LITERAL_NAMES = {
		null, "'.'", "','", "'+'", "'-'", "'*'", "'/'", "'=='", "'<='", "'>='", 
		"'!='", "'>'", "'<'", "'('", "')'", "'{'", "'}'", "';'", "'='", "'max'", 
		"'min'", "'initial'", null, null, "'Register'", "'State'", "'if'", "'on'", 
		"'Event'", "'Action'", "'eth'", "'ipv4'", "'tcp'", "'udp'"
	};
	private static final String[] _SYMBOLIC_NAMES = {
		null, null, "COMMA", "PLUS", "MINUS", "MUL", "DIV", "EQ", "LEQ", "GEQ", 
		"NEQ", "GRET", "LESS", "LPARENT", "RPARENT", "LBRACE", "RBRACE", "SEMICOLON", 
		"ASSIGNMENT", "MAX", "MIN", "INITIAL_MODIFIER", "EVENT", "EVENT_FIELD", 
		"REGISTER_DECLARER", "STATE_DECLARER", "CONDITIONS_DECLARER", "EVENT_DECLARER", 
		"CUSTOM_EVENT_DECLARER", "MACRO_ACTION_DECLARER", "ETH", "IPV4", "TCP", 
		"UDP", "ETH_FIELD", "IPV4_FIELD", "TCP_FIELD", "UDP_FIELD", "ID", "NUMBER", 
		"WS", "COMMENT", "LINE_COMMENT"
	};
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public xtraLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "xtra.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2,\u01bd\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\b"+
		"\3\t\3\t\3\t\3\n\3\n\3\n\3\13\3\13\3\13\3\f\3\f\3\r\3\r\3\16\3\16\3\17"+
		"\3\17\3\20\3\20\3\21\3\21\3\22\3\22\3\23\3\23\3\24\3\24\3\24\3\24\3\25"+
		"\3\25\3\25\3\25\3\26\3\26\3\26\3\26\3\26\3\26\3\26\3\26\3\27\3\27\3\27"+
		"\3\27\3\27\3\27\3\27\3\27\3\27\3\27\3\27\3\27\3\27\3\27\5\27\u00a2\n\27"+
		"\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30\3\30"+
		"\5\30\u00b2\n\30\3\31\3\31\3\31\3\31\3\31\3\31\3\31\3\31\3\31\3\32\3\32"+
		"\3\32\3\32\3\32\3\32\3\33\3\33\3\33\3\34\3\34\3\34\3\35\3\35\3\35\3\35"+
		"\3\35\3\35\3\36\3\36\3\36\3\36\3\36\3\36\3\36\3\37\3\37\3\37\3\37\3 \3"+
		" \3 \3 \3 \3!\3!\3!\3!\3\"\3\"\3\"\3\"\3#\3#\3#\3#\3#\3#\3#\3#\3#\3#\5"+
		"#\u00f1\n#\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3$\3"+
		"$\3$\3$\5$\u0109\n$\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3"+
		"%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3"+
		"%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3"+
		"%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\3%\5%\u015b\n%\3&\3"+
		"&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3&\3"+
		"&\3&\3&\3&\3&\3&\3&\3&\5&\u017d\n&\3\'\3\'\3\'\3\'\7\'\u0183\n\'\f\'\16"+
		"\'\u0186\13\'\3\'\3\'\3\'\3\'\6\'\u018c\n\'\r\'\16\'\u018d\5\'\u0190\n"+
		"\'\3(\6(\u0193\n(\r(\16(\u0194\3)\6)\u0198\n)\r)\16)\u0199\3)\3)\3*\3"+
		"*\3*\3*\3*\7*\u01a3\n*\f*\16*\u01a6\13*\3*\3*\3*\3*\3*\3+\3+\3+\3+\7+"+
		"\u01b1\n+\f+\16+\u01b4\13+\3+\3+\3+\3+\3,\3,\3-\3-\4\u01a4\u01b2\2.\3"+
		"\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f\27\r\31\16\33\17\35\20\37"+
		"\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63\33\65\34\67\359\36;\37="+
		" ?!A\"C#E$G%I&K\'M(O)Q*S+U,W\2Y\2\3\2\4\5\2\13\f\17\17\"\"\4\2C\\c|\2"+
		"\u01e0\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2"+
		"\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3"+
		"\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2"+
		"\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2\2\2"+
		"/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3\2\2"+
		"\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2\2\2"+
		"G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2Q\3\2\2\2\2S\3"+
		"\2\2\2\2U\3\2\2\2\3[\3\2\2\2\5]\3\2\2\2\7_\3\2\2\2\ta\3\2\2\2\13c\3\2"+
		"\2\2\re\3\2\2\2\17g\3\2\2\2\21j\3\2\2\2\23m\3\2\2\2\25p\3\2\2\2\27s\3"+
		"\2\2\2\31u\3\2\2\2\33w\3\2\2\2\35y\3\2\2\2\37{\3\2\2\2!}\3\2\2\2#\177"+
		"\3\2\2\2%\u0081\3\2\2\2\'\u0083\3\2\2\2)\u0087\3\2\2\2+\u008b\3\2\2\2"+
		"-\u00a1\3\2\2\2/\u00b1\3\2\2\2\61\u00b3\3\2\2\2\63\u00bc\3\2\2\2\65\u00c2"+
		"\3\2\2\2\67\u00c5\3\2\2\29\u00c8\3\2\2\2;\u00ce\3\2\2\2=\u00d5\3\2\2\2"+
		"?\u00d9\3\2\2\2A\u00de\3\2\2\2C\u00e2\3\2\2\2E\u00f0\3\2\2\2G\u0108\3"+
		"\2\2\2I\u015a\3\2\2\2K\u017c\3\2\2\2M\u018f\3\2\2\2O\u0192\3\2\2\2Q\u0197"+
		"\3\2\2\2S\u019d\3\2\2\2U\u01ac\3\2\2\2W\u01b9\3\2\2\2Y\u01bb\3\2\2\2["+
		"\\\7\60\2\2\\\4\3\2\2\2]^\7.\2\2^\6\3\2\2\2_`\7-\2\2`\b\3\2\2\2ab\7/\2"+
		"\2b\n\3\2\2\2cd\7,\2\2d\f\3\2\2\2ef\7\61\2\2f\16\3\2\2\2gh\7?\2\2hi\7"+
		"?\2\2i\20\3\2\2\2jk\7>\2\2kl\7?\2\2l\22\3\2\2\2mn\7@\2\2no\7?\2\2o\24"+
		"\3\2\2\2pq\7#\2\2qr\7?\2\2r\26\3\2\2\2st\7@\2\2t\30\3\2\2\2uv\7>\2\2v"+
		"\32\3\2\2\2wx\7*\2\2x\34\3\2\2\2yz\7+\2\2z\36\3\2\2\2{|\7}\2\2| \3\2\2"+
		"\2}~\7\177\2\2~\"\3\2\2\2\177\u0080\7=\2\2\u0080$\3\2\2\2\u0081\u0082"+
		"\7?\2\2\u0082&\3\2\2\2\u0083\u0084\7o\2\2\u0084\u0085\7c\2\2\u0085\u0086"+
		"\7z\2\2\u0086(\3\2\2\2\u0087\u0088\7o\2\2\u0088\u0089\7k\2\2\u0089\u008a"+
		"\7p\2\2\u008a*\3\2\2\2\u008b\u008c\7k\2\2\u008c\u008d\7p\2\2\u008d\u008e"+
		"\7k\2\2\u008e\u008f\7v\2\2\u008f\u0090\7k\2\2\u0090\u0091\7c\2\2\u0091"+
		"\u0092\7n\2\2\u0092,\3\2\2\2\u0093\u0094\7r\2\2\u0094\u0095\7m\2\2\u0095"+
		"\u0096\7v\2\2\u0096\u0097\7T\2\2\u0097\u0098\7e\2\2\u0098\u0099\7x\2\2"+
		"\u0099\u00a2\7f\2\2\u009a\u009b\7v\2\2\u009b\u009c\7k\2\2\u009c\u009d"+
		"\7o\2\2\u009d\u009e\7g\2\2\u009e\u009f\7q\2\2\u009f\u00a0\7w\2\2\u00a0"+
		"\u00a2\7v\2\2\u00a1\u0093\3\2\2\2\u00a1\u009a\3\2\2\2\u00a2.\3\2\2\2\u00a3"+
		"\u00a4\7f\2\2\u00a4\u00a5\7c\2\2\u00a5\u00a6\7v\2\2\u00a6\u00a7\7c\2\2"+
		"\u00a7\u00b2\7\62\2\2\u00a8\u00a9\7f\2\2\u00a9\u00aa\7c\2\2\u00aa\u00ab"+
		"\7v\2\2\u00ab\u00ac\7c\2\2\u00ac\u00b2\7\63\2\2\u00ad\u00ae\7r\2\2\u00ae"+
		"\u00af\7q\2\2\u00af\u00b0\7t\2\2\u00b0\u00b2\7v\2\2\u00b1\u00a3\3\2\2"+
		"\2\u00b1\u00a8\3\2\2\2\u00b1\u00ad\3\2\2\2\u00b2\60\3\2\2\2\u00b3\u00b4"+
		"\7T\2\2\u00b4\u00b5\7g\2\2\u00b5\u00b6\7i\2\2\u00b6\u00b7\7k\2\2\u00b7"+
		"\u00b8\7u\2\2\u00b8\u00b9\7v\2\2\u00b9\u00ba\7g\2\2\u00ba\u00bb\7t\2\2"+
		"\u00bb\62\3\2\2\2\u00bc\u00bd\7U\2\2\u00bd\u00be\7v\2\2\u00be\u00bf\7"+
		"c\2\2\u00bf\u00c0\7v\2\2\u00c0\u00c1\7g\2\2\u00c1\64\3\2\2\2\u00c2\u00c3"+
		"\7k\2\2\u00c3\u00c4\7h\2\2\u00c4\66\3\2\2\2\u00c5\u00c6\7q\2\2\u00c6\u00c7"+
		"\7p\2\2\u00c78\3\2\2\2\u00c8\u00c9\7G\2\2\u00c9\u00ca\7x\2\2\u00ca\u00cb"+
		"\7g\2\2\u00cb\u00cc\7p\2\2\u00cc\u00cd\7v\2\2\u00cd:\3\2\2\2\u00ce\u00cf"+
		"\7C\2\2\u00cf\u00d0\7e\2\2\u00d0\u00d1\7v\2\2\u00d1\u00d2\7k\2\2\u00d2"+
		"\u00d3\7q\2\2\u00d3\u00d4\7p\2\2\u00d4<\3\2\2\2\u00d5\u00d6\7g\2\2\u00d6"+
		"\u00d7\7v\2\2\u00d7\u00d8\7j\2\2\u00d8>\3\2\2\2\u00d9\u00da\7k\2\2\u00da"+
		"\u00db\7r\2\2\u00db\u00dc\7x\2\2\u00dc\u00dd\7\66\2\2\u00dd@\3\2\2\2\u00de"+
		"\u00df\7v\2\2\u00df\u00e0\7e\2\2\u00e0\u00e1\7r\2\2\u00e1B\3\2\2\2\u00e2"+
		"\u00e3\7w\2\2\u00e3\u00e4\7f\2\2\u00e4\u00e5\7r\2\2\u00e5D\3\2\2\2\u00e6"+
		"\u00e7\7u\2\2\u00e7\u00e8\7t\2\2\u00e8\u00f1\7e\2\2\u00e9\u00ea\7f\2\2"+
		"\u00ea\u00eb\7u\2\2\u00eb\u00f1\7v\2\2\u00ec\u00ed\7v\2\2\u00ed\u00ee"+
		"\7{\2\2\u00ee\u00ef\7r\2\2\u00ef\u00f1\7g\2\2\u00f0\u00e6\3\2\2\2\u00f0"+
		"\u00e9\3\2\2\2\u00f0\u00ec\3\2\2\2\u00f1F\3\2\2\2\u00f2\u00f3\7u\2\2\u00f3"+
		"\u00f4\7t\2\2\u00f4\u0109\7e\2\2\u00f5\u00f6\7f\2\2\u00f6\u00f7\7u\2\2"+
		"\u00f7\u0109\7v\2\2\u00f8\u00f9\7v\2\2\u00f9\u00fa\7v\2\2\u00fa\u0109"+
		"\7n\2\2\u00fb\u00fc\7r\2\2\u00fc\u00fd\7t\2\2\u00fd\u00fe\7q\2\2\u00fe"+
		"\u00ff\7v\2\2\u00ff\u0109\7q\2\2\u0100\u0101\7e\2\2\u0101\u0102\7j\2\2"+
		"\u0102\u0103\7g\2\2\u0103\u0104\7e\2\2\u0104\u0105\7m\2\2\u0105\u0106"+
		"\7u\2\2\u0106\u0107\7w\2\2\u0107\u0109\7o\2\2\u0108\u00f2\3\2\2\2\u0108"+
		"\u00f5\3\2\2\2\u0108\u00f8\3\2\2\2\u0108\u00fb\3\2\2\2\u0108\u0100\3\2"+
		"\2\2\u0109H\3\2\2\2\u010a\u010b\7u\2\2\u010b\u010c\7r\2\2\u010c\u010d"+
		"\7q\2\2\u010d\u010e\7t\2\2\u010e\u015b\7v\2\2\u010f\u0110\7f\2\2\u0110"+
		"\u0111\7r\2\2\u0111\u0112\7q\2\2\u0112\u0113\7t\2\2\u0113\u015b\7v\2\2"+
		"\u0114\u0115\7u\2\2\u0115\u0116\7g\2\2\u0116\u0117\7s\2\2\u0117\u0118"+
		"\7P\2\2\u0118\u015b\7q\2\2\u0119\u011a\7c\2\2\u011a\u011b\7e\2\2\u011b"+
		"\u011c\7m\2\2\u011c\u011d\7P\2\2\u011d\u015b\7q\2\2\u011e\u011f\7y\2\2"+
		"\u011f\u0120\7k\2\2\u0120\u0121\7p\2\2\u0121\u0122\7f\2\2\u0122\u0123"+
		"\7q\2\2\u0123\u015b\7y\2\2\u0124\u0125\7h\2\2\u0125\u0126\7n\2\2\u0126"+
		"\u0127\7c\2\2\u0127\u0128\7i\2\2\u0128\u015b\7u\2\2\u0129\u012a\7e\2\2"+
		"\u012a\u012b\7j\2\2\u012b\u012c\7g\2\2\u012c\u012d\7e\2\2\u012d\u012e"+
		"\7m\2\2\u012e\u012f\7u\2\2\u012f\u0130\7w\2\2\u0130\u015b\7o\2\2\u0131"+
		"\u0132\7v\2\2\u0132\u0133\7k\2\2\u0133\u0134\7o\2\2\u0134\u0135\7g\2\2"+
		"\u0135\u0136\7u\2\2\u0136\u0137\7v\2\2\u0137\u0138\7c\2\2\u0138\u0139"+
		"\7o\2\2\u0139\u015b\7r\2\2\u013a\u013b\7v\2\2\u013b\u013c\7k\2\2\u013c"+
		"\u013d\7o\2\2\u013d\u013e\7g\2\2\u013e\u013f\7u\2\2\u013f\u0140\7v\2\2"+
		"\u0140\u0141\7c\2\2\u0141\u0142\7o\2\2\u0142\u0143\7r\2\2\u0143\u0144"+
		"\7G\2\2\u0144\u0145\7e\2\2\u0145\u0146\7j\2\2\u0146\u0147\7q\2\2\u0147"+
		"\u0148\7T\2\2\u0148\u0149\7g\2\2\u0149\u014a\7r\2\2\u014a\u014b\7n\2\2"+
		"\u014b\u015b\7{\2\2\u014c\u014d\7o\2\2\u014d\u014e\7u\2\2\u014e\u015b"+
		"\7u\2\2\u014f\u0150\7y\2\2\u0150\u0151\7k\2\2\u0151\u0152\7p\2\2\u0152"+
		"\u0153\7f\2\2\u0153\u0154\7q\2\2\u0154\u0155\7y\2\2\u0155\u0156\7U\2\2"+
		"\u0156\u0157\7e\2\2\u0157\u0158\7c\2\2\u0158\u0159\7n\2\2\u0159\u015b"+
		"\7g\2\2\u015a\u010a\3\2\2\2\u015a\u010f\3\2\2\2\u015a\u0114\3\2\2\2\u015a"+
		"\u0119\3\2\2\2\u015a\u011e\3\2\2\2\u015a\u0124\3\2\2\2\u015a\u0129\3\2"+
		"\2\2\u015a\u0131\3\2\2\2\u015a\u013a\3\2\2\2\u015a\u014c\3\2\2\2\u015a"+
		"\u014f\3\2\2\2\u015bJ\3\2\2\2\u015c\u015d\7u\2\2\u015d\u015e\7r\2\2\u015e"+
		"\u015f\7q\2\2\u015f\u0160\7t\2\2\u0160\u017d\7v\2\2\u0161\u0162\7f\2\2"+
		"\u0162\u0163\7r\2\2\u0163\u0164\7q\2\2\u0164\u0165\7t\2\2\u0165\u017d"+
		"\7v\2\2\u0166\u0167\7e\2\2\u0167\u0168\7j\2\2\u0168\u0169\7g\2\2\u0169"+
		"\u016a\7e\2\2\u016a\u016b\7m\2\2\u016b\u016c\7u\2\2\u016c\u016d\7w\2\2"+
		"\u016d\u017d\7o\2\2\u016e\u016f\7n\2\2\u016f\u0170\7g\2\2\u0170\u0171"+
		"\7p\2\2\u0171\u0172\7i\2\2\u0172\u0173\7v\2\2\u0173\u017d\7j\2\2\u0174"+
		"\u0175\7r\2\2\u0175\u017d\7\63\2\2\u0176\u0177\7r\2\2\u0177\u017d\7\64"+
		"\2\2\u0178\u0179\7r\2\2\u0179\u017d\7\65\2\2\u017a\u017b\7r\2\2\u017b"+
		"\u017d\7\66\2\2\u017c\u015c\3\2\2\2\u017c\u0161\3\2\2\2\u017c\u0166\3"+
		"\2\2\2\u017c\u016e\3\2\2\2\u017c\u0174\3\2\2\2\u017c\u0176\3\2\2\2\u017c"+
		"\u0178\3\2\2\2\u017c\u017a\3\2\2\2\u017dL\3\2\2\2\u017e\u0184\5W,\2\u017f"+
		"\u0183\5W,\2\u0180\u0183\5Y-\2\u0181\u0183\7a\2\2\u0182\u017f\3\2\2\2"+
		"\u0182\u0180\3\2\2\2\u0182\u0181\3\2\2\2\u0183\u0186\3\2\2\2\u0184\u0182"+
		"\3\2\2\2\u0184\u0185\3\2\2\2\u0185\u0190\3\2\2\2\u0186\u0184\3\2\2\2\u0187"+
		"\u018b\7a\2\2\u0188\u018c\5W,\2\u0189\u018c\5Y-\2\u018a\u018c\7a\2\2\u018b"+
		"\u0188\3\2\2\2\u018b\u0189\3\2\2\2\u018b\u018a\3\2\2\2\u018c\u018d\3\2"+
		"\2\2\u018d\u018b\3\2\2\2\u018d\u018e\3\2\2\2\u018e\u0190\3\2\2\2\u018f"+
		"\u017e\3\2\2\2\u018f\u0187\3\2\2\2\u0190N\3\2\2\2\u0191\u0193\5Y-\2\u0192"+
		"\u0191\3\2\2\2\u0193\u0194\3\2\2\2\u0194\u0192\3\2\2\2\u0194\u0195\3\2"+
		"\2\2\u0195P\3\2\2\2\u0196\u0198\t\2\2\2\u0197\u0196\3\2\2\2\u0198\u0199"+
		"\3\2\2\2\u0199\u0197\3\2\2\2\u0199\u019a\3\2\2\2\u019a\u019b\3\2\2\2\u019b"+
		"\u019c\b)\2\2\u019cR\3\2\2\2\u019d\u019e\7\61\2\2\u019e\u019f\7,\2\2\u019f"+
		"\u01a4\3\2\2\2\u01a0\u01a3\5S*\2\u01a1\u01a3\13\2\2\2\u01a2\u01a0\3\2"+
		"\2\2\u01a2\u01a1\3\2\2\2\u01a3\u01a6\3\2\2\2\u01a4\u01a5\3\2\2\2\u01a4"+
		"\u01a2\3\2\2\2\u01a5\u01a7\3\2\2\2\u01a6\u01a4\3\2\2\2\u01a7\u01a8\7,"+
		"\2\2\u01a8\u01a9\7\61\2\2\u01a9\u01aa\3\2\2\2\u01aa\u01ab\b*\2\2\u01ab"+
		"T\3\2\2\2\u01ac\u01ad\7\61\2\2\u01ad\u01ae\7\61\2\2\u01ae\u01b2\3\2\2"+
		"\2\u01af\u01b1\13\2\2\2\u01b0\u01af\3\2\2\2\u01b1\u01b4\3\2\2\2\u01b2"+
		"\u01b3\3\2\2\2\u01b2\u01b0\3\2\2\2\u01b3\u01b5\3\2\2\2\u01b4\u01b2\3\2"+
		"\2\2\u01b5\u01b6\7\f\2\2\u01b6\u01b7\3\2\2\2\u01b7\u01b8\b+\2\2\u01b8"+
		"V\3\2\2\2\u01b9\u01ba\t\3\2\2\u01baX\3\2\2\2\u01bb\u01bc\4\62;\2\u01bc"+
		"Z\3\2\2\2\23\2\u00a1\u00b1\u00f0\u0108\u015a\u017c\u0182\u0184\u018b\u018d"+
		"\u018f\u0194\u0199\u01a2\u01a4\u01b2\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}