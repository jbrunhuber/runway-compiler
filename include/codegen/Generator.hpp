//
// Generator.hpp
// code generation visitor
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_GENERATOR_HPP
#define RUNWAY_CODEGEN_GENERATOR_HPP

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

#include "logger.h"
#include "parser/Nodes.hpp"
#include "codegen/rw_symtable_entry.hpp"

class rw_symtable_entry;
enum class ExpressionType;

class Generator {
 public:

  Generator();

  ~Generator();

  /**
   * Generates LLVM-IR for expressions
   */

  llvm::Value *emitFunctionCallPostFixExpression(FunctionCallPostfixExpression *);

  llvm::Value *emitLogicalOrExpression(LogicalOrExpression *);

  llvm::Value *emitLogicalAndExpression(LogicalAndExpression *);

  llvm::Value *emitEqualityExpression(EqualityExpression *);

  llvm::Value *emitRelationalExpression(RelationalExpression *);

  llvm::Value *emitAdditiveExpression(AdditiveExpression *);

  llvm::Value *emitMultiplicativeExpression(MultiplicativeExpression *);

  llvm::Value *emitUnaryExpression(UnaryExpression *);

  llvm::Value *emitPostFixExpression(PostFixExpression *);

  llvm::Value *emitAssignmentExpression(AssignmentExpression *);

  llvm::Value *emitIdentifierPrimaryExpression(Expression*);

  llvm::Value *emitPrimaryExpression(PrimaryExpression *);

  void emitVariableDeclarationStatement(VariableDeclarationStatement *);

  void emitIfStatement(IfStatement*);

  void emitReturnStatement(ReturnStatement*);

  void emitWhileLoopStatement(WhileLoopStatement*);

  void emitExpressionStatement(ExpressionStatement*);

  void createPrintFunction(Expression *, bool);

  llvm::Value* createLlvmFpValue(double, ExpressionType);

  llvm::Value* createLlvmIntValue(int64_t, ExpressionType);

  // Creates base construct
  void construct();

  void finalize();

  std::string getIR();

 private:
  llvm::Module *_module;
  llvm::IRBuilder<> *_builder;
  llvm::BasicBlock *_insert_point;
  std::map<std::string, rw_symtable_entry *> _values;
  std::map<std::string, llvm::Function *> _functions;
};

#endif /* defined (RUNWAY_CODEGEN_GENERATOR_HPP) */
