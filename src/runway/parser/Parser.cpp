//
// Parser.cpp
// Parser for the runway language
//
// Created by Joshua Brunhuber on 30.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//
#include "parser/Parser.hpp"

/**
 *
 */
Parser::Parser(Tokenizer *tokenizer) {

  _tokenizer = tokenizer;
  //initialize
  nextToken();  //lookahead
  nextToken();  //and current token
}

/**
 * Statements
 *
 * Supported: if, for, while, function-calls, return
 */
bool Parser::parseStatement(Statement **stmt) {

  //end
  if (_current_token.token_type == TokenType::T_EOF) {
    return false;
  }

  if (IS_PUNCTUATOR("{")) {
    nextToken();  //step '{'
    BodyStatement *body_stmt = 0;
    parseBodyStatement(&body_stmt);
    *stmt = body_stmt;
    return true;

  } else if (IS_KEYWORD("if")) {
    IfStatement *if_statement = 0;
    parseIfStatement(&if_statement);
    *stmt = if_statement;
    return true;

  } else if (IS_KEYWORD("while")) {
    WhileLoopStatement *while_loop_statement = 0;
    parseWhileStatement(&while_loop_statement);
    *stmt = while_loop_statement;
    return true;

  } else if (IS_KEYWORD("for")) {
    ForStatement *for_statement = 0;
    parseForStatement(&for_statement);
    *stmt = for_statement;
    return true;

  } else if (IS_KEYWORD("else")) {
    nextToken(); //step 'else' and parse if
    Statement *statement = nullptr;
    parseStatement(&statement);
    *stmt = statement;
    return true;
  }

  if (parseExpressionStatement(stmt) && _current_token.textual_content == ";") {
    nextToken();
    return true;
  }

  //Syntax Error
  ERROR_PRINT("Syntax Error: Expected ';' but " << _current_token.textual_content << " found");
  return false;
}

/**
 * Parses Expression-Statements
 */
bool Parser::parseExpressionStatement(Statement **stmt) {

  if (isType(_current_token)) {
    VariableDeclarationStatement *variable_decl_stmt = 0;
    parseVariableDeclarationStatement(&variable_decl_stmt);
    *stmt = variable_decl_stmt;
    return true;

  } else if (IS_KEYWORD("return")) {
    nextToken();  //step 'return'
    ReturnStatement *return_statement = new ReturnStatement;
    Expression *return_value = 0;
    parseExpression(&return_value);
    return_statement->expression_to_return = return_value;
    *stmt = return_statement;
    return true;

  } else if (IS_KEYWORD("continue")) {
    nextToken();  //step 'continue'
    JumpStatement *continue_jump_statement = new JumpStatement;
    continue_jump_statement->jump_statement_type = JumpStatementType::CONTINUE;
    *stmt = continue_jump_statement;
    return true;

  } else if (IS_KEYWORD("break")) {
    nextToken();  //step 'break'
    JumpStatement *break_jump_statement = new JumpStatement;
    break_jump_statement->jump_statement_type = JumpStatementType::BREAK;
    *stmt = break_jump_statement;
    return true;

  } else {
    //Create an expression statement and assign an expression
    ExpressionStatement *expr_stmt = new ExpressionStatement;
    Expression *expr = nullptr;

    parseExpression(&expr);

    expr_stmt->expression = expr;
    *stmt = expr_stmt;
    return true;
  }
}

/**
 * IfStatement
 */
bool Parser::parseIfStatement(IfStatement **if_statement) {

  *if_statement = new IfStatement;

  nextToken();  //eat if keyword

  if (IS_PUNCTUATOR("(")) {
    nextToken();  //eat '('
  } else {
    return false;
  }

  //parse the condition expression
  Expression *condition_expr = nullptr;
  parseExpression(&condition_expr);
  (*if_statement)->condition = condition_expr;

  nextToken();  //eat ')'

  Statement *statement = nullptr;
  if (IS_PUNCTUATOR("{")) {
    BodyStatement *body_statement = new BodyStatement;
    parseBodyStatement(&body_statement);
    statement = body_statement;
  } else {
    parseStatement(&statement);
  }
  (*if_statement)->statement = statement;

  if (IS_KEYWORD("else")) {

    Statement *else_statement = nullptr;
    parseStatement(&else_statement);
    (*if_statement)->else_stmt = else_statement;
  }
  return true;
}

