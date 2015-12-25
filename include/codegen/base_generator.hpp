//
// base_generator.hpp
// code generation visitor
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_GENERATOR_HPP
#define RUNWAY_CODEGEN_GENERATOR_HPP

#include <codegen/scope_block.hpp>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <algorithm>
#include <memory>
#include <stack>

#include <logger.h>
#include <parser/Nodes.hpp>
#include <parser/Parser.hpp>
#include <tools/codegen_datatypes.hpp>

enum class ExpressionType;
class ScopeBlock;

class BaseGenerator {
 public:

  BaseGenerator();
  BaseGenerator(llvm::Module *, llvm::IRBuilder<> *, llvm::BasicBlock *);

  virtual ~BaseGenerator();


  llvm::Value *emitFunctionCallPostFixExpression(FunctionCallPostfixExpression *);

  llvm::Value *emitLogicalOrExpression(LogicalOrExpression *);

  llvm::Value *emitLogicalAndExpression(LogicalAndExpression *);

  llvm::Value *emitEqualityExpression(EqualityExpression *);

  llvm::Value *emitRelationalExpression(RelationalExpression *);

  llvm::Value *emitAdditiveExpression(AdditiveExpression *);

  llvm::Value *emitMultiplicativeExpression(MultiplicativeExpression *);

  llvm::Value *emitUnaryExpression(UnaryExpression *);

  llvm::Value *emitPostFixExpression(PostFixExpression *);

  virtual llvm::Value *emitAssignmentExpression(AssignmentExpression *);

  llvm::Value *emitIdentifierPrimaryExpression(Expression *);

  llvm::Value *emitPrimaryExpression(PrimaryExpression *);

  void emitForStatement(ForStatement *);

  void emitVariableDeclarationStatement(VariableDeclarationStatement *);

  void emitIfStatement(IfStatement *);

  void emitReturnStatement(ReturnStatement *);

  void emitWhileLoopStatement(WhileLoopStatement *);

  void emitExpressionStatement(ExpressionStatement *);

  void createPrintFunction(Expression *, bool);

  void emitBodyStatement(BodyStatement *);

  void construct();

  void finalize();

  std::string getIR();

  llvm::BasicBlock *insert_point;

  virtual void setInsertPoint(llvm::BasicBlock *);

protected:
  llvm::Value *doAssignment(AssignmentExpression *assignment_expr);
  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  std::map<std::string, llvm::Function *> _functions;
  std::stack<ScopeBlock *> *block_stack;
};

#endif /* defined (RUNWAY_CODEGEN_GENERATOR_HPP) */
