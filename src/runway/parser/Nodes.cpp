//
// Nodes.cpp
// Implementation of the nodes
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "parser/Nodes.hpp"

llvm::Value *AdditiveExpression::emit(Generator *generator) {

  return generator->emitAdditiveExpression(this);
}

llvm::Value *LogicalOrExpression::emit(Generator *generator) {

  return generator->emitLogicalOrExpression(this);
}

llvm::Value *LogicalAndExpression::emit(Generator *generator) {

  return generator->emitLogicalAndExpression(this);
}

llvm::Value *EqualityExpression::emit(Generator *generator) {

  return generator->emitEqualityExpression(this);
}

llvm::Value *UnaryExpression::emit(Generator *generator) {

  return generator->emitUnaryExpression(this);
}

llvm::Value *MultiplicativeExpression::emit(Generator *generator) {

  return generator->emitMultiplicativeExpression(this);
}

llvm::Value *IncrementPostFixExpression::emit(Generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FunctionCallPostfixExpression::emit(Generator *generator) {

  return generator->emitFunctionCallPostFixExpression(this);
}

llvm::Value *ArrayPostFixExpression::emit(Generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FieldAccessPostFixExpression::emit(Generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *IdentifierPrimaryExpression::emit(Generator *generator) {

  return generator->emitIdentifierPrimaryExpression(this);
}

llvm::Value *ConditionalExpression::emit(Generator *generator) {

  //TODO impl
  return nullptr;
}
llvm::Value *AssignmentExpression::emit(Generator *generator) {

  return generator->emitAssignmentExpression(this);
}

llvm::Value *RelationalExpression::emit(Generator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *PrimaryExpression::emit(Generator *generator) {

  return generator->emitPrimaryExpression(this);
}

void IfStatement::emit(Generator *generator) {
  //TODO impl
}

void ForStatement::emit(Generator *generator) {
  //TODO impl
}

void JumpStatement::emit(Generator *generator) {
  //TODO impl
}

void ReturnStatement::emit(Generator *generator) {
  //TODO impl
}

void WhileLoopStatement::emit(Generator *generator) {
  //TODO impl
}

void VariableDeclarationStatement::emit(Generator *generator) {

  generator->emitVariableDeclarationStatement(this);
}

void BodyStatement::emit(Generator *generator) {

  //TODO impl
}

void ExpressionStatement::emit(Generator *generator) {

  generator->emitExpressionStatement(this);
}


AssignmentExpression::AssignmentExpression() : expression_to_assign(nullptr) { }