/**
 * WhileStatement
 */
bool Parser::parseWhileStatement(WhileLoopStatement **while_loop_statement) {

  *while_loop_statement = new WhileLoopStatement;

  nextToken();  //eat while keyword
  nextToken();  //eat ( punctuator
  Expression *condition = 0;

  if (parseExpression(&condition)) {
    (*while_loop_statement)->termination = condition;
  }

  nextToken();  //eat ) punctuator

  Statement *statement = 0;
  parseStatement(&statement);
  (*while_loop_statement)->statement = statement;
  return true;
}

/**
 * ForStatement
 */
bool Parser::parseForStatement(ForStatement **for_statement) {

  nextToken();  //step for
  nextToken();  //step '('
  *for_statement = new ForStatement;

  VariableDeclarationStatement *initialisation = 0;
  parseVariableDeclarationStatement(&initialisation);

  if (IS_PUNCTUATOR(";")) {  //standard for stmt

    (*for_statement)->forType = ForType::STANDARD;
    nextToken();  //step ';'

    Expression *termination = 0;
    parseExpression(&termination);

    nextToken();  //step ';'

    Expression *increment = 0;
    parseExpression(&increment);

    (*for_statement)->termination_expr = termination;
    (*for_statement)->increment_expr = increment;
  } else if (IS_PUNCTUATOR(":")) {  //Runway for stmt
    nextToken();  //step ':'
    (*for_statement)->forType = ForType::RUNWAY;
    Expression *range_start_expr = 0;
    parseExpression(&range_start_expr);
    if (IS_PUNCTUATOR("..")) {
      nextToken();  //step range
      Expression *range_end_expr = 0;
      parseExpression(&range_end_expr);
    } else {
      ERROR_PRINT("Syntax Error: Invalid range token" << _current_token.textual_content
                      << "in for loop. Use '..' to mark a range like '0 .. 1337'");
      return false;
    }
  } else {
    ERROR_PRINT("Syntax Error: Invalid Token " << _current_token.textual_content << "  in for loop");
    return false;
  }

  nextToken();  //step ')'

  (*for_statement)->initialization_stmt = initialisation;

  Statement *stmt = 0;
  parseStatement(&stmt);
  (*for_statement)->statement = stmt;
  return true;
}

/**
 * BodyStatement
 */
bool Parser::parseBodyStatement(BodyStatement **body_statement) {

  nextToken(); //step '{'

  //create body statement instance
  *body_statement = new BodyStatement;

  //create a list of statements
  std::vector<Statement *> statements;
  Statement *statement = 0;

  Statement *stmt = 0;
  while (!(IS_PUNCTUATOR("}"))) {
    parseStatement(&stmt);
    statements.push_back(statement);
  }
  nextToken();  //step '}'

  //set the statement list
  (*body_statement)->statements = statements;
  return true;
}

/**
 * VariableDeclarationStatement
 */
