//
// Nodes.cpp
// Implementation of the nodes
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "parser/Nodes.hpp"

llvm::Value *AdditiveExpression::emit(base_generator *generator) {

  return generator->emitAdditiveExpression(this);
}

llvm::Value *LogicalOrExpression::emit(base_generator *generator) {

  return generator->emitLogicalOrExpression(this);
}

llvm::Value *LogicalAndExpression::emit(base_generator *generator) {

  return generator->emitLogicalAndExpression(this);
}

llvm::Value *EqualityExpression::emit(base_generator *generator) {

  return generator->emitEqualityExpression(this);
}

llvm::Value *UnaryExpression::emit(base_generator *generator) {

  return generator->emitUnaryExpression(this);
}

llvm::Value *MultiplicativeExpression::emit(base_generator *generator) {

  return generator->emitMultiplicativeExpression(this);
}

llvm::Value *IncrementPostFixExpression::emit(base_generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FunctionCallPostfixExpression::emit(base_generator *generator) {

  return generator->emitFunctionCallPostFixExpression(this);
}

llvm::Value *ArrayPostFixExpression::emit(base_generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FieldAccessPostFixExpression::emit(base_generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *IdentifierPrimaryExpression::emit(base_generator *generator) {

  return generator->emitIdentifierPrimaryExpression(this);
}

llvm::Value *ConditionalExpression::emit(base_generator *generator) {

  //TODO impl
  return nullptr;
}
llvm::Value *AssignmentExpression::emit(base_generator *generator) {

  return generator->emitAssignmentExpression(this);
}

llvm::Value *RelationalExpression::emit(base_generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *PrimaryExpression::emit(base_generator *generator) {

  return generator->emitPrimaryExpression(this);
}

void IfStatement::emit(base_generator *generator) {

  generator->emitIfStatement(this);
}

void ForStatement::emit(base_generator *generator) {
  //TODO impl
}

void JumpStatement::emit(base_generator *generator) {
  //TODO impl
}

void ReturnStatement::emit(base_generator *generator) {
  //TODO impl
}

void WhileLoopStatement::emit(base_generator *generator) {
  //TODO impl
}

void VariableDeclarationStatement::emit(base_generator *generator) {

  generator->emitVariableDeclarationStatement(this);
}

void BodyStatement::emit(base_generator *generator) {

  //TODO impl
}

void ExpressionStatement::emit(base_generator *generator) {

  generator->emitExpressionStatement(this);
}

AssignmentExpression::AssignmentExpression() : expression_to_assign(nullptr) { }

VariableDeclarationStatement::VariableDeclarationStatement() : expression_to_assign(nullptr) { }

Expression::Expression() : type(ExpressionType::UNDEFINED){ }
