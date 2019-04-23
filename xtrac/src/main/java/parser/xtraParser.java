// Generated from /home/giacomo/IdeaProjects/XTRAC/src/main/java/xtra.g4 by ANTLR 4.7
package parser;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class xtraParser extends Parser {
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
	public static final int
		RULE_program = 0, RULE_body_element = 1, RULE_declaration = 2, RULE_function = 3, 
		RULE_event_statement = 4, RULE_event_body = 5, RULE_state_declaration = 6, 
		RULE_state_body = 7, RULE_condition = 8, RULE_conditions_statement = 9, 
		RULE_conditions_body = 10, RULE_custom_event_definition = 11, RULE_macro_action_definition = 12, 
		RULE_macro_action_body = 13, RULE_expr = 14, RULE_assignment = 15, RULE_term = 16, 
		RULE_header_type = 17, RULE_header_field_name = 18, RULE_header_field = 19, 
		RULE_identifier = 20, RULE_integer = 21, RULE_event_field = 22;
	public static final String[] ruleNames = {
		"program", "body_element", "declaration", "function", "event_statement", 
		"event_body", "state_declaration", "state_body", "condition", "conditions_statement", 
		"conditions_body", "custom_event_definition", "macro_action_definition", 
		"macro_action_body", "expr", "assignment", "term", "header_type", "header_field_name", 
		"header_field", "identifier", "integer", "event_field"
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

	@Override
	public String getGrammarFileName() { return "xtra.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public xtraParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class ProgramContext extends ParserRuleContext {
		public List<DeclarationContext> declaration() {
			return getRuleContexts(DeclarationContext.class);
		}
		public DeclarationContext declaration(int i) {
			return getRuleContext(DeclarationContext.class,i);
		}
		public List<Custom_event_definitionContext> custom_event_definition() {
			return getRuleContexts(Custom_event_definitionContext.class);
		}
		public Custom_event_definitionContext custom_event_definition(int i) {
			return getRuleContext(Custom_event_definitionContext.class,i);
		}
		public List<Macro_action_definitionContext> macro_action_definition() {
			return getRuleContexts(Macro_action_definitionContext.class);
		}
		public Macro_action_definitionContext macro_action_definition(int i) {
			return getRuleContext(Macro_action_definitionContext.class,i);
		}
		public List<Body_elementContext> body_element() {
			return getRuleContexts(Body_elementContext.class);
		}
		public Body_elementContext body_element(int i) {
			return getRuleContext(Body_elementContext.class,i);
		}
		public List<State_declarationContext> state_declaration() {
			return getRuleContexts(State_declarationContext.class);
		}
		public State_declarationContext state_declaration(int i) {
			return getRuleContext(State_declarationContext.class,i);
		}
		public ProgramContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_program; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterProgram(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitProgram(this);
		}
	}

	public final ProgramContext program() throws RecognitionException {
		ProgramContext _localctx = new ProgramContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_program);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(49);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==REGISTER_DECLARER) {
				{
				{
				setState(46);
				declaration();
				}
				}
				setState(51);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(56);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==CUSTOM_EVENT_DECLARER || _la==MACRO_ACTION_DECLARER) {
				{
				setState(54);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CUSTOM_EVENT_DECLARER:
					{
					setState(52);
					custom_event_definition();
					}
					break;
				case MACRO_ACTION_DECLARER:
					{
					setState(53);
					macro_action_definition();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(58);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(62);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==ID) {
				{
				{
				setState(59);
				body_element();
				}
				}
				setState(64);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(68);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==STATE_DECLARER) {
				{
				{
				setState(65);
				state_declaration();
				}
				}
				setState(70);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Body_elementContext extends ParserRuleContext {
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public FunctionContext function() {
			return getRuleContext(FunctionContext.class,0);
		}
		public Body_elementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_body_element; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterBody_element(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitBody_element(this);
		}
	}

	public final Body_elementContext body_element() throws RecognitionException {
		Body_elementContext _localctx = new Body_elementContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_body_element);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(73);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				{
				setState(71);
				assignment();
				}
				break;
			case 2:
				{
				setState(72);
				function();
				}
				break;
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclarationContext extends ParserRuleContext {
		public TerminalNode REGISTER_DECLARER() { return getToken(xtraParser.REGISTER_DECLARER, 0); }
		public List<IdentifierContext> identifier() {
			return getRuleContexts(IdentifierContext.class);
		}
		public IdentifierContext identifier(int i) {
			return getRuleContext(IdentifierContext.class,i);
		}
		public TerminalNode SEMICOLON() { return getToken(xtraParser.SEMICOLON, 0); }
		public List<TerminalNode> COMMA() { return getTokens(xtraParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(xtraParser.COMMA, i);
		}
		public DeclarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declaration; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterDeclaration(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitDeclaration(this);
		}
	}

	public final DeclarationContext declaration() throws RecognitionException {
		DeclarationContext _localctx = new DeclarationContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_declaration);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(75);
			match(REGISTER_DECLARER);
			setState(76);
			identifier();
			setState(81);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(77);
				match(COMMA);
				setState(78);
				identifier();
				}
				}
				setState(83);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(84);
			match(SEMICOLON);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunctionContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(xtraParser.ID, 0); }
		public TerminalNode LPARENT() { return getToken(xtraParser.LPARENT, 0); }
		public TerminalNode RPARENT() { return getToken(xtraParser.RPARENT, 0); }
		public TerminalNode SEMICOLON() { return getToken(xtraParser.SEMICOLON, 0); }
		public List<TermContext> term() {
			return getRuleContexts(TermContext.class);
		}
		public TermContext term(int i) {
			return getRuleContext(TermContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(xtraParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(xtraParser.COMMA, i);
		}
		public FunctionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_function; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterFunction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitFunction(this);
		}
	}

	public final FunctionContext function() throws RecognitionException {
		FunctionContext _localctx = new FunctionContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_function);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(86);
			match(ID);
			setState(87);
			match(LPARENT);
			setState(91);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << EVENT) | (1L << ETH) | (1L << IPV4) | (1L << TCP) | (1L << UDP) | (1L << ID) | (1L << NUMBER))) != 0)) {
				{
				{
				setState(88);
				term();
				}
				}
				setState(93);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(98);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(94);
				match(COMMA);
				setState(95);
				term();
				}
				}
				setState(100);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(101);
			match(RPARENT);
			setState(102);
			match(SEMICOLON);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Event_statementContext extends ParserRuleContext {
		public TerminalNode EVENT_DECLARER() { return getToken(xtraParser.EVENT_DECLARER, 0); }
		public TerminalNode LPARENT() { return getToken(xtraParser.LPARENT, 0); }
		public TerminalNode EVENT() { return getToken(xtraParser.EVENT, 0); }
		public TerminalNode RPARENT() { return getToken(xtraParser.RPARENT, 0); }
		public Event_bodyContext event_body() {
			return getRuleContext(Event_bodyContext.class,0);
		}
		public Event_statementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_event_statement; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterEvent_statement(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitEvent_statement(this);
		}
	}

	public final Event_statementContext event_statement() throws RecognitionException {
		Event_statementContext _localctx = new Event_statementContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_event_statement);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(104);
			match(EVENT_DECLARER);
			setState(105);
			match(LPARENT);
			setState(106);
			match(EVENT);
			setState(107);
			match(RPARENT);
			setState(108);
			event_body();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Event_bodyContext extends ParserRuleContext {
		public TerminalNode LBRACE() { return getToken(xtraParser.LBRACE, 0); }
		public TerminalNode RBRACE() { return getToken(xtraParser.RBRACE, 0); }
		public List<Conditions_statementContext> conditions_statement() {
			return getRuleContexts(Conditions_statementContext.class);
		}
		public Conditions_statementContext conditions_statement(int i) {
			return getRuleContext(Conditions_statementContext.class,i);
		}
		public List<Body_elementContext> body_element() {
			return getRuleContexts(Body_elementContext.class);
		}
		public Body_elementContext body_element(int i) {
			return getRuleContext(Body_elementContext.class,i);
		}
		public Event_bodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_event_body; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterEvent_body(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitEvent_body(this);
		}
	}

	public final Event_bodyContext event_body() throws RecognitionException {
		Event_bodyContext _localctx = new Event_bodyContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_event_body);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(110);
			match(LBRACE);
			setState(115);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==CONDITIONS_DECLARER || _la==ID) {
				{
				setState(113);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case CONDITIONS_DECLARER:
					{
					setState(111);
					conditions_statement();
					}
					break;
				case ID:
					{
					setState(112);
					body_element();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(117);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(118);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class State_declarationContext extends ParserRuleContext {
		public TerminalNode STATE_DECLARER() { return getToken(xtraParser.STATE_DECLARER, 0); }
		public TerminalNode ID() { return getToken(xtraParser.ID, 0); }
		public State_bodyContext state_body() {
			return getRuleContext(State_bodyContext.class,0);
		}
		public TerminalNode INITIAL_MODIFIER() { return getToken(xtraParser.INITIAL_MODIFIER, 0); }
		public State_declarationContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_state_declaration; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterState_declaration(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitState_declaration(this);
		}
	}

	public final State_declarationContext state_declaration() throws RecognitionException {
		State_declarationContext _localctx = new State_declarationContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_state_declaration);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(120);
			match(STATE_DECLARER);
			setState(122);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==INITIAL_MODIFIER) {
				{
				setState(121);
				match(INITIAL_MODIFIER);
				}
			}

			setState(124);
			match(ID);
			setState(125);
			state_body();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class State_bodyContext extends ParserRuleContext {
		public TerminalNode LBRACE() { return getToken(xtraParser.LBRACE, 0); }
		public TerminalNode RBRACE() { return getToken(xtraParser.RBRACE, 0); }
		public List<Body_elementContext> body_element() {
			return getRuleContexts(Body_elementContext.class);
		}
		public Body_elementContext body_element(int i) {
			return getRuleContext(Body_elementContext.class,i);
		}
		public List<Event_statementContext> event_statement() {
			return getRuleContexts(Event_statementContext.class);
		}
		public Event_statementContext event_statement(int i) {
			return getRuleContext(Event_statementContext.class,i);
		}
		public State_bodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_state_body; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterState_body(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitState_body(this);
		}
	}

	public final State_bodyContext state_body() throws RecognitionException {
		State_bodyContext _localctx = new State_bodyContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_state_body);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(127);
			match(LBRACE);
			setState(132);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==EVENT_DECLARER || _la==ID) {
				{
				setState(130);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case ID:
					{
					setState(128);
					body_element();
					}
					break;
				case EVENT_DECLARER:
					{
					setState(129);
					event_statement();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(134);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(135);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ConditionContext extends ParserRuleContext {
		public List<TermContext> term() {
			return getRuleContexts(TermContext.class);
		}
		public TermContext term(int i) {
			return getRuleContext(TermContext.class,i);
		}
		public TerminalNode EQ() { return getToken(xtraParser.EQ, 0); }
		public TerminalNode LEQ() { return getToken(xtraParser.LEQ, 0); }
		public TerminalNode GEQ() { return getToken(xtraParser.GEQ, 0); }
		public TerminalNode NEQ() { return getToken(xtraParser.NEQ, 0); }
		public TerminalNode GRET() { return getToken(xtraParser.GRET, 0); }
		public TerminalNode LESS() { return getToken(xtraParser.LESS, 0); }
		public ConditionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_condition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterCondition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitCondition(this);
		}
	}

	public final ConditionContext condition() throws RecognitionException {
		ConditionContext _localctx = new ConditionContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_condition);
		try {
			setState(161);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,14,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(137);
				term();
				setState(138);
				match(EQ);
				setState(139);
				term();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(141);
				term();
				setState(142);
				match(LEQ);
				setState(143);
				term();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(145);
				term();
				setState(146);
				match(GEQ);
				setState(147);
				term();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(149);
				term();
				setState(150);
				match(NEQ);
				setState(151);
				term();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(153);
				term();
				setState(154);
				match(GRET);
				setState(155);
				term();
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(157);
				term();
				setState(158);
				match(LESS);
				setState(159);
				term();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Conditions_statementContext extends ParserRuleContext {
		public TerminalNode CONDITIONS_DECLARER() { return getToken(xtraParser.CONDITIONS_DECLARER, 0); }
		public TerminalNode LPARENT() { return getToken(xtraParser.LPARENT, 0); }
		public List<ConditionContext> condition() {
			return getRuleContexts(ConditionContext.class);
		}
		public ConditionContext condition(int i) {
			return getRuleContext(ConditionContext.class,i);
		}
		public TerminalNode RPARENT() { return getToken(xtraParser.RPARENT, 0); }
		public Conditions_bodyContext conditions_body() {
			return getRuleContext(Conditions_bodyContext.class,0);
		}
		public List<TerminalNode> COMMA() { return getTokens(xtraParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(xtraParser.COMMA, i);
		}
		public Conditions_statementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditions_statement; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterConditions_statement(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitConditions_statement(this);
		}
	}

	public final Conditions_statementContext conditions_statement() throws RecognitionException {
		Conditions_statementContext _localctx = new Conditions_statementContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_conditions_statement);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(163);
			match(CONDITIONS_DECLARER);
			setState(164);
			match(LPARENT);
			setState(165);
			condition();
			setState(170);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(166);
				match(COMMA);
				setState(167);
				condition();
				}
				}
				setState(172);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(173);
			match(RPARENT);
			setState(174);
			conditions_body();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Conditions_bodyContext extends ParserRuleContext {
		public TerminalNode LBRACE() { return getToken(xtraParser.LBRACE, 0); }
		public TerminalNode RBRACE() { return getToken(xtraParser.RBRACE, 0); }
		public List<Body_elementContext> body_element() {
			return getRuleContexts(Body_elementContext.class);
		}
		public Body_elementContext body_element(int i) {
			return getRuleContext(Body_elementContext.class,i);
		}
		public Conditions_bodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_conditions_body; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterConditions_body(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitConditions_body(this);
		}
	}

	public final Conditions_bodyContext conditions_body() throws RecognitionException {
		Conditions_bodyContext _localctx = new Conditions_bodyContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_conditions_body);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(176);
			match(LBRACE);
			setState(180);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==ID) {
				{
				{
				setState(177);
				body_element();
				}
				}
				setState(182);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(183);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Custom_event_definitionContext extends ParserRuleContext {
		public TerminalNode CUSTOM_EVENT_DECLARER() { return getToken(xtraParser.CUSTOM_EVENT_DECLARER, 0); }
		public TerminalNode ID() { return getToken(xtraParser.ID, 0); }
		public TerminalNode ASSIGNMENT() { return getToken(xtraParser.ASSIGNMENT, 0); }
		public List<ConditionContext> condition() {
			return getRuleContexts(ConditionContext.class);
		}
		public ConditionContext condition(int i) {
			return getRuleContext(ConditionContext.class,i);
		}
		public TerminalNode SEMICOLON() { return getToken(xtraParser.SEMICOLON, 0); }
		public List<TerminalNode> COMMA() { return getTokens(xtraParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(xtraParser.COMMA, i);
		}
		public Custom_event_definitionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_custom_event_definition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterCustom_event_definition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitCustom_event_definition(this);
		}
	}

	public final Custom_event_definitionContext custom_event_definition() throws RecognitionException {
		Custom_event_definitionContext _localctx = new Custom_event_definitionContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_custom_event_definition);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(185);
			match(CUSTOM_EVENT_DECLARER);
			setState(186);
			match(ID);
			setState(187);
			match(ASSIGNMENT);
			setState(188);
			condition();
			setState(193);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(189);
				match(COMMA);
				setState(190);
				condition();
				}
				}
				setState(195);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(196);
			match(SEMICOLON);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Macro_action_definitionContext extends ParserRuleContext {
		public TerminalNode MACRO_ACTION_DECLARER() { return getToken(xtraParser.MACRO_ACTION_DECLARER, 0); }
		public TerminalNode ID() { return getToken(xtraParser.ID, 0); }
		public Macro_action_bodyContext macro_action_body() {
			return getRuleContext(Macro_action_bodyContext.class,0);
		}
		public Macro_action_definitionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_macro_action_definition; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterMacro_action_definition(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitMacro_action_definition(this);
		}
	}

	public final Macro_action_definitionContext macro_action_definition() throws RecognitionException {
		Macro_action_definitionContext _localctx = new Macro_action_definitionContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_macro_action_definition);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(198);
			match(MACRO_ACTION_DECLARER);
			setState(199);
			match(ID);
			setState(200);
			macro_action_body();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Macro_action_bodyContext extends ParserRuleContext {
		public TerminalNode LBRACE() { return getToken(xtraParser.LBRACE, 0); }
		public TerminalNode RBRACE() { return getToken(xtraParser.RBRACE, 0); }
		public List<Body_elementContext> body_element() {
			return getRuleContexts(Body_elementContext.class);
		}
		public Body_elementContext body_element(int i) {
			return getRuleContext(Body_elementContext.class,i);
		}
		public Macro_action_bodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_macro_action_body; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterMacro_action_body(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitMacro_action_body(this);
		}
	}

	public final Macro_action_bodyContext macro_action_body() throws RecognitionException {
		Macro_action_bodyContext _localctx = new Macro_action_bodyContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_macro_action_body);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(202);
			match(LBRACE);
			setState(204); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(203);
				body_element();
				}
				}
				setState(206); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( _la==ID );
			setState(208);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExprContext extends ParserRuleContext {
		public List<TermContext> term() {
			return getRuleContexts(TermContext.class);
		}
		public TermContext term(int i) {
			return getRuleContext(TermContext.class,i);
		}
		public TerminalNode MUL() { return getToken(xtraParser.MUL, 0); }
		public TerminalNode DIV() { return getToken(xtraParser.DIV, 0); }
		public TerminalNode PLUS() { return getToken(xtraParser.PLUS, 0); }
		public TerminalNode MINUS() { return getToken(xtraParser.MINUS, 0); }
		public TerminalNode LPARENT() { return getToken(xtraParser.LPARENT, 0); }
		public TerminalNode COMMA() { return getToken(xtraParser.COMMA, 0); }
		public TerminalNode RPARENT() { return getToken(xtraParser.RPARENT, 0); }
		public TerminalNode MAX() { return getToken(xtraParser.MAX, 0); }
		public TerminalNode MIN() { return getToken(xtraParser.MIN, 0); }
		public ExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterExpr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitExpr(this);
		}
	}

	public final ExprContext expr() throws RecognitionException {
		ExprContext _localctx = new ExprContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_expr);
		int _la;
		try {
			setState(234);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,19,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(210);
				term();
				setState(211);
				match(MUL);
				setState(212);
				term();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(214);
				term();
				setState(215);
				match(DIV);
				setState(216);
				term();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(218);
				term();
				setState(219);
				match(PLUS);
				setState(220);
				term();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(222);
				term();
				setState(223);
				match(MINUS);
				setState(224);
				term();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(226);
				_la = _input.LA(1);
				if ( !(_la==MAX || _la==MIN) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(227);
				match(LPARENT);
				setState(228);
				term();
				setState(229);
				match(COMMA);
				setState(230);
				term();
				setState(231);
				match(RPARENT);
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(233);
				term();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AssignmentContext extends ParserRuleContext {
		public IdentifierContext identifier() {
			return getRuleContext(IdentifierContext.class,0);
		}
		public TerminalNode ASSIGNMENT() { return getToken(xtraParser.ASSIGNMENT, 0); }
		public ExprContext expr() {
			return getRuleContext(ExprContext.class,0);
		}
		public TerminalNode SEMICOLON() { return getToken(xtraParser.SEMICOLON, 0); }
		public AssignmentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assignment; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterAssignment(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitAssignment(this);
		}
	}

	public final AssignmentContext assignment() throws RecognitionException {
		AssignmentContext _localctx = new AssignmentContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_assignment);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(236);
			identifier();
			setState(237);
			match(ASSIGNMENT);
			setState(238);
			expr();
			setState(239);
			match(SEMICOLON);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TermContext extends ParserRuleContext {
		public IdentifierContext identifier() {
			return getRuleContext(IdentifierContext.class,0);
		}
		public IntegerContext integer() {
			return getRuleContext(IntegerContext.class,0);
		}
		public Header_fieldContext header_field() {
			return getRuleContext(Header_fieldContext.class,0);
		}
		public Event_fieldContext event_field() {
			return getRuleContext(Event_fieldContext.class,0);
		}
		public TermContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_term; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterTerm(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitTerm(this);
		}
	}

	public final TermContext term() throws RecognitionException {
		TermContext _localctx = new TermContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_term);
		try {
			setState(245);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ID:
				enterOuterAlt(_localctx, 1);
				{
				setState(241);
				identifier();
				}
				break;
			case NUMBER:
				enterOuterAlt(_localctx, 2);
				{
				setState(242);
				integer();
				}
				break;
			case ETH:
			case IPV4:
			case TCP:
			case UDP:
				enterOuterAlt(_localctx, 3);
				{
				setState(243);
				header_field();
				}
				break;
			case EVENT:
				enterOuterAlt(_localctx, 4);
				{
				setState(244);
				event_field();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Header_typeContext extends ParserRuleContext {
		public TerminalNode ETH() { return getToken(xtraParser.ETH, 0); }
		public TerminalNode IPV4() { return getToken(xtraParser.IPV4, 0); }
		public TerminalNode TCP() { return getToken(xtraParser.TCP, 0); }
		public TerminalNode UDP() { return getToken(xtraParser.UDP, 0); }
		public Header_typeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_header_type; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterHeader_type(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitHeader_type(this);
		}
	}

	public final Header_typeContext header_type() throws RecognitionException {
		Header_typeContext _localctx = new Header_typeContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_header_type);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(247);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << ETH) | (1L << IPV4) | (1L << TCP) | (1L << UDP))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Header_field_nameContext extends ParserRuleContext {
		public TerminalNode ETH_FIELD() { return getToken(xtraParser.ETH_FIELD, 0); }
		public TerminalNode IPV4_FIELD() { return getToken(xtraParser.IPV4_FIELD, 0); }
		public TerminalNode TCP_FIELD() { return getToken(xtraParser.TCP_FIELD, 0); }
		public TerminalNode UDP_FIELD() { return getToken(xtraParser.UDP_FIELD, 0); }
		public Header_field_nameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_header_field_name; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterHeader_field_name(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitHeader_field_name(this);
		}
	}

	public final Header_field_nameContext header_field_name() throws RecognitionException {
		Header_field_nameContext _localctx = new Header_field_nameContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_header_field_name);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(249);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << ETH_FIELD) | (1L << IPV4_FIELD) | (1L << TCP_FIELD) | (1L << UDP_FIELD))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Header_fieldContext extends ParserRuleContext {
		public Header_typeContext header_type() {
			return getRuleContext(Header_typeContext.class,0);
		}
		public Header_field_nameContext header_field_name() {
			return getRuleContext(Header_field_nameContext.class,0);
		}
		public Header_fieldContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_header_field; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterHeader_field(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitHeader_field(this);
		}
	}

	public final Header_fieldContext header_field() throws RecognitionException {
		Header_fieldContext _localctx = new Header_fieldContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_header_field);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(251);
			header_type();
			setState(252);
			match(T__0);
			setState(253);
			header_field_name();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IdentifierContext extends ParserRuleContext {
		public TerminalNode ID() { return getToken(xtraParser.ID, 0); }
		public IdentifierContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_identifier; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterIdentifier(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitIdentifier(this);
		}
	}

	public final IdentifierContext identifier() throws RecognitionException {
		IdentifierContext _localctx = new IdentifierContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_identifier);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(255);
			match(ID);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IntegerContext extends ParserRuleContext {
		public TerminalNode NUMBER() { return getToken(xtraParser.NUMBER, 0); }
		public IntegerContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_integer; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterInteger(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitInteger(this);
		}
	}

	public final IntegerContext integer() throws RecognitionException {
		IntegerContext _localctx = new IntegerContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_integer);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(257);
			match(NUMBER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class Event_fieldContext extends ParserRuleContext {
		public TerminalNode EVENT() { return getToken(xtraParser.EVENT, 0); }
		public TerminalNode EVENT_FIELD() { return getToken(xtraParser.EVENT_FIELD, 0); }
		public Event_fieldContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_event_field; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).enterEvent_field(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof xtraListener ) ((xtraListener)listener).exitEvent_field(this);
		}
	}

	public final Event_fieldContext event_field() throws RecognitionException {
		Event_fieldContext _localctx = new Event_fieldContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_event_field);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(259);
			match(EVENT);
			setState(260);
			match(T__0);
			setState(261);
			match(EVENT_FIELD);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3,\u010a\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\3\2\7\2\62"+
		"\n\2\f\2\16\2\65\13\2\3\2\3\2\7\29\n\2\f\2\16\2<\13\2\3\2\7\2?\n\2\f\2"+
		"\16\2B\13\2\3\2\7\2E\n\2\f\2\16\2H\13\2\3\3\3\3\5\3L\n\3\3\4\3\4\3\4\3"+
		"\4\7\4R\n\4\f\4\16\4U\13\4\3\4\3\4\3\5\3\5\3\5\7\5\\\n\5\f\5\16\5_\13"+
		"\5\3\5\3\5\7\5c\n\5\f\5\16\5f\13\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\6\3\6"+
		"\3\7\3\7\3\7\7\7t\n\7\f\7\16\7w\13\7\3\7\3\7\3\b\3\b\5\b}\n\b\3\b\3\b"+
		"\3\b\3\t\3\t\3\t\7\t\u0085\n\t\f\t\16\t\u0088\13\t\3\t\3\t\3\n\3\n\3\n"+
		"\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3\n\3"+
		"\n\3\n\3\n\3\n\5\n\u00a4\n\n\3\13\3\13\3\13\3\13\3\13\7\13\u00ab\n\13"+
		"\f\13\16\13\u00ae\13\13\3\13\3\13\3\13\3\f\3\f\7\f\u00b5\n\f\f\f\16\f"+
		"\u00b8\13\f\3\f\3\f\3\r\3\r\3\r\3\r\3\r\3\r\7\r\u00c2\n\r\f\r\16\r\u00c5"+
		"\13\r\3\r\3\r\3\16\3\16\3\16\3\16\3\17\3\17\6\17\u00cf\n\17\r\17\16\17"+
		"\u00d0\3\17\3\17\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20"+
		"\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\5\20"+
		"\u00ed\n\20\3\21\3\21\3\21\3\21\3\21\3\22\3\22\3\22\3\22\5\22\u00f8\n"+
		"\22\3\23\3\23\3\24\3\24\3\25\3\25\3\25\3\25\3\26\3\26\3\27\3\27\3\30\3"+
		"\30\3\30\3\30\3\30\2\2\31\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 \"$&"+
		"(*,.\2\5\3\2\25\26\3\2 #\3\2$\'\2\u0111\2\63\3\2\2\2\4K\3\2\2\2\6M\3\2"+
		"\2\2\bX\3\2\2\2\nj\3\2\2\2\fp\3\2\2\2\16z\3\2\2\2\20\u0081\3\2\2\2\22"+
		"\u00a3\3\2\2\2\24\u00a5\3\2\2\2\26\u00b2\3\2\2\2\30\u00bb\3\2\2\2\32\u00c8"+
		"\3\2\2\2\34\u00cc\3\2\2\2\36\u00ec\3\2\2\2 \u00ee\3\2\2\2\"\u00f7\3\2"+
		"\2\2$\u00f9\3\2\2\2&\u00fb\3\2\2\2(\u00fd\3\2\2\2*\u0101\3\2\2\2,\u0103"+
		"\3\2\2\2.\u0105\3\2\2\2\60\62\5\6\4\2\61\60\3\2\2\2\62\65\3\2\2\2\63\61"+
		"\3\2\2\2\63\64\3\2\2\2\64:\3\2\2\2\65\63\3\2\2\2\669\5\30\r\2\679\5\32"+
		"\16\28\66\3\2\2\28\67\3\2\2\29<\3\2\2\2:8\3\2\2\2:;\3\2\2\2;@\3\2\2\2"+
		"<:\3\2\2\2=?\5\4\3\2>=\3\2\2\2?B\3\2\2\2@>\3\2\2\2@A\3\2\2\2AF\3\2\2\2"+
		"B@\3\2\2\2CE\5\16\b\2DC\3\2\2\2EH\3\2\2\2FD\3\2\2\2FG\3\2\2\2G\3\3\2\2"+
		"\2HF\3\2\2\2IL\5 \21\2JL\5\b\5\2KI\3\2\2\2KJ\3\2\2\2L\5\3\2\2\2MN\7\32"+
		"\2\2NS\5*\26\2OP\7\4\2\2PR\5*\26\2QO\3\2\2\2RU\3\2\2\2SQ\3\2\2\2ST\3\2"+
		"\2\2TV\3\2\2\2US\3\2\2\2VW\7\23\2\2W\7\3\2\2\2XY\7(\2\2Y]\7\17\2\2Z\\"+
		"\5\"\22\2[Z\3\2\2\2\\_\3\2\2\2][\3\2\2\2]^\3\2\2\2^d\3\2\2\2_]\3\2\2\2"+
		"`a\7\4\2\2ac\5\"\22\2b`\3\2\2\2cf\3\2\2\2db\3\2\2\2de\3\2\2\2eg\3\2\2"+
		"\2fd\3\2\2\2gh\7\20\2\2hi\7\23\2\2i\t\3\2\2\2jk\7\35\2\2kl\7\17\2\2lm"+
		"\7\30\2\2mn\7\20\2\2no\5\f\7\2o\13\3\2\2\2pu\7\21\2\2qt\5\24\13\2rt\5"+
		"\4\3\2sq\3\2\2\2sr\3\2\2\2tw\3\2\2\2us\3\2\2\2uv\3\2\2\2vx\3\2\2\2wu\3"+
		"\2\2\2xy\7\22\2\2y\r\3\2\2\2z|\7\33\2\2{}\7\27\2\2|{\3\2\2\2|}\3\2\2\2"+
		"}~\3\2\2\2~\177\7(\2\2\177\u0080\5\20\t\2\u0080\17\3\2\2\2\u0081\u0086"+
		"\7\21\2\2\u0082\u0085\5\4\3\2\u0083\u0085\5\n\6\2\u0084\u0082\3\2\2\2"+
		"\u0084\u0083\3\2\2\2\u0085\u0088\3\2\2\2\u0086\u0084\3\2\2\2\u0086\u0087"+
		"\3\2\2\2\u0087\u0089\3\2\2\2\u0088\u0086\3\2\2\2\u0089\u008a\7\22\2\2"+
		"\u008a\21\3\2\2\2\u008b\u008c\5\"\22\2\u008c\u008d\7\t\2\2\u008d\u008e"+
		"\5\"\22\2\u008e\u00a4\3\2\2\2\u008f\u0090\5\"\22\2\u0090\u0091\7\n\2\2"+
		"\u0091\u0092\5\"\22\2\u0092\u00a4\3\2\2\2\u0093\u0094\5\"\22\2\u0094\u0095"+
		"\7\13\2\2\u0095\u0096\5\"\22\2\u0096\u00a4\3\2\2\2\u0097\u0098\5\"\22"+
		"\2\u0098\u0099\7\f\2\2\u0099\u009a\5\"\22\2\u009a\u00a4\3\2\2\2\u009b"+
		"\u009c\5\"\22\2\u009c\u009d\7\r\2\2\u009d\u009e\5\"\22\2\u009e\u00a4\3"+
		"\2\2\2\u009f\u00a0\5\"\22\2\u00a0\u00a1\7\16\2\2\u00a1\u00a2\5\"\22\2"+
		"\u00a2\u00a4\3\2\2\2\u00a3\u008b\3\2\2\2\u00a3\u008f\3\2\2\2\u00a3\u0093"+
		"\3\2\2\2\u00a3\u0097\3\2\2\2\u00a3\u009b\3\2\2\2\u00a3\u009f\3\2\2\2\u00a4"+
		"\23\3\2\2\2\u00a5\u00a6\7\34\2\2\u00a6\u00a7\7\17\2\2\u00a7\u00ac\5\22"+
		"\n\2\u00a8\u00a9\7\4\2\2\u00a9\u00ab\5\22\n\2\u00aa\u00a8\3\2\2\2\u00ab"+
		"\u00ae\3\2\2\2\u00ac\u00aa\3\2\2\2\u00ac\u00ad\3\2\2\2\u00ad\u00af\3\2"+
		"\2\2\u00ae\u00ac\3\2\2\2\u00af\u00b0\7\20\2\2\u00b0\u00b1\5\26\f\2\u00b1"+
		"\25\3\2\2\2\u00b2\u00b6\7\21\2\2\u00b3\u00b5\5\4\3\2\u00b4\u00b3\3\2\2"+
		"\2\u00b5\u00b8\3\2\2\2\u00b6\u00b4\3\2\2\2\u00b6\u00b7\3\2\2\2\u00b7\u00b9"+
		"\3\2\2\2\u00b8\u00b6\3\2\2\2\u00b9\u00ba\7\22\2\2\u00ba\27\3\2\2\2\u00bb"+
		"\u00bc\7\36\2\2\u00bc\u00bd\7(\2\2\u00bd\u00be\7\24\2\2\u00be\u00c3\5"+
		"\22\n\2\u00bf\u00c0\7\4\2\2\u00c0\u00c2\5\22\n\2\u00c1\u00bf\3\2\2\2\u00c2"+
		"\u00c5\3\2\2\2\u00c3\u00c1\3\2\2\2\u00c3\u00c4\3\2\2\2\u00c4\u00c6\3\2"+
		"\2\2\u00c5\u00c3\3\2\2\2\u00c6\u00c7\7\23\2\2\u00c7\31\3\2\2\2\u00c8\u00c9"+
		"\7\37\2\2\u00c9\u00ca\7(\2\2\u00ca\u00cb\5\34\17\2\u00cb\33\3\2\2\2\u00cc"+
		"\u00ce\7\21\2\2\u00cd\u00cf\5\4\3\2\u00ce\u00cd\3\2\2\2\u00cf\u00d0\3"+
		"\2\2\2\u00d0\u00ce\3\2\2\2\u00d0\u00d1\3\2\2\2\u00d1\u00d2\3\2\2\2\u00d2"+
		"\u00d3\7\22\2\2\u00d3\35\3\2\2\2\u00d4\u00d5\5\"\22\2\u00d5\u00d6\7\7"+
		"\2\2\u00d6\u00d7\5\"\22\2\u00d7\u00ed\3\2\2\2\u00d8\u00d9\5\"\22\2\u00d9"+
		"\u00da\7\b\2\2\u00da\u00db\5\"\22\2\u00db\u00ed\3\2\2\2\u00dc\u00dd\5"+
		"\"\22\2\u00dd\u00de\7\5\2\2\u00de\u00df\5\"\22\2\u00df\u00ed\3\2\2\2\u00e0"+
		"\u00e1\5\"\22\2\u00e1\u00e2\7\6\2\2\u00e2\u00e3\5\"\22\2\u00e3\u00ed\3"+
		"\2\2\2\u00e4\u00e5\t\2\2\2\u00e5\u00e6\7\17\2\2\u00e6\u00e7\5\"\22\2\u00e7"+
		"\u00e8\7\4\2\2\u00e8\u00e9\5\"\22\2\u00e9\u00ea\7\20\2\2\u00ea\u00ed\3"+
		"\2\2\2\u00eb\u00ed\5\"\22\2\u00ec\u00d4\3\2\2\2\u00ec\u00d8\3\2\2\2\u00ec"+
		"\u00dc\3\2\2\2\u00ec\u00e0\3\2\2\2\u00ec\u00e4\3\2\2\2\u00ec\u00eb\3\2"+
		"\2\2\u00ed\37\3\2\2\2\u00ee\u00ef\5*\26\2\u00ef\u00f0\7\24\2\2\u00f0\u00f1"+
		"\5\36\20\2\u00f1\u00f2\7\23\2\2\u00f2!\3\2\2\2\u00f3\u00f8\5*\26\2\u00f4"+
		"\u00f8\5,\27\2\u00f5\u00f8\5(\25\2\u00f6\u00f8\5.\30\2\u00f7\u00f3\3\2"+
		"\2\2\u00f7\u00f4\3\2\2\2\u00f7\u00f5\3\2\2\2\u00f7\u00f6\3\2\2\2\u00f8"+
		"#\3\2\2\2\u00f9\u00fa\t\3\2\2\u00fa%\3\2\2\2\u00fb\u00fc\t\4\2\2\u00fc"+
		"\'\3\2\2\2\u00fd\u00fe\5$\23\2\u00fe\u00ff\7\3\2\2\u00ff\u0100\5&\24\2"+
		"\u0100)\3\2\2\2\u0101\u0102\7(\2\2\u0102+\3\2\2\2\u0103\u0104\7)\2\2\u0104"+
		"-\3\2\2\2\u0105\u0106\7\30\2\2\u0106\u0107\7\3\2\2\u0107\u0108\7\31\2"+
		"\2\u0108/\3\2\2\2\27\638:@FKS]dsu|\u0084\u0086\u00a3\u00ac\u00b6\u00c3"+
		"\u00d0\u00ec\u00f7";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}