bool Parser::parseVariableDeclarationStatement(VariableDeclarationStatement **variable_declaration_statement) {

  //create variable declaration statement
  *variable_declaration_statement = new VariableDeclarationStatement;

  //set the type in the variable declaration statement
  PrimaryExpression *type_expr = new PrimaryExpression;
  type_expr->string_value = _current_token.textual_content;

  std::string type_identifier = _current_token.textual_content;

  if (!type_identifier.compare("int")) {
    type_expr->type = ExpressionType::INTEGER;
  } else if (!type_identifier.compare("float")) {
    type_expr->type = ExpressionType::FLOAT;
  } else if (!type_identifier.compare("double")) {
    type_expr->type = ExpressionType::DOUBLE;
  } else if (!type_identifier.compare("string")) {
    type_expr->type = ExpressionType::STRING;
  } else if (!type_identifier.compare("bool")) {
    type_expr->type = ExpressionType::BOOL;
  }

  (*variable_declaration_statement)->type = type_expr;
  nextToken();  //step type

  //if the current token is a identifier then parse the assignment expression
  if (_current_token.token_type == TokenType::IDENTIFIER) {

    //identifier
    std::string value_identifier = _current_token.textual_content;
    IdentifierPrimaryExpression *identifier_expr = new IdentifierPrimaryExpression;
    identifier_expr->type = ExpressionType::IDENTIFIER;
    identifier_expr->string_value = value_identifier;

    //when it's just a declaration don't parse assignment expression
    if (_lookahead_token.textual_content.compare(";")) {  //it's NOT a semicolon (assignment)
      Expression *assignment_expr = nullptr;
      parseAssignmentExpression(&assignment_expr);
      ((AssignmentExpression *) assignment_expr)->type = type_expr->type;
      (*variable_declaration_statement)->expression_to_assign = (AssignmentExpression *) assignment_expr;
    } else {
      nextToken();  //step identifier
    }
    (*variable_declaration_statement)->identifier = identifier_expr;
    return true;
  } else {
    return false;
  }
}

/**
 * Expression
 *
 * delegates to the top-node-expression
 */
bool Parser::parseExpression(Expression **expr) {

  parseAssignmentExpression(expr);
  return true;
}

/**
 * AssignmentExpression
 */
bool Parser::parseAssignmentExpression(Expression **expr) {

  Expression *lhs_identifier_expr = new AssignmentExpression;

  //identifier
  parseLogicalOrExpression(&lhs_identifier_expr);

  Operator assignment_operator = Operator::NONE;
  if (IS_PUNCTUATOR("=")) {
    assignment_operator = Operator::ASSIGNMENT;
  } else if (IS_PUNCTUATOR("+=")) {
    assignment_operator = Operator::ASSIGNMENT_ADD;
  } else if (IS_PUNCTUATOR("-=")) {
    assignment_operator = Operator::ASSIGNMENT_SUB;
  }

  //when there's no operator there's no assignment expression
  if (assignment_operator != Operator::NONE) {
    AssignmentExpression *assignment_expr = new AssignmentExpression;
    assignment_expr->assignment_operator = assignment_operator;

    IdentifierPrimaryExpression *identifier_expr = (IdentifierPrimaryExpression *) lhs_identifier_expr;
    identifier_expr->type = ExpressionType::NULL_PTR;
    assignment_expr->identifier = identifier_expr;
    nextToken();
    Expression *expression_to_assign = 0;
    parseExpression(&expression_to_assign);
    assignment_expr->expression_to_assign = expression_to_assign;

    *expr = assignment_expr;
  } else {
    *expr = lhs_identifier_expr;
  }
  return true;
}

/**
 * LogicalOR
 */
bool Parser::parseLogicalOrExpression(Expression **expr) {

  Expression *lhs_expr = 0;
  parseLogicalAndExpression(&lhs_expr);

  //if there's a logical OR operator
  if (IS_PUNCTUATOR("||")) {

    LogicalOrExpression *logical_or_expr = new LogicalOrExpression;
    logical_or_expr->lhs_logical_and_expr = (LogicalAndExpression *) lhs_expr;

    nextToken();  //eat '||' operator

    LogicalOrExpression *rhs_logical_or_expression = 0;
    parseLogicalOrExpression((Expression **) &rhs_logical_or_expression);
    logical_or_expr->rhs_logical_or_expr = rhs_logical_or_expression;
    *expr = logical_or_expr;  //logical or expression
  } else {
    *expr = lhs_expr;
  }
  return true;
}

/**
 * LogicalAND
 */
