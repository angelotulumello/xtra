grammar xtra;

program: declaration*
         (custom_event_definition|macro_action_definition)*
         body_element*
         state_declaration*
         ;

body_element: (assignment // An element of a body of a block (condition, event and state blocks)
            |  function)
            //|  macro_action_call)
            ;

declaration: REGISTER_DECLARER identifier (COMMA identifier)* SEMICOLON; // Register(s) declaration

function: ID LPARENT term* (COMMA term)* RPARENT SEMICOLON; // a function i.e. action call

event_statement: EVENT_DECLARER LPARENT EVENT RPARENT event_body; // The on(event){event_body}
event_body: LBRACE 
            (conditions_statement|body_element)*
            RBRACE; // Inside an event block we can have conditions blocks and actions

state_declaration: STATE_DECLARER INITIAL_MODIFIER? ID state_body; //State [initial] state_name {state_body}
state_body: LBRACE (body_element/*|conditions_statement*/|event_statement)* RBRACE; // can be a /*condition block,*/ an event block or actions

condition: term EQ term
         | term LEQ term
         | term GEQ term
         | term NEQ term
         | term GRET term
         | term LESS term
         ;
/*
condition: (identifier|header_field) EQ term
         | (identifier|header_field) LEQ term
         | (identifier|header_field) GEQ term
         | (identifier|header_field) NEQ term
         | (identifier|header_field) GRET term
         | (identifier|header_field) LESS term
         ;
*/
conditions_statement: CONDITIONS_DECLARER LPARENT
                      condition (COMMA condition)*
                      RPARENT conditions_body; // The if (condition(s)) {condition_body}
conditions_body: LBRACE body_element* RBRACE; // can be only a group of actions

custom_event_definition: CUSTOM_EVENT_DECLARER ID ASSIGNMENT condition (COMMA condition)* SEMICOLON; // Event custom_event = reg0 != reg1;

macro_action_definition: MACRO_ACTION_DECLARER ID macro_action_body; // Action action_name {action_body}
macro_action_body: LBRACE body_element+ RBRACE; // action_body is a set of actions and updates
//macro_action_call: ID LPARENT RPARENT SEMICOLON;

//expr: LPARENT expr RPARENT
//    | expr MUL expr
//    | expr DIV expr
//    | expr PLUS expr
//    | expr MINUS expr
//    | term
//    ;
/*expr: term MUL term
    | term DIV term
    | term PLUS term
    | term MINUS term
    | term
    ;*/
expr: term MUL term
    | term DIV term
    | term PLUS term
    | term MINUS term
    | (MAX|MIN) LPARENT term COMMA term RPARENT
    | term
    ;
assignment: identifier ASSIGNMENT expr SEMICOLON;
term: identifier
    | integer
    | header_field
    | event_field
    ;
header_type: ETH | IPV4 | TCP | UDP;
header_field_name: ETH_FIELD | IPV4_FIELD | TCP_FIELD | UDP_FIELD;
header_field: header_type'.'header_field_name;
identifier: ID; // A basic identifier for functions, registers and state names
integer: NUMBER; // The basic integer type
event_field: EVENT'.'EVENT_FIELD;

COMMA: ',';
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
EQ: '==';
LEQ: '<=';
GEQ: '>=';
NEQ: '!=';
GRET: '>';
LESS: '<';
LPARENT: '(';
RPARENT: ')';
LBRACE: '{';
RBRACE: '}';
SEMICOLON: ';';
ASSIGNMENT: '=';
MAX: 'max';
MIN: 'min';

INITIAL_MODIFIER: 'initial'; // Used to mark a state as intial

EVENT: 'pktRcvd' | 'timeout';
EVENT_FIELD: 'data0' | 'data1' | 'port';

REGISTER_DECLARER: 'Register'; // Used to declare register(s)
STATE_DECLARER: 'State'; // Used to declare a state and begin state statement
CONDITIONS_DECLARER: 'if'; // Used to begin a condition(s) statement
EVENT_DECLARER: 'on'; // Used to begin a event statement
CUSTOM_EVENT_DECLARER: 'Event'; // Used to define a custom event
MACRO_ACTION_DECLARER: 'Action'; // Used to define a macro action

ETH: 'eth';
IPV4: 'ipv4';
TCP: 'tcp';
UDP: 'udp';

ETH_FIELD: 'src' | 'dst' | 'type';
IPV4_FIELD: 'src' | 'dst' | 'ttl' | 'proto' | 'checksum';
TCP_FIELD: 'sport' | 'dport' | 'seqNo' | 'ackNo' | 'window' | 'flags' | 'checksum'
           | 'timestamp' | 'timestampEchoReply' | 'mss' | 'windowScale';
UDP_FIELD: 'sport' | 'dport' | 'checksum' | 'length' | 'p1' | 'p2' | 'p3' | 'p4';

ID: ((LETTER) (LETTER | DIGIT | '_')*) | ('_' (LETTER | DIGIT | '_')+); // Can be componed of letters, digits
                                                                        // and underscores. Can begin only with
                                                                        // a letter or an underscore
NUMBER: DIGIT+;

WS: [ \t\r\n]+ -> skip; // Ignoring spaces, tabs, returns and newlines

COMMENT : '/*' (COMMENT|.)*? '*/' -> skip; // Ignoring C-style block comments
LINE_COMMENT  : '//' .*? '\n' -> skip; // Ignoring C-style line comments

fragment LETTER: ('a'..'z'|'A'..'Z');
fragment DIGIT: '0'..'9';
