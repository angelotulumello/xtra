package parser;

import compilerutils.*;
import exceptions.*;
import model.*;
import model.Boolean;
import model.opcodes.ConditionOpcode;
import model.opcodes.UpdateOpcode;
import model.readOnlyRegisters.ReadOnlyRegistersOpcode;

import java.util.List;
import java.util.Vector;

import static parser.Scope.*;

public class XtraCompiler extends xtraBaseListener {
    private Update update; // Update in parsing, consumed by exitAssignment

    private Vector<Action> conditionActions = new Vector<>(); // consumed by exitConditions_statement
    private Vector<Action> eventActions = new Vector<>();
    private Vector<Action> stateActions = new Vector<>();
    private Vector<Action> macroAction = new Vector<>();

    private Vector<TableRow> inParsingTableRows = new Vector<>(); // table row in parsing. Consumed by exitState_declaration

    // consumed by exitProgram
    private Vector<Condition> conditions = new Vector<>();
    private State initialState = null;
    private Vector<TableRow> tableRows = new Vector<>(); // the parsed table rows
    private Table table;

    private Vector<ConditionResult> conditionResults = new Vector<>(); // consumed by exitConditions_statement

    private Vector<ParserException> parserExceptions = new Vector<>(); // Exception raised by the AST visit

    // the state of the current AST visit:
    private Scope currentScope = outside; // i am in a condition-block, in a event-block, in a state-block or in an action definition block
    private Scope lastScope = outside; // the previuous scope
    private Event currentEvent; // the last event where the visit is entered
    private State currentState; // the last state where the visit is entered

    @Override
    public void enterMacro_action_definition(xtraParser.Macro_action_definitionContext ctx) {
        super.enterMacro_action_definition(ctx);
        lastScope = currentScope;
        currentScope = inMacroActionBlock;
        macroAction.removeAllElements();
    }

    @Override
    public void enterEvent_statement(xtraParser.Event_statementContext ctx) {
        super.enterEvent_statement(ctx);

        currentEvent = Event.toEvent(ctx.EVENT().getText());
        eventActions = new Vector<>();
        lastScope = currentScope;
        currentScope = inEventBlock;
    }

    @Override
    public void enterDeclaration(xtraParser.DeclarationContext ctx) {
        super.enterDeclaration(ctx);
        List<xtraParser.IdentifierContext> identifiers = ctx.identifier();
        for (xtraParser.IdentifierContext context : identifiers) {
            String registerName = context.getText();
            if (WritableRegisterFactory.getInstance().isPresent(registerName))
                parserExceptions.add(new RegisterException("Multiple declaration of the register " + registerName));
            WritableRegisterFactory.getInstance().allocateRegister(context.getText());
        }
    }

    @Override
    public void exitExpr(xtraParser.ExprContext ctx) {
        super.exitExpr(ctx);
        xtraParser.TermContext op0Ctx = ctx.term(0);
        xtraParser.TermContext op1Ctx = ctx.term(1);
        Operand op0, op1;
        UpdateOpcode opcode = null;

        if (op1Ctx != null) {
            if (ctx.PLUS() != null)
                opcode = UpdateOpcode.XTRA_SUM;
            else if (ctx.MINUS() != null)
                opcode = UpdateOpcode.XTRA_MINUS;
            else if (ctx.MUL() != null)
                opcode = UpdateOpcode.XTRA_MULTIPLY;
            else if (ctx.DIV() != null)
                opcode = UpdateOpcode.XTRA_DIVIDE;
            else if (ctx.MAX() != null)
                opcode = UpdateOpcode.XTRA_MAX;
            else if (ctx.MIN() != null)
                opcode = UpdateOpcode.XTRA_MIN;

            try {
                op0 = getOperator(op0Ctx);
                op1 = getOperator(op1Ctx);
                update = new Update(opcode, op0, op1, null); // dest register will be written by the declaration
            } catch (UndeclaredRegisterException e) {
                parserExceptions.add(e);
            }
        }
        else
        {
            try {
                op0 = getOperator(op0Ctx);
                op1 = new Constant(0);
                update = new Update(UpdateOpcode.XTRA_SUM, op0, op1, null); // dest register will be written by the declaration
            } catch (UndeclaredRegisterException e) {
                parserExceptions.add(e);
            }
        }
    }

