//
// nodes.cpp
// Implementation of the nodes
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <parser/nodes.hpp>

llvm::Value *AdditiveExpression::Emit(BaseGenerator *generator) {

  return generator->EmitAdditiveExpression(this);
}

llvm::Value *LogicalOrExpression::Emit(BaseGenerator *generator) {

  return generator->EmitLogicalOrExpression(this);
}

llvm::Value *LogicalAndExpression::Emit(BaseGenerator *generator) {

  return generator->EmitLogicalAndExpression(this);
}

llvm::Value *EqualityExpression::Emit(BaseGenerator *generator) {

  return generator->EmitEqualityExpression(this);
}

llvm::Value *UnaryExpression::Emit(BaseGenerator *generator) {

  return generator->EmitUnaryExpression(this);
}

llvm::Value *MultiplicativeExpression::Emit(BaseGenerator *generator) {

  return generator->EmitMultiplicativeExpression(this);
}

llvm::Value *IncrementPostFixExpression::Emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FunctionCallPostfixExpression::Emit(BaseGenerator *generator) {

  return generator->EmitFunctionCallPostFixExpression(this);
}

llvm::Value *ArrayPostFixExpression::Emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *FieldAccessPostFixExpression::Emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *IdentifierPrimaryExpression::Emit(BaseGenerator *generator) {

  return generator->EmitIdentifierPrimaryExpression(this);
}

llvm::Value *ConditionalExpression::Emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}
llvm::Value *AssignmentExpression::Emit(BaseGenerator *generator) {

  return generator->EmitAssignmentExpression(this);
}

llvm::Value *RelationalExpression::Emit(BaseGenerator *generator) {

  //TODO impl
  return nullptr;
}

llvm::Value *PrimaryExpression::Emit(BaseGenerator *generator) {

  return generator->EmitPrimaryExpression(this);
}

void IfStatement::Emit(BaseGenerator *generator) {

  generator->EmitIfStatement(this);
}

void ForStatement::Emit(BaseGenerator *generator) {
  //TODO impl
}

void JumpStatement::Emit(BaseGenerator *generator) {
  //TODO impl
}

void ReturnStatement::Emit(BaseGenerator *generator) {
  //TODO impl
}

void WhileLoopStatement::Emit(BaseGenerator *generator) {
  //TODO impl
}

void VariableDeclarationStatement::Emit(BaseGenerator *generator) {

  generator->EmitVariableDeclarationStatement(this);
}

void BodyStatement::Emit(BaseGenerator *generator) {

  generator->EmitBodyStatement(this);
}

void ExpressionStatement::Emit(BaseGenerator *generator) {

  generator->EmitExpressionStatement(this);
}

AssignmentExpression::AssignmentExpression() : expression_to_assign(nullptr) { }

VariableDeclarationStatement::VariableDeclarationStatement() : expression_to_assign(nullptr) { }

Expression::Expression() : type(ExpressionType::UNDEFINED){ }