bool Parser::parseLogicalAndExpression(Expression **expr) {

  Expression *lhs_equality_expression = 0;
  parseEqualityExpression(&lhs_equality_expression);

  //if there's a logic AND operator
  if (IS_PUNCTUATOR("&&")) {

    LogicalAndExpression *logical_and_expr = new LogicalAndExpression;
    logical_and_expr->lhs_equality_expr = (EqualityExpression *) lhs_equality_expression;

    nextToken();  //eat '&&'

    LogicalOrExpression *rhs_logical_or_expr = 0;
    parseLogicalOrExpression((Expression **) &rhs_logical_or_expr);
    logical_and_expr->rhs_logical_or_expr = rhs_logical_or_expr;

    *expr = logical_and_expr;
  } else {
    *expr = lhs_equality_expression;
  }
  return true;
}

/**
 * EqualityExpression
 */
bool Parser::parseEqualityExpression(Expression **expr) {

  parseRelationalExpression(expr);

  EqualityExpression *equality_expr = new EqualityExpression;
  bool has_rhs = false;
  if (IS_PUNCTUATOR("==")) {
    equality_expr->compare_operator = Operator::COMPARE_EQUAL;
    has_rhs = true;
  } else if (IS_PUNCTUATOR("!=")) {
    equality_expr->compare_operator = Operator::COMPARE_UNEQUAL;
    has_rhs = true;
  }
  if (has_rhs) {
    //lhs
    equality_expr->lhs_relational_expr = *expr;

    nextToken();  //eat compare operator

    //rhs
    Expression *rhs_equality_expr = nullptr;
    parseEqualityExpression(&rhs_equality_expr);
    equality_expr->rhs_equality_expr = rhs_equality_expr;

    *expr = equality_expr;
  }
  return true;
}

/**
 * UnaryExpression
 */
bool Parser::parseUnaryExpression(Expression **expr) {

  UnaryExpression *unary_expression = new UnaryExpression;

  //prefix
  bool prefix = false;
  if (IS_PUNCTUATOR("++")) {
    unary_expression->unary_operator = Operator::INCREMENT;
    prefix = true;
  } else if (IS_PUNCTUATOR("--")) {
    unary_expression->unary_operator = Operator::DECREMENT;
    prefix = true;
  } else if (IS_PUNCTUATOR("!")) {
    unary_expression->unary_operator = Operator::NEGATE;
    prefix = true;
  } else if (IS_PUNCTUATOR("-")) {
    unary_expression->unary_operator = Operator::NEGATIVE;
    prefix = true;
  }

  if (prefix) {
    nextToken();  //if we have a prefix, jump over it
  }

  PostFixExpression *post_fix_expr = 0;
  parsePostFixExpression((Expression **) &post_fix_expr);

  if (prefix) {
    unary_expression->postfix_expr = post_fix_expr;
    *expr = unary_expression;
  } else {
    *expr = post_fix_expr;
  }
  return true;
}

/**
 * RelationalExpression
 */
bool Parser::parseRelationalExpression(Expression **expr) {

  Expression *lhs_additive_expr = 0;
  parseAdditiveExpression(&lhs_additive_expr);

  bool has_rhs = false;
  RelationalExpression *relational_expr = new RelationalExpression;
  if (IS_PUNCTUATOR("<")) {
    relational_expr->relational_operator = Operator::COMPARE_LESS;
    has_rhs = true;
  } else if (IS_PUNCTUATOR(">")) {
    relational_expr->relational_operator = Operator::COMPARE_GREATER;
    has_rhs = true;
  } else if (IS_PUNCTUATOR("<=")) {
    relational_expr->relational_operator = Operator::COMPARE_LESS_OR_EQUAL;
    has_rhs = true;
  } else if (IS_PUNCTUATOR(">=")) {
    relational_expr->relational_operator = Operator::COMPARE_GREATER_OR_EQUAL;
    has_rhs = true;
  }

  if (has_rhs) {
    relational_expr->lhs_additive_expression = lhs_additive_expr;
    nextToken();  //eat operator
    Expression *rhs_relational_expr = 0;
    parseRelationalExpression(&rhs_relational_expr);
    relational_expr->rhs_relational_expr = rhs_relational_expr;
    *expr = relational_expr;
  } else {
    *expr = lhs_additive_expr;
  }
  return true;
}

