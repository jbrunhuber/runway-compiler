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

  llvm::Value *EmitFunctionCallPostFixExpression(FunctionCallPostfixExpression *);

  llvm::Value *EmitLogicalOrExpression(LogicalOrExpression *);

  llvm::Value *EmitLogicalAndExpression(LogicalAndExpression *);

  llvm::Value *EmitEqualityExpression(EqualityExpression *);

  llvm::Value *EmitRelationalExpression(RelationalExpression *);

  llvm::Value *EmitAdditiveExpression(AdditiveExpression *);

  llvm::Value *EmitMultiplicativeExpression(MultiplicativeExpression *);

  llvm::Value *EmitUnaryExpression(UnaryExpression *);

  llvm::Value *EmitPostFixExpression(PostFixExpression *);

  virtual llvm::Value *EmitAssignmentExpression(AssignmentExpression *);

  llvm::Value *EmitIdentifierPrimaryExpression(Expression *);

  llvm::Value *EmitPrimaryExpression(PrimaryExpression *);

  void EmitForStatement(ForStatement *);

  /**
   * Emits a variable declaration statement
   */
  void EmitVariableDeclarationStatement(VariableDeclarationStatement *);

  void EmitIfStatement(IfStatement *);

  void EmitReturnStatement(ReturnStatement *);

  void EmitWhileLoopStatement(WhileLoopStatement *);

  void EmitExpressionStatement(ExpressionStatement *);

  /**
   * Emits iterative statements until terminating '}' occur
   */
  void EmitBodyStatement(BodyStatement *);

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
  virtual void SetInsertPoint(llvm::BasicBlock *);

  /**
   * Returns the emitted IR (llvm) as a string
   */
  std::string GetIR();

  void create_print_function(Expression *, bool);

  llvm::BasicBlock *insert_point;

 protected:

  /**
   * Emits the base assignment expression
   */
  llvm::Value *do_assignment(AssignmentExpression *assignment_expr);

  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  std::map<std::string, llvm::Function *> _functions;
  std::stack<ScopeBlock *> *block_stack;
};

#endif /* defined (RUNWAY_CODEGEN_GENERATOR_HPP) */
