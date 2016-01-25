//
// nodes.cpp
// Implementation of the nodes
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <parser/nodes.hpp>

llvm::Value *AdditiveExpression::emit(BaseGenerator *generator) {

  return generator->emitAdditiveExpression(this);
}

llvm::Value *LogicalOrExpression::emit(BaseGenerator *generator) {

  return generator->emitLogicalOrExpression(this);
}

llvm::Value *LogicalAndExpression::emit(BaseGenerator *generator) {

  return generator->emitLogicalAndExpression(this);
}

llvm::Value *EqualityExpression::emit(BaseGenerator *generator) {

  return generator->emitEqualityExpression(this);
}

llvm::Value *UnaryExpression::emit(BaseGenerator *generator) {

  return generator->emitUnaryExpression(this);
}

llvm::Value *MultiplicativeExpression::emit(BaseGenerator *generator) {

  return generator->emitMultiplicativeExpression(this);
}

llvm::Value *IncrementPostFixExpression::emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FunctionCallPostfixExpression::emit(BaseGenerator *generator) {

  return generator->emitFunctionCallPostFixExpression(this);
}

llvm::Value *ArrayPostFixExpression::emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FieldAccessPostFixExpression::emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *IdentifierPrimaryExpression::emit(BaseGenerator *generator) {

  return generator->emitIdentifierPrimaryExpression(this);
}

llvm::Value *ConditionalExpression::emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}
llvm::Value *AssignmentExpression::emit(BaseGenerator *generator) {

  return generator->emitAssignmentExpression(this);
}

llvm::Value *RelationalExpression::emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *PrimaryExpression::emit(BaseGenerator *generator) {

  return generator->emitPrimaryExpression(this);
}

void IfStatement::emit(BaseGenerator *generator) {

  generator->emitIfStatement(this);
}

void ForStatement::emit(BaseGenerator *generator) {
  //TODO impl
}

void JumpStatement::emit(BaseGenerator *generator) {
  //TODO impl
}

void ReturnStatement::emit(BaseGenerator *generator) {
  //TODO impl
}

void WhileLoopStatement::emit(BaseGenerator *generator) {
  //TODO impl
}

void VariableDeclarationStatement::emit(BaseGenerator *generator) {

  generator->emitVariableDeclarationStatement(this);
}

void BodyStatement::emit(BaseGenerator *generator) {

  generator->emitBodyStatement(this);
}

void ExpressionStatement::emit(BaseGenerator *generator) {

  generator->emitExpressionStatement(this);
}

AssignmentExpression::AssignmentExpression() : expression_to_assign(nullptr) { }

VariableDeclarationStatement::VariableDeclarationStatement() : expression_to_assign(nullptr) { }

Expression::Expression() : type(ExpressionType::UNDEFINED){ }