    @Override
    public void exitAssignment(xtraParser.AssignmentContext ctx) {
        super.exitAssignment(ctx);
        String registerName = ctx.identifier().getText();
        Register register = WritableRegisterFactory.getInstance().getRegister(registerName);
        if (register == null)
            parserExceptions.add(new UndeclaredRegisterException("The register '" + registerName + "' was undeclared."));
        update.setOp2(register);

        Vector<Action> updateVector = new Vector<>();
        updateVector.add(update);
        addActionsInScope(updateVector);
    }

    @Override
    public void enterState_declaration(xtraParser.State_declarationContext ctx) {
        super.enterState_declaration(ctx);

        inParsingTableRows = new Vector<>();
        lastScope = currentScope;
        currentScope = inStateBlock;
        EventOracle.getInstance().resetMap();
        stateActions = new Vector<>();

        String stateName = ctx.ID().getSymbol().getText();
        currentState = StateFactory.getInstance().declareState(stateName);
    }

    @Override
    public void enterConditions_body(xtraParser.Conditions_bodyContext ctx) {
        super.enterConditions_body(ctx);
        conditionActions = new Vector<>();
        lastScope = currentScope;
        currentScope = inConditionBlock;
    }

    @Override
    public void enterConditions_statement(xtraParser.Conditions_statementContext ctx) {
        super.enterConditions_statement(ctx);
        conditionResults = new Vector<>();
        lastScope = currentScope;
        currentScope = inConditionBlock;
    }

    @Override
    public void exitCondition(xtraParser.ConditionContext ctx) /*throws UndeclaredRegisterException, UnknownOperandException*/ {
        super.exitCondition(ctx);
        xtraParser.TermContext op0Ctx = ctx.term(0);
        xtraParser.TermContext op1Ctx = ctx.term(1);
        ConditionOpcode opcode = null;
        Operand op0, op1;

        if (ctx.EQ() != null)
            opcode = ConditionOpcode.XTRA_EQUAL;
        else if (ctx.NEQ() != null)
            opcode = ConditionOpcode.XTRA_NOT_EQUAL;
        else if (ctx.LEQ() != null)
            opcode = ConditionOpcode.XTRA_LESS_EQ;
        else if (ctx.GEQ() != null)
            opcode = ConditionOpcode.XTRA_GREATER_EQ;
        else if (ctx.LESS() != null)
            opcode = ConditionOpcode.XTRA_LESS;
        else if (ctx.GRET() != null)
            opcode = ConditionOpcode.XTRA_GREATER;

        try {
            op0 = getOperator(op0Ctx);
            op1 = getOperator(op1Ctx);
            Condition condition;

            if (!ConditionFactory.getInstance().isPresent(op0, op1, opcode)) {
                condition = ConditionFactory.getInstance().createCondition(op0, op1, opcode);
                conditions.add(condition);
            } else {
                condition = ConditionFactory.getInstance().getCondition(op0, op1, opcode);
            }

            ConditionResult result = new ConditionResult(condition, Boolean.XTRA_TRUE);
            conditionResults.add(result);
        } catch (UndeclaredRegisterException e) {
            parserExceptions.add(e);
        }
    }