/**
 * PostFix Expressions
 *
 * see https://bitbucket.org/jbrunhuber/runway-compiler/wiki/Postfix%20Expression%20Parsing
 */
bool Parser::parsePostFixExpression(Expression **expr) {

  Expression *last_tmp_expr = 0;  //expression of the last iteration
  PostFixExpression *current_postfix_expr = 0;  //current postfix-expression in the iteration

  // parse the primary
  PrimaryExpression *primary_expr = 0;
  parsePrimaryExpression((Expression **) &primary_expr);
  last_tmp_expr = primary_expr;

  bool exit = false;

  while (!exit) {
    current_postfix_expr = 0;
    if (IS_PUNCTUATOR(".")) {
      //field access
      nextToken();  //step '.'
      FieldAccessPostFixExpression *field_access_expr = new FieldAccessPostFixExpression;
      field_access_expr->identifier = _current_token.textual_content;
      nextToken();  //step identifier
      current_postfix_expr = field_access_expr;
    } else if (IS_PUNCTUATOR("[")) {
      //array
      ArrayPostFixExpression *array_postfix_expr = new ArrayPostFixExpression;
      nextToken();  //step '['
      Expression *array_index_expr = 0;
      parseExpression(&array_index_expr);
      array_postfix_expr->index = array_index_expr;
      nextToken();  //step ']'
      current_postfix_expr = array_postfix_expr;
    } else if (IS_PUNCTUATOR("(")) {
      FunctionCallPostfixExpression *func_call_expr = new FunctionCallPostfixExpression;
      nextToken();  //step '('
      if (!IS_PUNCTUATOR(")")) {  //when parameter are available parse them
        std::vector<Expression *> params;
        Expression *expression = 0;
        parseExpression(&expression);
        params.push_back(expression);
        while (IS_PUNCTUATOR(",")) {
          parseExpression(&expression);
          if (expression != 0) {
            params.push_back(expression);
          }
        }
        func_call_expr->arguments = params;
      }
      nextToken();  //step ')'

      //If it's an identifier, the primary expression is actually an identifier-primary-expression
      func_call_expr->identifier = (IdentifierPrimaryExpression *) primary_expr;
      current_postfix_expr = func_call_expr;
    } else if (IS_PUNCTUATOR("++")) {

      nextToken();  // step '++'
      //increment
      IncrementPostFixExpression *inc_expr = new IncrementPostFixExpression;
      inc_expr->increment_operator = Operator::INCREMENT;
      current_postfix_expr = inc_expr;
    } else if (IS_PUNCTUATOR("--")) {
      nextToken();  // step '--'
      //decrement
      IncrementPostFixExpression *dec_expr = new IncrementPostFixExpression;
      current_postfix_expr = dec_expr;
      dec_expr->increment_operator = Operator::DECREMENT;
    } else {
      exit = true;
    }
    //only if the postfix expr is initialized
    if (current_postfix_expr != 0) {
      current_postfix_expr->postFixExpr = (PostFixExpression *) last_tmp_expr;
      last_tmp_expr = current_postfix_expr;
    }
  }
  *expr = last_tmp_expr;
  return true;
}

/**
 * AdditiveExpression
 */
bool Parser::parseAdditiveExpression(Expression **expr) {

  parseMultiplicativeExpression(expr);

  bool has_rhs = false;
  AdditiveExpression *additive_expr = new AdditiveExpression;
  if (IS_PUNCTUATOR("+")) {
    additive_expr->additive_operator = Operator::SUM;
    has_rhs = true;
  } else if (IS_PUNCTUATOR("-")) {
    additive_expr->additive_operator = Operator::SUB;
    has_rhs = true;
  }

  if (has_rhs) {
    //lhs
    additive_expr->lhs_multiplicative_expression = *expr;

    nextToken();  //step operator

    //rhs
    Expression *rhs_additive_expression;
    parseAdditiveExpression(&rhs_additive_expression);
    additive_expr->rhs_additive_expression = rhs_additive_expression;

    *expr = additive_expr;
  }
  return true;
}

