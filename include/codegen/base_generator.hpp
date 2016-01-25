//
// base_generator.hpp
// code generation visitor
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_GENERATOR_HPP
#define RUNWAY_CODEGEN_GENERATOR_HPP

#include <iostream>
#include <algorithm>
#include <memory>
#include <stack>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

#include <logger.h>
#include <tools/codegen_datatypes.hpp>
#include <codegen/scope_block.hpp>
#include <parser/nodes.hpp>
#include <parser/parser.hpp>

enum class ExpressionType;
class ScopeBlock;

class BaseGenerator {
 public:

  /**
   * constructs a base generator and initializes required components such as irbuilder, module and insertpoint
   */
  BaseGenerator();

  /**
   * constructs a base generator and sets required components
   */
  BaseGenerator(llvm::Module *, llvm::IRBuilder<> *, llvm::BasicBlock *);

  /**
   * Destroyes the generator :)
   * deletes the module and the irbuilder
   */
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

  /**
   * Emits a variable declaration statement
   */
  void emitVariableDeclarationStatement(VariableDeclarationStatement *);

  void emitIfStatement(IfStatement *);

  void emitReturnStatement(ReturnStatement *);

  void emitWhileLoopStatement(WhileLoopStatement *);

  void emitExpressionStatement(ExpressionStatement *);

  void createPrintFunction(Expression *, bool);

  /**
   * Emits iterative statements until terminating '}' occur
   */
  void emitBodyStatement(BodyStatement *);

  /**
   * Creates a main function and initializes required elements
   */
  void construct();

  /**
   * creates a void return statement at the bottom of the parent function
   */
  void finalize();

  /**
   * Sets the insert point for the builder
   */
  virtual void setInsertPoint(llvm::BasicBlock *);

  /**
   * Returns the emitted IR (llvm) as a string
   */
  std::string getIR();

  llvm::BasicBlock *insert_point;

 protected:

  /**
   * Emits the base assignment expression
   */
  llvm::Value *doAssignment(AssignmentExpression *assignment_expr);

  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  std::map<std::string, llvm::Function *> _functions;
  std::stack<ScopeBlock *> *block_stack;
};

#endif /* defined (RUNWAY_CODEGEN_GENERATOR_HPP) */