    @Override
    public void exitConditions_statement(xtraParser.Conditions_statementContext ctx) {
        super.exitConditions_statement(ctx);

        /*if (currentEvent != null) {*/
            // simple case (if in an event block)

        TableRow tableRow = new TableRow(conditionResults, conditionActions, currentState, currentEvent);
        inParsingTableRows.add(tableRow);

        /*}
        else
        {
            for (Event event: Event.getEvents())
            {
                TableRow tableRow = new TableRow(conditionResults, conditionActions, currentState, event);
                inParsingTableRows.add(tableRow);
            }
        }*/
        currentScope = lastScope;
        conditionActions = new Vector<>();
    }

    @Override
    public void exitState_declaration(xtraParser.State_declarationContext ctx) {
        super.exitState_declaration(ctx);

        for (Event event: EventOracle.getInstance().getRemaining())
            inParsingTableRows.add(new TableRow(new Vector<ConditionResult>(), new Vector<Action>(), currentState, event));

        for (TableRow tableRow : inParsingTableRows) {
            //if (tableRow.getState().equals(currentState)) {
                // adding state-scope actions
                Vector<Action> actions = new Vector<>();
                actions.addAll(stateActions);
                actions.addAll(tableRow.getActions());
                tableRow.setActions(actions);
                //}
        }

        if (ctx.INITIAL_MODIFIER() != null) {
            if (initialState == null)
                initialState = currentState;
            else
                parserExceptions.add(new InitialStateException("Multiple definition of the initial state"));
        }

        currentScope = lastScope;
        for(TableRow tableRow: inParsingTableRows){
            if(!tableRow.getActions().isEmpty())
                tableRows.add(tableRow);
        }
        //tableRows.addAll(inParsingTableRows);
        currentState = null;
    }

    @Override
    public void exitEvent_statement(xtraParser.Event_statementContext ctx) {
        super.exitEvent_statement(ctx);
        String eventName = ctx.EVENT().getSymbol().getText();
        Event event = Event.toEvent(eventName);

        if (!eventActions.isEmpty() && EventOracle.getInstance().isPresent(event))
            inParsingTableRows.add(new TableRow(new Vector<ConditionResult>(), new Vector<Action>(), currentState, event));

        for (TableRow tableRow : inParsingTableRows) {
            if (tableRow.getEvent().equals(event)) {
                // adding event-scope actions
                Vector<Action> actions = new Vector<>();
                actions.addAll(eventActions);
                actions.addAll(tableRow.getActions());
                tableRow.setActions(actions);
                }
        }

        currentScope = lastScope;
        EventOracle.getInstance().remove(event);
        currentEvent = null;
    }

    private Operand getOperator(xtraParser.TermContext termContext) throws UndeclaredRegisterException {
        if (termContext.identifier() != null) {
            // try to parse as a register
            return getOperator(termContext.identifier());
        } else if (termContext.header_field() != null) {
            // try to parse it as an header field
            return getOperator(termContext.header_field());
        } else if (termContext.event_field() != null) {
            // try to parse it as an event field
            return getOperator(termContext.event_field());
        }
        else {
            // try to parse it as a constant
            String integerString = termContext.integer().getText();
            int value = Integer.valueOf(integerString);
            return new Constant(value);
        }
    }

    private Operand getOperator(xtraParser.IdentifierContext identifierContext) throws UndeclaredRegisterException {
        String registerName = identifierContext.getText();
        Register register;

        try {
            register = ReadOnlyRegisterFactory.getInstance().getReadOnlyRegister(registerName);
            return register;
        }
        catch (UndeclaredRegisterException e) {
            register = WritableRegisterFactory.getInstance().getRegister(registerName);
            if (register == null) {
                throw new UndeclaredRegisterException("The register '" + registerName + "' was undeclared.");
            }
            return register;
        }
    }

    private Operand getOperator(xtraParser.Header_fieldContext headerFieldContext) {
        return HeaderFieldFactory.getInstance().getHeaderField(headerFieldContext.header_type().getText(),
                headerFieldContext.header_field_name().getText());
    }

    private Operand getOperator(xtraParser.Event_fieldContext eventFieldContext) {
        return EventFieldFactory.getInstance().getEventField(eventFieldContext.EVENT().getText(),
                eventFieldContext.EVENT_FIELD().getText());
    }

