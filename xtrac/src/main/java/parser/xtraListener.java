// Generated from /home/giacomo/IdeaProjects/XTRAC/src/main/java/xtra.g4 by ANTLR 4.7
package parser;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link xtraParser}.
 */
public interface xtraListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link xtraParser#program}.
	 * @param ctx the parse tree
	 */
	void enterProgram(xtraParser.ProgramContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#program}.
	 * @param ctx the parse tree
	 */
	void exitProgram(xtraParser.ProgramContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#body_element}.
	 * @param ctx the parse tree
	 */
	void enterBody_element(xtraParser.Body_elementContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#body_element}.
	 * @param ctx the parse tree
	 */
	void exitBody_element(xtraParser.Body_elementContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#declaration}.
	 * @param ctx the parse tree
	 */
	void enterDeclaration(xtraParser.DeclarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#declaration}.
	 * @param ctx the parse tree
	 */
	void exitDeclaration(xtraParser.DeclarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#function}.
	 * @param ctx the parse tree
	 */
	void enterFunction(xtraParser.FunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#function}.
	 * @param ctx the parse tree
	 */
	void exitFunction(xtraParser.FunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#event_statement}.
	 * @param ctx the parse tree
	 */
	void enterEvent_statement(xtraParser.Event_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#event_statement}.
	 * @param ctx the parse tree
	 */
	void exitEvent_statement(xtraParser.Event_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#event_body}.
	 * @param ctx the parse tree
	 */
	void enterEvent_body(xtraParser.Event_bodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#event_body}.
	 * @param ctx the parse tree
	 */
	void exitEvent_body(xtraParser.Event_bodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#state_declaration}.
	 * @param ctx the parse tree
	 */
	void enterState_declaration(xtraParser.State_declarationContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#state_declaration}.
	 * @param ctx the parse tree
	 */
	void exitState_declaration(xtraParser.State_declarationContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#state_body}.
	 * @param ctx the parse tree
	 */
	void enterState_body(xtraParser.State_bodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#state_body}.
	 * @param ctx the parse tree
	 */
	void exitState_body(xtraParser.State_bodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#condition}.
	 * @param ctx the parse tree
	 */
	void enterCondition(xtraParser.ConditionContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#condition}.
	 * @param ctx the parse tree
	 */
	void exitCondition(xtraParser.ConditionContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#conditions_statement}.
	 * @param ctx the parse tree
	 */
	void enterConditions_statement(xtraParser.Conditions_statementContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#conditions_statement}.
	 * @param ctx the parse tree
	 */
	void exitConditions_statement(xtraParser.Conditions_statementContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#conditions_body}.
	 * @param ctx the parse tree
	 */
	void enterConditions_body(xtraParser.Conditions_bodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#conditions_body}.
	 * @param ctx the parse tree
	 */
	void exitConditions_body(xtraParser.Conditions_bodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#custom_event_definition}.
	 * @param ctx the parse tree
	 */
	void enterCustom_event_definition(xtraParser.Custom_event_definitionContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#custom_event_definition}.
	 * @param ctx the parse tree
	 */
	void exitCustom_event_definition(xtraParser.Custom_event_definitionContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#macro_action_definition}.
	 * @param ctx the parse tree
	 */
	void enterMacro_action_definition(xtraParser.Macro_action_definitionContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#macro_action_definition}.
	 * @param ctx the parse tree
	 */
	void exitMacro_action_definition(xtraParser.Macro_action_definitionContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#macro_action_body}.
	 * @param ctx the parse tree
	 */
	void enterMacro_action_body(xtraParser.Macro_action_bodyContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#macro_action_body}.
	 * @param ctx the parse tree
	 */
	void exitMacro_action_body(xtraParser.Macro_action_bodyContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(xtraParser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(xtraParser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#assignment}.
	 * @param ctx the parse tree
	 */
	void enterAssignment(xtraParser.AssignmentContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#assignment}.
	 * @param ctx the parse tree
	 */
	void exitAssignment(xtraParser.AssignmentContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#term}.
	 * @param ctx the parse tree
	 */
	void enterTerm(xtraParser.TermContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#term}.
	 * @param ctx the parse tree
	 */
	void exitTerm(xtraParser.TermContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#header_type}.
	 * @param ctx the parse tree
	 */
	void enterHeader_type(xtraParser.Header_typeContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#header_type}.
	 * @param ctx the parse tree
	 */
	void exitHeader_type(xtraParser.Header_typeContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#header_field_name}.
	 * @param ctx the parse tree
	 */
	void enterHeader_field_name(xtraParser.Header_field_nameContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#header_field_name}.
	 * @param ctx the parse tree
	 */
	void exitHeader_field_name(xtraParser.Header_field_nameContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#header_field}.
	 * @param ctx the parse tree
	 */
	void enterHeader_field(xtraParser.Header_fieldContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#header_field}.
	 * @param ctx the parse tree
	 */
	void exitHeader_field(xtraParser.Header_fieldContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#identifier}.
	 * @param ctx the parse tree
	 */
	void enterIdentifier(xtraParser.IdentifierContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#identifier}.
	 * @param ctx the parse tree
	 */
	void exitIdentifier(xtraParser.IdentifierContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#integer}.
	 * @param ctx the parse tree
	 */
	void enterInteger(xtraParser.IntegerContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#integer}.
	 * @param ctx the parse tree
	 */
	void exitInteger(xtraParser.IntegerContext ctx);
	/**
	 * Enter a parse tree produced by {@link xtraParser#event_field}.
	 * @param ctx the parse tree
	 */
	void enterEvent_field(xtraParser.Event_fieldContext ctx);
	/**
	 * Exit a parse tree produced by {@link xtraParser#event_field}.
	 * @param ctx the parse tree
	 */
	void exitEvent_field(xtraParser.Event_fieldContext ctx);
}