/**
 * MultiplicativeExpression
 */
bool Parser::parseMultiplicativeExpression(Expression **expr) {

  parseUnaryExpression(expr);

  bool has_rhs = false;
  MultiplicativeExpression *multiplicative_expr = new MultiplicativeExpression;
  if (IS_PUNCTUATOR("*")) {
    multiplicative_expr->multiplicative_operator = Operator::MUL;
    has_rhs = true;
  } else if (IS_PUNCTUATOR("/")) {
    multiplicative_expr->multiplicative_operator = Operator::DIV;
    has_rhs = true;
  }

  if (has_rhs) {
    //lhs
    multiplicative_expr->lhs_unary_expression = *expr;

    nextToken();  //step operator

    //rhs
    Expression *rhs_additive_expression;
    parseAdditiveExpression(&rhs_additive_expression);
    multiplicative_expr->rhs_additive_expression = rhs_additive_expression;

    *expr = multiplicative_expr;
  }
  return true;
}

/**
 * PrimaryExpression
 */
bool Parser::parsePrimaryExpression(Expression **expr) {

  //create new primary expression instance
  PrimaryExpression *primary_expr = new PrimaryExpression;
  *expr = primary_expr;

  //identifier
  if (IS_TOKEN_TYPE(TokenType::IDENTIFIER)) {
    IdentifierPrimaryExpression *identifier_expr = new IdentifierPrimaryExpression;
    identifier_expr->string_value = _current_token.textual_content;
    nextToken();
    *expr = identifier_expr;

  } else if (isType(_current_token)) {
    primary_expr->string_value = _current_token.textual_content;
    nextToken();

  } else if (IS_TOKEN_TYPE(TokenType::BOOL_LITERAL)) {
    primary_expr->type = ExpressionType::BOOL;
    primary_expr->bool_value = _current_token.bool_content;
    nextToken();

  } else if (IS_TOKEN_TYPE(TokenType::NUMERIC_LITERAL_INT)) {
    primary_expr->type = ExpressionType::INTEGER;
    primary_expr->int_value = (int) _current_token.numeric_content;
    nextToken();

  } else if (IS_TOKEN_TYPE(TokenType::NUMERIC_LITERAL_FLOAT)) {
    primary_expr->type = ExpressionType::FLOAT;
    primary_expr->double_value = _current_token.numeric_content;
    nextToken();

  } else if (IS_TOKEN_TYPE(TokenType::NUMERIC_LITERAL_DOUBLE)) {
    primary_expr->type = ExpressionType::DOUBLE;
    primary_expr->double_value = _current_token.numeric_content;
    nextToken();

  } else if (IS_TOKEN_TYPE(TokenType::TEXTUAL_LITERAL)) {
    primary_expr->type = ExpressionType::STRING;
    primary_expr->string_value = _current_token.textual_content;
    nextToken();

  } else if (IS_PUNCTUATOR("(")) {
    primary_expr->type = ExpressionType::EXPR;
    nextToken();  // eat '('
    LogicalOrExpression *logical_or_expr = 0;
    parseLogicalOrExpression((Expression **) &logical_or_expr);
    primary_expr->expression = logical_or_expr;
    nextToken();  // eat ')'
  } else {
    ERROR_PRINT("invalid expression type");
  }
  return true;
}

/**
 * Checks if a token is a type (primitive or reference)
 */
bool Parser::isType(Token &token) {

  //float
  if (token.textual_content == "float") {
    return true;
  }
  //doule
  if (token.textual_content == "double") {
    return true;
  }
  //boolean
  if (token.textual_content == "bool") {
    return true;
  }

  //character
  if (token.textual_content == "char") {
    return true;
  }
  //string
  if (token.textual_content == "string") {
    return true;
  }
  //integer
  if (token.textual_content == "int") {
    return true;
  }
  return false;
}

/**
 * sets the next and lookahead token
 */
void Parser::nextToken() {

  _current_token = _lookahead_token;
  _tokenizer->next(_lookahead_token);
}
