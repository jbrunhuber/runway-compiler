//
// Generator.cpp
// Code generation implementation
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "codegen/Generator.hpp"
#include <tools/DebugManager.hpp>
#include <parser/Parser.hpp>

/**
 * Creates the module and the IRBuilder
 */
Generator::Generator() {

  _module = new llvm::Module("runway", llvm::getGlobalContext());
  _builder = new llvm::IRBuilder<>(_module->getContext());
}

/**
 * Free up memory
 */
Generator::~Generator() {

  delete _module;
  delete _builder;
}

/**
 * Emits code for a function call statement
 */
llvm::Value *Generator::emitFunctionCallPostFixExpression(
    FunctionCallPostfixExpression *func_call_expr) {

  std::string identifier = func_call_expr->identifier->string_value;
  if (!identifier.compare("print") == 0) {
    DebugManager::printMessage("create print func", ModuleInfo::CODEGEN);
    createPrintFunction(func_call_expr->arguments.at(0), false);
  } else if (!identifier.compare("println") == 0) {
    DebugManager::printMessage("create println func", ModuleInfo::CODEGEN);
    createPrintFunction(func_call_expr->arguments.at(0), true);
  }
  return nullptr;
}

/**
 * Creates a print function for the specific type (bool, numeric, string, ...)
 */
void Generator::createPrintFunction(Expression *expr, bool new_line) {

  std::cout << "print" << std::endl;

  llvm::Value *llvm_value_expr = expr->emit(this);  //todo implement identifier (symbol table access)

  std::string print_function_identifier = "printf";

  /**
   * Look in symbol table for existing function
   */
  llvm::Function *llvm_print_func = _functions[print_function_identifier];
  if (llvm_print_func == nullptr) {
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(
        llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));
    llvm::FunctionType *llvm_printf_type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(llvm::getGlobalContext()), printf_arg_types,
        true);
    llvm_print_func = llvm::Function::Create(llvm_printf_type,
                                             llvm::Function::ExternalLinkage,
                                             print_function_identifier,
                                             _module);
    llvm_print_func->setCallingConv(llvm::CallingConv::C);
    _functions[print_function_identifier] = llvm_print_func;
  }

  /**
   * Determine which kind of expression we have (numeric, string, bool, ...)
   */
  ExpressionType expression_type;
  llvm::Type *llvm_value_type = llvm_value_expr->getType();
  std::string printf_function_type_spcifier;

  if (llvm_value_type->isFloatTy() || llvm_value_type->isDoubleTy()) {  //todo impl double
    expression_type = ExpressionType::FLOAT;
    printf_function_type_spcifier = "%f";
  } else if (llvm_value_type->isIntegerTy()) {
    expression_type = ExpressionType::INTEGER;
    printf_function_type_spcifier = "%i";
  } else {
    expression_type = ExpressionType::STRING;
    printf_function_type_spcifier = "%s";
  }
  llvm::Constant *llvm_printf_func_const = llvm_print_func;
  std::vector<llvm::Value *> param_values;
  param_values.push_back(
      _builder->CreateGlobalStringPtr(printf_function_type_spcifier));

  param_values.push_back(llvm_value_expr);
  llvm::ArrayRef<llvm::Value *> llvm_func_arguments(param_values);

  /**
   * Finally create the build call
   */
  _builder->CreateCall(llvm_print_func, llvm_func_arguments);

}

/**
 * Allocates space on the stack for the (primitive) type of the declared symbol
 */