    private Vector<Operand> getOperators(List<xtraParser.TermContext> terms) throws UndeclaredRegisterException, OutOfScopeException, RegisterException {
        Vector<Operand> ops = new Vector<>();

        for(int i=0; i < terms.size(); i++)
        {
            ops.add(i, getOperator(terms.get(i)));

            /*if (terms.get(i).identifier() != null)
            {
                String registerName = terms.get(i).identifier().getText();
                ops.add(i, WritableRegisterFactory.getInstance().getRegister(registerName));
                if (ops.get(i) == null)
                    throw new UndeclaredRegisterException("The register '"+registerName+"' was undeclared.");
            }
            else
            {
                String integerString = terms.get(i).integer().getText();
                int value = Integer.valueOf(integerString);

                ops.add(i, new Constant(value));
            }*/
        }
        return ops;
    }

    @Override
    public void exitFunction(xtraParser.FunctionContext ctx) {
        super.exitFunction(ctx);
        String functionName = ctx.ID().getSymbol().getText();
        List<xtraParser.TermContext> terms = ctx.term();

        if (ActionsFactory.getInstance().isAStateTransition(functionName)) {
            try {
                Vector<Action> actions = new Vector<>();
                actions.add(ActionsFactory.getInstance().getStateTransition(terms));
                addActionsInScope(actions);
            } catch (TooManyArgsException|UndeclaredStateException e) {
                parserExceptions.add(e);
            }
            return;
        }

        Vector<Operand> args;
        try {
            args = getOperators(terms);
            addActionsInScope(getActions(functionName, args));
        } catch (UndeclaredRegisterException |
                TooManyArgsException | UnknownOpcodeException | OutOfScopeException | RegisterException e) {
            parserExceptions.add(e);
        }
    }

    private void addActionsInScope(Vector<Action> parsedActions) {
        switch (currentScope) {
            case inConditionBlock:
                conditionActions.addAll(parsedActions);
                break;
            case inEventBlock:
                eventActions.addAll(parsedActions);
                break;
            case inStateBlock:
                stateActions.addAll(parsedActions);
                break;
            case inMacroActionBlock:
                macroAction.addAll(parsedActions);
                break;
        }
    }

    @Override
    public void exitMacro_action_definition(xtraParser.Macro_action_definitionContext ctx) {
        super.exitMacro_action_definition(ctx);

        String name = ctx.ID().getText();

        try {
            MacroActionsFactory.getInstance().createMacroAction(name, macroAction);
        } catch (AlreadyDefinedException e) {
            parserExceptions.add(e);
        }

        currentScope = lastScope;
    }

    private Vector<Action> getActions(String functionName, Vector<Operand> args) throws TooManyArgsException, UnknownOpcodeException {
        Vector<Action> actions = new Vector<>();

        try {
            Vector<Action> acts = MacroActionsFactory.getInstance().getMacroAction(functionName);
            return acts;
        } catch (UndefinedMacroActionException e) {
            actions.add(ActionsFactory.getInstance().getAction(functionName, args));
            return actions;
        }
    }

    @Override
    public void exitProgram(xtraParser.ProgramContext ctx) {
        super.exitProgram(ctx);
        if (initialState == null)
            parserExceptions.add(new InitialStateException("Initial state undefined"));
        table = new Table(initialState, tableRows, conditions);
        try {
            StateFactory.getInstance().checkPendings();
        } catch (UndeclaredStateException e) {
            parserExceptions.add(e);
        }
    }

    public Table getTable() throws ParserException {
        if (!parserExceptions.isEmpty()) {
            String exceptionMessage = new String();
            for (ParserException exception : parserExceptions) {
                exceptionMessage += exception.getMessage() + "\n";
            }
            throw new ParserException(exceptionMessage);
        }
        return table;
    }
}
