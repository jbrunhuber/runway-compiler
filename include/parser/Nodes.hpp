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
#include "../../include/codegen/Generator.hpp"

class Generator;

/**
 *
 */
class Node {
};

/**
 * Statement
 */
class Statement : public Node {
 public:
  virtual void emit(Generator*) = 0;
};

class ExpressionStatement : public Statement {
 public:
  void emit(Generator*);
  Expression *expression;
};

/**
 * Expression
 */
class Expression {
 public:
  virtual llvm::Value *emit(Generator *) = 0;
};

class ReturnStatement : public Statement {
 public:
  void emit(Generator*);
  Expression *expression_to_return;
};

/**
 * Additive
 */
class AdditiveExpression : public Expression {
 public:
  Operator additive_operator;
  MultiplicativeExpression *lhs_multiplicative_expression;
  AdditiveExpression *rhs_additive_expression;
  llvm::Value *emit(Generator *generator);
};

/**
 * Multiplicative
 */
class MultiplicativeExpression : public Expression {
 public:
  Expression *lhs_unary_expression;
  AdditiveExpression *rhs_additive_expression;
  Operator multiplicative_operator;
  llvm::Value *emit(Generator *generator);

};

/**
 *
 */
enum class ExpressionType {
  STRING,
  BOOL,
  IDENTIFIER,
  EXPR,
  FLOAT,
  INTEGER,
  NULL_PTR
};

/**
 * Primary
 */
class PrimaryExpression : public Expression {
 public:
  llvm::Value *emit(Generator *generator);
  int int_value;
  double double_value;
  bool bool_value;
  ExpressionType expr_type;
  std::string string_value;
  Expression *expression;
};

class IdentifierPrimaryExpression : public PrimaryExpression {
 public:
  llvm::Value *emit(Generator *generator);
};

class EmptyExpressionStatement : public ExpressionStatement {
};

/**
 * Assignment
 */
class AssignmentExpression : public Expression {
 public:
  IdentifierPrimaryExpression *identifier;
  Operator assignment_operator;
  Expression *expression_to_assign;
  ExpressionType type;
  llvm::Value *emit(Generator *generator);

};

/**
 * Condition
 */
class ConditionalExpression : public Expression {
 public:
  llvm::Value *emit(Generator *generator);
  Operator compare_operator;

};

/**
 *
 */
class LogicalOrExpression : public ConditionalExpression {
 public:
  LogicalAndExpression *lhs_logical_and_expr;
  LogicalOrExpression *rhs_logical_or_expr;
  llvm::Value *emit(Generator *generator);
};

/**
 *
 */
class PostFixExpression : public Expression {
 public:
  PostFixExpression *postFixExpr;
  virtual llvm::Value *emit(Generator *generator) = 0;
};

/**
 *
 */
class IncrementPostFixExpression : public PostFixExpression {
 public:
  Operator increment_operator;
  llvm::Value *emit(Generator *generator);
};

/**
 *
 */
class ArrayPostFixExpression : public PostFixExpression {
 public:
  llvm::Value *emit(Generator *generator);
  Expression *index;
};

/**
 *
 */
class FunctionCallPostfixExpression : public PostFixExpression {
 public:
  llvm::Value *emit(Generator *generator);
  std::vector<Expression *> arguments;
  IdentifierPrimaryExpression *identifier;
};

/**
 *
 */
class FieldAccessPostFixExpression : public PostFixExpression {
 public:
  llvm::Value *emit(Generator *generator);
  std::string identifier;
};

/**
 *
 */
class UnaryExpression : public Expression {
 public:
  Operator unary_operator;
  PostFixExpression *postfix_expr;
  llvm::Value *emit(Generator *generator);
};

/**
 *
 */
class LogicalAndExpression : public ConditionalExpression {
 public:
  EqualityExpression *lhs_equality_expr;
  LogicalOrExpression *rhs_logical_or_expr;
  llvm::Value *emit(Generator *generator);
};

/**
 *
 */
class EqualityExpression : public ConditionalExpression {
 public:
  Operator equality_operator;
  RelationalExpression *lhs_relational_expr;
  EqualityExpression *rhs_equality_expr;
  llvm::Value *emit(Generator *generator);
};

/**
 *
 */
class RelationalExpression : public ConditionalExpression {
 public:
  Operator relational_operator;
  Expression *lhs_additive_expression;
  Expression *rhs_relational_expr;
  llvm::Value *emit(Generator *generator);
};

/**
 * Body Statement
 */
class BodyStatement : public Statement {
 public:
  void emit(Generator*);
  std::vector<Statement *> statements;
};

/**
 * If condition
 */
class IfStatement : public Statement {
 public:
  void emit(Generator*);
  Expression *condition;
  Statement *statement;
};

/**
 * Variable declaration
 */
class VariableDeclarationStatement : public Statement {
 public:
  void emit(Generator*);
  IdentifierPrimaryExpression *identifier;
  PrimaryExpression *type;
  AssignmentExpression *expression_to_assign;
};

/**
 * JumpStatement
 */
class JumpStatement : public Statement {
 public:
  void emit(Generator*);
  JumpStatementType jump_statement_type;
};

/**
 *
 */
class TypeExpression : Expression {
 public:
  std::string type_identifier;
  uint32_t integer_size;
};

/**
 * Loop
 */
class LoopStatement : public Statement {
 public:
  virtual void emit(Generator*) = 0;
};

/**
 * While Loop
 */
class WhileLoopStatement : public LoopStatement {
 public:
  void emit(Generator*);
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

class ForStatement : public LoopStatement {
 public:
  void emit(Generator*);
  Statement *statement;
  VariableDeclarationStatement *initialization_stmt;
  Expression *termination_expr;
  Expression *increment_expr;
  ForType forType;
  Expression *range_start;
  Expression *range_end;
};

#endif /* defined(RUNWAY_PARSER_NODES_HPP) */