void Generator::emitVariableDeclarationStatement(
    VariableDeclarationStatement *var_decl_stmt) {

  DebugManager::printMessage("variable declaration statement",
                             ModuleInfo::CODEGEN);

  if (var_decl_stmt == 0) {
    std::cerr << "variable declaration statement is empty" << std::endl;
    return;
  }

  //identifier
  std::string identifier = var_decl_stmt->identifier->string_value;

  DebugManager::printMessage("identifier", ModuleInfo::CODEGEN);

  //type
  llvm::Type *type = nullptr;
  ExpressionType expression_type = var_decl_stmt->type->expr_type;
  switch (expression_type) {
    case ExpressionType::BOOL:
      type = llvm::Type::getInt1Ty(llvm::getGlobalContext());
      break;
    case ExpressionType::FLOAT:
      type = llvm::Type::getFloatTy(llvm::getGlobalContext());
      break;
    case ExpressionType::INTEGER:
      type = llvm::Type::getInt32Ty(llvm::getGlobalContext());
      break;
  }

  DebugManager::printMessage("type", ModuleInfo::CODEGEN);

  //allocate stack space
  llvm::AllocaInst *llvm_alloca_inst = new llvm::AllocaInst(type, identifier,
                                                            _insert_point);
  llvm::Value *llvm_test_int = llvm::ConstantInt::getIntegerValue(
      llvm::Type::getInt32Ty(llvm::getGlobalContext()), llvm::APInt(32, 5));
  llvm::StoreInst *store = new llvm::StoreInst(llvm_test_int, llvm_alloca_inst,
                                               false, _insert_point);

  DebugManager::printMessage("alloca", ModuleInfo::CODEGEN);

  //store assignment value
  AssignmentExpression *assignment = var_decl_stmt->expression_to_assign;
  llvm::Value *llvm_assignment_value = nullptr;
  if (assignment != nullptr) {
    llvm_assignment_value = assignment->emit(this);
  }

  DebugManager::printMessage("value", ModuleInfo::CODEGEN);
  //symbol table
  _values[identifier] = llvm_assignment_value;
}

/**
 * Stores the assignment value to the asignee
 */
llvm::Value* Generator::emitAssignmentExpression(
    AssignmentExpression *assignment_expr) {

  llvm::Value *llvm_value = nullptr;

  //identifier
  std::string identifier = assignment_expr->identifier->string_value;

  //store the variable

  return llvm_value;
}

/**
 * LogicalOrExpression
 */
llvm::Value *Generator::emitLogicalOrExpression(LogicalOrExpression *expr) {

  std::unique_ptr<Expression> lhs_expr(expr->lhs_logical_and_expr);
  std::unique_ptr<LogicalOrExpression> rhs_expr(expr->rhs_logical_or_expr);

  std::unique_ptr<llvm::Value> lhs_value(lhs_expr->emit(this));
  std::unique_ptr<llvm::Value> rhs_value(rhs_expr->emit(this));

  //TODO compare

  return nullptr;
}

/**
 * LogicalAndExpression
 */
llvm::Value *Generator::emitLogicalAndExpression(LogicalAndExpression *expr) {

  std::unique_ptr<EqualityExpression> lhs_expr(expr->lhs_equality_expr);
  std::unique_ptr<LogicalOrExpression> rhs_expr(expr->rhs_logical_or_expr);

  std::unique_ptr<llvm::Value> lhs_value(lhs_expr->emit(this));
  std::unique_ptr<llvm::Value> rhs_value(rhs_expr->emit(this));

  //TODO compare
  return nullptr;
}

/**
 *
 */
llvm::Value *Generator::emitAdditiveExpression(AdditiveExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_multiplicative_expression->emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_additive_expression->emit(this);

  llvm::Value *llvm_result_value = 0;

  if (expr->additive_operator == Operator::SUM) {
    llvm_result_value = _builder->CreateAdd(llvm_lhs_value, llvm_rhs_value);
  } else if (expr->additive_operator == Operator::SUB) {
    llvm_result_value = _builder->CreateSub(llvm_lhs_value, llvm_rhs_value);
  } else {
    ERR_PRINTLN("undefined operator for additive operation");
  }
  return llvm_result_value;
}

/**
 *
 */
llvm::Value *Generator::emitMultiplicativeExpression(
    MultiplicativeExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_unary_expression->emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_additive_expression->emit(this);

  llvm::Value *llvm_result_value = 0;

  if (expr->multiplicative_operator == Operator::MUL) {
    llvm_result_value = _builder->CreateMul(llvm_lhs_value, llvm_rhs_value);
  } else if (expr->multiplicative_operator == Operator::DIV) {
    llvm_result_value = _builder->CreateExactSDiv(llvm_lhs_value,
                                                  llvm_rhs_value);
  } else {
    ERR_PRINTLN("undefined operator for multiplicative operation");
  }
  return llvm_result_value;
}

/**
 * PrimaryExpression
 *
 * CASES: BOOL, NUM, STRING, EXPR
 */
