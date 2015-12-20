//
// Nodes.hpp
// the nodes
//
// Created by Joshua Brunhuber on 29.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_PARSER_NODES_HPP
#define RUNWAY_PARSER_NODES_HPP

#include <vector>
#include <string>
#include <llvm/IR/Value.h>

#include "NodesForward.hpp"
#include "JumpStatementType.hpp"
#include "Operator.hpp"
#include "../../include/codegen/base_generator.hpp"

class base_generator;

/**
 *
 */
class Node {
};

/**
 *
 */
enum class ExpressionType {
  STRING,
  BOOL,
  EXPR,
  FLOAT,
  DOUBLE,
  INTEGER,
  NULL_PTR,
  UNDEFINED,
  IDENTIFIER,
};

/**
 * Statement
 */
class Statement: public Node {
 public:
  virtual void emit(base_generator *) = 0;
  virtual ~Statement() {
  }
};

class ExpressionStatement: public Statement {
 public:
  void emit(base_generator *);
  Expression *expression;
  virtual ~ExpressionStatement() {
  }
};

/**
 * Expression
 */
class Expression {
 public:
  virtual llvm::Value *emit(base_generator *) = 0;
  ExpressionType type;
  Expression();
  virtual ~Expression() {
  }
};

class ReturnStatement: public Statement {
 public:
  void emit(base_generator *);
  Expression *expression_to_return;
};

/**
 * Additive
 */
class AdditiveExpression: public Expression {
 public:
  Operator additive_operator;
  Expression *lhs_multiplicative_expression;
  Expression *rhs_additive_expression;
  llvm::Value *emit(base_generator *generator);
};

/**
 * Multiplicative
 */
class MultiplicativeExpression: public Expression {
 public:
  Expression *lhs_unary_expression;
  Expression *rhs_additive_expression;
  Operator multiplicative_operator;
  llvm::Value *emit(base_generator *generator);

};

/**
 * Primary
 */
class PrimaryExpression: public Expression {
 public:
  llvm::Value *emit(base_generator *generator);
  int int_value;
  double double_value;
  bool bool_value;
  std::string string_value;
  Expression *expression;
};

class IdentifierPrimaryExpression: public PrimaryExpression {
 public:
  llvm::Value *emit(base_generator *generator);
};

class EmptyExpressionStatement: public ExpressionStatement {
};

/**
 * Assignment
 */
class AssignmentExpression: public Expression {
 public:
  AssignmentExpression();
  IdentifierPrimaryExpression *identifier;
  Operator assignment_operator;
  Expression *expression_to_assign;
  llvm::Value *emit(base_generator *generator);

};

/**
 * Condition
 */
class ConditionalExpression: public Expression {
 public:
  llvm::Value *emit(base_generator *generator);
  Operator compare_operator;

};

/**
 *
 */
class LogicalOrExpression: public ConditionalExpression {
 public:
  Expression *lhs_logical_and_expr;
  Expression *rhs_logical_or_expr;
  llvm::Value *emit(base_generator *generator);
};

/**
 *
 */
class PostFixExpression: public Expression {
 public:
  PostFixExpression *postFixExpr;
  virtual llvm::Value *emit(base_generator *generator) = 0;
};

/**
 *
 */
class IncrementPostFixExpression: public PostFixExpression {
 public:
  Operator increment_operator;
  llvm::Value *emit(base_generator *generator);
};

/**
 *
 */
class ArrayPostFixExpression: public PostFixExpression {
 public:
  llvm::Value *emit(base_generator *generator);
  Expression *index;
};

/**
 *
 */
class FunctionCallPostfixExpression: public PostFixExpression {
 public:
  llvm::Value *emit(base_generator *generator);
  std::vector<Expression *> arguments;
  IdentifierPrimaryExpression *identifier;
};

/**
 *
 */
class FieldAccessPostFixExpression: public PostFixExpression {
 public:
  llvm::Value *emit(base_generator *generator);
  std::string identifier;
};

/**
 *
 */
class UnaryExpression: public Expression {
 public:
  Operator unary_operator;
  PostFixExpression *postfix_expr;
  llvm::Value *emit(base_generator *generator);
};

/**
 *
 */
class LogicalAndExpression: public ConditionalExpression {
 public:
  Expression *lhs_equality_expr;
  Expression *rhs_logical_or_expr;
  llvm::Value *emit(base_generator *generator);
};

/**
 *
 */
class EqualityExpression: public ConditionalExpression {
 public:
  Operator equality_operator;
  Expression *lhs_relational_expr;
  Expression *rhs_equality_expr;
  llvm::Value *emit(base_generator *generator);
};

/**
 *
 */
class RelationalExpression: public ConditionalExpression {
 public:
  Operator relational_operator;
  Expression *lhs_additive_expression;
  Expression *rhs_relational_expr;
  llvm::Value *emit(base_generator *generator);
};

/**
 * Body Statement
 */
class BodyStatement: public Statement {
 public:
  void emit(base_generator *);
  std::vector<Statement *> statements;
};

enum class ConditionType {
  IF, ELSE_IF, ELSE
};

/**
 * If condition
 */
class IfStatement: public Statement {
 public:
  void emit(base_generator *);
  Expression *condition;
  BodyStatement *body;
  IfStatement *elseif;
  ConditionType type;
};

/**
 * Variable declaration
 */
class VariableDeclarationStatement: public Statement {
 public:
  VariableDeclarationStatement();
  void emit(base_generator *);
  IdentifierPrimaryExpression *identifier;
  PrimaryExpression *type;
  AssignmentExpression *expression_to_assign;
};

/**
 * JumpStatement
 */
class JumpStatement: public Statement {
 public:
  void emit(base_generator *);
  JumpStatementType jump_statement_type;
};

/**
 *
 */
class TypeExpression: Expression {
 public:
  std::string type_identifier;
  uint32_t integer_size;
};

/**
 * Loop
 */
class LoopStatement: public Statement {
 public:
  virtual void emit(base_generator *) = 0;
};

/**
 * While Loop
 */
class WhileLoopStatement: public LoopStatement {
 public:
  void emit(base_generator *);
  Statement *statement;
  Expression *termination;
};

/**
 * Standard-For-Loop or enhanced runway-for-loop
 */
enum class ForType {
  STANDARD,
  RUNWAY
};

class ForStatement: public LoopStatement {
 public:
  void emit(base_generator *);
  Statement *statement;
  VariableDeclarationStatement *initialization_stmt;
  Expression *termination_expr;
  Expression *increment_expr;
  ForType forType;
  Expression *range_start;
  Expression *range_end;
};

#endif /* defined(RUNWAY_PARSER_NODES_HPP) */
