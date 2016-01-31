//
// nodes.hpp
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

#include <parser/jump_stmt_type.hpp>
#include <parser/nodes_forward.hpp>
#include <parser/operator.hpp>
#include <codegen/base_generator.hpp>

class BaseGenerator;

/**
 *
 */
class Node {
};


/**
 * Statement
 */
class Statement: public Node {
 public:
  virtual void Emit(BaseGenerator *) = 0;
  virtual ~Statement() {
  }
};

class BlockStatement: public Statement {
 public:
  void Emit(BaseGenerator *);
  std::vector<Statement *> statements;
};

class ExpressionStatement: public Statement {
 public:
  void Emit(BaseGenerator *);
  Expression *expression;
  virtual ~ExpressionStatement() {
  }
};

/**
 * Expression
 */
class Expression {
 public:
  Expression *lhs_expr;
  Expression *rhs_expr;
  virtual llvm::Value *Emit(BaseGenerator *) = 0;
  ElementType type;
  Expression();
  virtual ~Expression() {
  }
};

class ReturnStatement: public Statement {
 public:
  void Emit(BaseGenerator *);
  Expression *expression_to_return;
};

/**
 * Additive
 */
class AdditiveExpression: public Expression {
 public:
  Operator additive_operator;
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 * Multiplicative
 */
class MultiplicativeExpression: public Expression {
 public:
  Operator multiplicative_operator;
  llvm::Value *Emit(BaseGenerator *generator);

};

/**
 * Primary
 */
class PrimaryExpression: public Expression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
  int int_value;
  double double_value;
  bool bool_value;
  std::string string_value;
  Expression *expression;
};

class IdentifierPrimaryExpression: public PrimaryExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
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
  llvm::Value *Emit(BaseGenerator *generator);

};

/**
 * Condition
 */
class ConditionalExpression: public Expression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
  Operator compare_operator;

};

/**
 *
 */
class LogicalOrExpression: public ConditionalExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 *
 */
class PostFixExpression: public Expression {
 public:
  PostFixExpression *postFixExpr;
  virtual llvm::Value *Emit(BaseGenerator *generator) = 0;
};

/**
 *
 */
class IncrementPostFixExpression: public PostFixExpression {
 public:
  Operator increment_operator;
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 *
 */
class ArrayPostFixExpression: public PostFixExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
  Expression *index;
};

/**
 *
 */
class FunctionCallPostfixExpression: public PostFixExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
  std::vector<Expression *> arguments;
  IdentifierPrimaryExpression *identifier;
};

/**
 *
 */
class FieldAccessPostFixExpression: public PostFixExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
  std::string identifier;
};

/**
 *
 */
class UnaryExpression: public Expression {
 public:
  Operator unary_operator;
  PostFixExpression *postfix_expr;
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 *
 */
class LogicalAndExpression: public ConditionalExpression {
 public:
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 *
 */
class EqualityExpression: public ConditionalExpression {
 public:
  Operator equality_operator;
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 *
 */
class RelationalExpression: public ConditionalExpression {
 public:
  Operator relational_operator;
  llvm::Value *Emit(BaseGenerator *generator);
};

/**
 * If condition
 */
class IfStatement: public Statement {
 public:
  void Emit(BaseGenerator *);
  Expression *condition;
  Statement *statement;
  Statement *else_stmt;
};

/**
 * Variable declaration
 */
class VariableDeclarationStatement: public Statement {
 public:
  VariableDeclarationStatement();
  void Emit(BaseGenerator *);
  IdentifierPrimaryExpression *identifier;
  PrimaryExpression *type;
  AssignmentExpression *expression_to_assign;
};

/**
 * JumpStatement
 */
class JumpStatement: public Statement {
 public:
  void Emit(BaseGenerator *);
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
  virtual void Emit(BaseGenerator *) = 0;
};

/**
 * While Loop
 */
class WhileLoopStatement: public LoopStatement {
 public:
  void Emit(BaseGenerator *);
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
  void Emit(BaseGenerator *);
  Statement *statement;
  VariableDeclarationStatement *initialization_stmt;
  Expression *termination_expr;
  Expression *increment_expr;
  ForType forType;
  Expression *range_start;
  Expression *range_end;
};

#endif /* defined(RUNWAY_PARSER_NODES_HPP) */