llvm::Constant *Generator::emitPrimaryExpression(PrimaryExpression *expr) {

  //determine the type of the declared expression
  ExpressionType expr_type = expr->expr_type;
  if (expr_type == ExpressionType::BOOL) {
    DebugManager::printMessage("emit primary expression BOOLEAN",
                               ModuleInfo::CODEGEN);
    uint8_t value = expr->bool_value;
    return llvm::ConstantInt::getIntegerValue(
        llvm::Type::getInt1Ty(llvm::getGlobalContext()), llvm::APInt(1, value));

  } else if (expr_type == ExpressionType::STRING) {
    return _builder->CreateGlobalString(expr->string_value);

  } else if (expr_type == ExpressionType::FLOAT) {
    double value = expr->double_value;
    return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(value));

  } else if (expr_type == ExpressionType::INTEGER) {
    int value = expr->int_value;
    return llvm::ConstantInt::getIntegerValue(
        llvm::Type::getInt32Ty(llvm::getGlobalContext()),
        llvm::APInt(32, value));
  }
  return nullptr;
}

/**
 *
 */
llvm::Value *Generator::emitEqualityExpression(EqualityExpression *expr) {

  std::unique_ptr<RelationalExpression> lhs_expr(expr->lhs_relational_expr);
  std::unique_ptr<EqualityExpression> rhs_expr(expr->rhs_equality_expr);

  std::unique_ptr<llvm::Value> lhs_value(lhs_expr->emit(this));
  std::unique_ptr<llvm::Value> rhs_value(rhs_expr->emit(this));

  std::shared_ptr<llvm::Value> result_value = 0;

  Operator compare_operator = expr->compare_operator;
  if (compare_operator == Operator::COMPARE_EQUAL) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpEQ(lhs_value.get(), rhs_value.get()));

  } else if (compare_operator == Operator::COMPARE_GREATER) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpSGT(rhs_value.get(), lhs_value.get()));

  } else if (compare_operator == Operator::COMPARE_GREATER_OR_EQUAL) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpSGE(rhs_value.get(), lhs_value.get()));

  } else if (compare_operator == Operator::COMPARE_LESS) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpSLT(rhs_value.get(), lhs_value.get()));

  } else if (compare_operator == Operator::COMPARE_LESS_OR_EQUAL) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpSLE(rhs_value.get(), lhs_value.get()));

  } else if (compare_operator == Operator::COMPARE_UNEQUAL) {
    result_value = std::shared_ptr<llvm::Value>(
        _builder->CreateICmpNE(rhs_value.get(), lhs_value.get()));

  } else {
    ERR_PRINTLN("INVALID COMPARE OPERATOR");
  }
  return result_value.get();
}

/**
 *
 */
llvm::Value *Generator::emitUnaryExpression(UnaryExpression *expr) {

  PostFixExpression *lhs_expr = expr->postfix_expr;

  std::unique_ptr<llvm::Value> value(lhs_expr->emit(this));

  Operator unary_operator = expr->unary_operator;
  if (unary_operator == Operator::INCREMENT) {
    return value.get();

  } else if (unary_operator == Operator::DECREMENT) {
    llvm::LoadInst *load_instr = _builder->CreateLoad(value.get());
    llvm::Value *dec = _builder->CreateSub(_builder->getInt8(1), load_instr);
    llvm::StoreInst *store_instr = _builder->CreateStore(dec, load_instr);
    return value.get();

  } else if (unary_operator == Operator::NEGATE) {
    //impl
  } else if (unary_operator == Operator::NEGATIVE) {
    //impl
  }
  return nullptr;
}

/**
 * Constructs the main function and the insert point
 */
void Generator::construct() {

  llvm::FunctionType *main_function_type = llvm::FunctionType::get(
      _builder->getVoidTy(), false);
  llvm::Function *main_function = llvm::Function::Create(
      main_function_type, llvm::Function::ExternalLinkage, "main", _module);

  _insert_point = llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                           "entrypoint", main_function);
  _builder->SetInsertPoint(_insert_point);
}

/**
 * Creates a void return statement
 */
void Generator::finalize() {

  _builder->CreateRetVoid();
}

/**
 *
 */
void Generator::emitExpressionStatement(ExpressionStatement *expr_stmt) {

  std::unique_ptr<Expression> expression(expr_stmt->expression);
  expression->emit(this);
}

/**
 * returns the llvm bytecode as string value
 */
std::string Generator::getIR() {

  std::string str;
  llvm::raw_string_ostream rso(str);
  _module->print(rso, nullptr);
  return str;
}