//
// Generator.cpp
// Code generation implementation
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "codegen/Generator.hpp"
#include "parser/Parser.hpp"
#include "tools/codegen_datatypes.hpp"
#include "tools/codegen.h"

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
 * Identifier
 */
llvm::Value *Generator::emitIdentifierPrimaryExpression(Expression *expr) {

  IdentifierPrimaryExpression *identifier = (IdentifierPrimaryExpression *) expr;
  rw_symtable_entry *value = _values[identifier->string_value];

  delete identifier;

  if (value == nullptr) {
    std::cerr << "use of undeclared identifier " << identifier << std::endl;
    return nullptr;
  }

  return value->llvm_ptr;
}

/**
 * Emits code for a function call statement
 */
llvm::Value *Generator::emitFunctionCallPostFixExpression(FunctionCallPostfixExpression *func_call_expr) {

  std::string identifier = func_call_expr->identifier->string_value;
  if (!identifier.compare("print")) {
    createPrintFunction(func_call_expr->arguments.at(0), false);
  } else if (!identifier.compare("println")) {
    createPrintFunction(func_call_expr->arguments.at(0), true);
  }
  delete func_call_expr;
  return nullptr;
}

/**
 * Creates a print function
 *
 * param: parameter expression
 * param: true, if the function should make a line break
 */
void Generator::createPrintFunction(Expression *parameter_expr, bool new_line) {

  const std::string const_function_name = "printf";
  const std::string const_form_string_arg = "%s";
  const std::string const_form_int_arg = "%i";
  const std::string const_form_float_arg = "%f";
  const std::string const_new_line_string = "\n";

  llvm::Value *llvm_parameter_value = parameter_expr->emit(this);

  //when it's a pointer type load the value
  if (llvm_parameter_value->getType()->isPointerTy()) {
    llvm_parameter_value = _builder->CreateLoad(llvm_parameter_value);
  }

  /**
   * Look in symbol table for existing function
   */
  llvm::Function *llvm_print_func = _functions[const_function_name];
  if (llvm_print_func == nullptr) {
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));
    llvm::FunctionType *llvm_printf_type =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), printf_arg_types, true);
    llvm_print_func =
        llvm::Function::Create(llvm_printf_type, llvm::Function::ExternalLinkage, const_function_name, _module);
    llvm_print_func->setCallingConv(llvm::CallingConv::C);
    _functions[const_function_name] = llvm_print_func;
  }

  /**
   * Determine which kind of expression we have (numeric, string, bool, ...)
   */
  llvm::Type *llvm_parameter_type = llvm_parameter_value->getType();
  std::string printf_parameter_format;

  if (llvm_parameter_type->isFloatTy() || llvm_parameter_type->isDoubleTy()) {  //todo impl double
    printf_parameter_format = const_form_float_arg;
  } else if (llvm_parameter_type->isIntegerTy()) {
    printf_parameter_format = const_form_int_arg;
  } else {
    printf_parameter_format = const_form_string_arg;
  }

  if (new_line) {
    printf_parameter_format.append(const_new_line_string);
  }

  std::vector<llvm::Value *> param_values;
  param_values.push_back(_builder->CreateGlobalStringPtr(printf_parameter_format));

  param_values.push_back(llvm_parameter_value);
  llvm::ArrayRef<llvm::Value *> llvm_func_arguments(param_values);

  /**
   * Finally create the function call call
   */
  _builder->CreateCall(llvm_print_func, llvm_func_arguments);
}

/**
 * Allocates stack space for a variable declaration statement and assign the value if there's an assignment available
 *
 * param: the parsed variable declaration statement
 */
void Generator::emitVariableDeclarationStatement(VariableDeclarationStatement *var_decl_stmt) {

  std::string identifier = var_decl_stmt->identifier->string_value;

  llvm::Type *llvm_variable_type = nullptr;
  ExpressionType expression_type = var_decl_stmt->type->type;

  //set the llvm type
  switch (expression_type) {
    case ExpressionType::BOOL:
      llvm_variable_type = llvm::Type::getInt1Ty(llvm::getGlobalContext());
      break;
    case ExpressionType::FLOAT:
      llvm_variable_type = llvm::Type::getFloatTy(llvm::getGlobalContext());
      break;
    case ExpressionType::DOUBLE:
      llvm_variable_type = llvm::Type::getDoubleTy(llvm::getGlobalContext());
      break;
    case ExpressionType::INTEGER:
      llvm_variable_type = llvm::Type::getInt32Ty(llvm::getGlobalContext());
      break;
    case ExpressionType::STRING:
      llvm_variable_type = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
      break;
  }

  //allocate stack space
  llvm::AllocaInst *llvm_alloca_inst = new llvm::AllocaInst(llvm_variable_type, identifier, _insert_point);

  //save the pointer of the value in the symbol table
  rw_symtable_entry *variable_declaration_entry = new rw_symtable_entry;

  variable_declaration_entry->llvm_ptr = llvm_alloca_inst;
  variable_declaration_entry->type = expression_type;
  _values[identifier] = variable_declaration_entry;

  //if there's a assignment beside the variable declaration, emit the rhs assignment value
  AssignmentExpression *assignment_expr = var_decl_stmt->expression_to_assign;
  if (assignment_expr != nullptr) {
    assignment_expr->emit(this);
  }
  delete var_decl_stmt;
}

/**
 * Stores the rhs of an assignment expression
 *
 * param: the parsed assignment expression
 * return: the pointer to the new assigned value
 */
llvm::Value *Generator::emitAssignmentExpression(AssignmentExpression *assignment_expr) {

  std::string identifier = assignment_expr->identifier->string_value;

  //check if the type in assignment expression matches the allocated type
  ExpressionType declared_type = _values[identifier]->type;
  ExpressionType assigned_type = assignment_expr->expression_to_assign->type;

  llvm::Value *llvm_emitted_assignment_value = assignment_expr->expression_to_assign->emit(this);

  if ((declared_type == ExpressionType::FLOAT || declared_type == ExpressionType::DOUBLE)
      && assigned_type == ExpressionType::INTEGER) {
    //cast from int to float/double
    llvm::ConstantInt *integer_value = (llvm::ConstantInt *) llvm_emitted_assignment_value;
    llvm_emitted_assignment_value = createLlvmFpValue(integer_value->getSExtValue(), declared_type);
  } else if (declared_type == ExpressionType::FLOAT && assigned_type == ExpressionType::DOUBLE) {
    WARN_PRINTLN("precision loss: conversion from double to float is currently not supported");
    return nullptr;
  } else if (declared_type == ExpressionType::DOUBLE && assigned_type == ExpressionType::FLOAT) {
    llvm::ConstantFP *fp_value = (llvm::ConstantFP *) llvm_emitted_assignment_value;
    double value = fp_value->getValueAPF().convertToFloat();
    llvm_emitted_assignment_value = createLlvmFpValue(value, ExpressionType::DOUBLE);
  }

  rw_symtable_entry *entry = _values[identifier];
  //when there's no value in symbol table print error
  if (entry == nullptr) {
    ERR_PRINTLN("Use of undeclared identifier " << " " << identifier);
    return nullptr;
  }

  new llvm::StoreInst(llvm_emitted_assignment_value, entry->llvm_ptr, false, _insert_point);

  delete assignment_expr;
  return entry->llvm_ptr;
}

/**
 * Emits lhs and rhs and returns the compared result
 *
 * param: logical or expression
 */
llvm::Value *Generator::emitLogicalOrExpression(LogicalOrExpression *expr) {

  llvm::Value *lhs_value = expr->lhs_logical_and_expr->emit(this);
  llvm::Value *rhs_value = expr->rhs_logical_or_expr->emit(this);

  return createCompareResult(lhs_value, rhs_value, Operator::LOGICAL_OR);
}

/**
 * Emits lhs and rhs and returns the compared result
 *
 * param: logical and expression
 */
llvm::Value *Generator::emitLogicalAndExpression(LogicalAndExpression *expr) {

  llvm::Value *lhs_value = expr->lhs_equality_expr->emit(this);
  llvm::Value *rhs_value = expr->rhs_logical_or_expr->emit(this);

  return createCompareResult(lhs_value, rhs_value, Operator::LOGICAL_AND);
}

/**
 *
 */
llvm::Value *Generator::emitAdditiveExpression(AdditiveExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_multiplicative_expression->emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_additive_expression->emit(this);

  llvm::Value *llvm_result_value = nullptr;

  bool floating_point =
      llvm_lhs_value->getType()->isFloatingPointTy() || llvm_rhs_value->getType()->isFloatingPointTy();

  if (expr->additive_operator == Operator::SUM) {
    if (floating_point) {
      llvm_result_value = _builder->CreateFAdd(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = _builder->CreateAdd(llvm_lhs_value, llvm_rhs_value);
    }
  } else if (expr->additive_operator == Operator::SUB) {
    if (floating_point) {
      llvm_result_value = _builder->CreateFSub(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = _builder->CreateSub(llvm_lhs_value, llvm_rhs_value);
    }
  } else {
    ERR_PRINTLN("undefined operator for additive operation");
  }

  delete expr;

  return llvm_result_value;
}

/**
 *
 */
llvm::Value *Generator::emitMultiplicativeExpression(MultiplicativeExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_unary_expression->emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_additive_expression->emit(this);

  llvm::Value *llvm_result_value = nullptr;

  bool floating_point =
      llvm_lhs_value->getType()->isFloatingPointTy() || llvm_rhs_value->getType()->isFloatingPointTy();

  if (expr->multiplicative_operator == Operator::MUL) {
    if (floating_point) {
      llvm_result_value = _builder->CreateFMul(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = _builder->CreateMul(llvm_lhs_value, llvm_rhs_value);
    }
  } else if (expr->multiplicative_operator == Operator::DIV) {
    if (floating_point) {
      llvm_result_value = _builder->CreateFMul(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = _builder->CreateMul(llvm_lhs_value, llvm_rhs_value);
    }
    llvm_result_value = _builder->CreateExactSDiv(llvm_lhs_value, llvm_rhs_value);
  } else {
    ERR_PRINTLN("undefined operator for multiplicative operation");
  }

  delete expr;
  delete llvm_rhs_value;
  delete llvm_lhs_value;

  return llvm_result_value;
}

/**
 * PrimaryExpression
 *
 * CASES: BOOL, NUM, STRING, EXPR
 */
llvm::Value *Generator::emitPrimaryExpression(PrimaryExpression *expr) {

  //determine the type of the declared expression
  ExpressionType expr_type = expr->type;
  if (expr_type == ExpressionType::BOOL) {
    uint8_t value = expr->bool_value;
    return llvm::ConstantInt::getIntegerValue(llvm::Type::getInt1Ty(llvm::getGlobalContext()), llvm::APInt(1, value));

  } else if (expr_type == ExpressionType::STRING) {
    return _builder->CreateGlobalStringPtr(expr->string_value);

  } else if (expr_type == ExpressionType::FLOAT || expr_type == ExpressionType::DOUBLE) {

    delete expr;
    return createLlvmFpValue(expr->double_value, expr_type);

  } else if (expr_type == ExpressionType::INTEGER) {

    delete expr;
    return createLlvmIntValue(expr->int_value, expr_type);
  }
  return nullptr;
}

/**
 *
 */
llvm::Value *Generator::emitEqualityExpression(EqualityExpression *expr) {

  Expression *lhs_expr = expr->lhs_relational_expr;
  Expression *rhs_expr = expr->rhs_equality_expr;

  llvm::Value *lhs_value = lhs_expr->emit(this);
  llvm::Value *rhs_value = rhs_expr->emit(this);

  llvm::Value *result_value = nullptr;

  Operator compare_operator = expr->compare_operator;
  if (compare_operator == Operator::COMPARE_EQUAL) {
    result_value = _builder->CreateICmpEQ(lhs_value, rhs_value);

  } else if (compare_operator == Operator::COMPARE_GREATER) {
    result_value = _builder->CreateICmpSGT(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_GREATER_OR_EQUAL) {
    result_value = _builder->CreateICmpSGE(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_LESS) {
    result_value = _builder->CreateICmpSLT(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_LESS_OR_EQUAL) {
    result_value = _builder->CreateICmpSLE(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_UNEQUAL) {
    result_value = _builder->CreateICmpNE(rhs_value, lhs_value);

  } else {
    ERR_PRINTLN("INVALID COMPARE OPERATOR");
  }
  return result_value;
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

  llvm::FunctionType *main_function_type = llvm::FunctionType::get(_builder->getVoidTy(), false);
  llvm::Function
      *main_function = llvm::Function::Create(main_function_type, llvm::Function::ExternalLinkage, "main", _module);

  _functions["main"] = main_function;
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

  Expression *expression = expr_stmt->expression;
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

/**
 * Emits code for a for-loop
 */
void Generator::emitForStatement(ForStatement *for_statment) {

  llvm::BasicBlock *for_instructions =
      llvm::BasicBlock::Create(llvm::getGlobalContext(), "forinst", _functions["main"], _insert_point);

}

/**
 *
 */
void Generator::emitIfStatement(IfStatement *if_statement) {

  llvm::Value *expr = if_statement->condition->emit(this);
  // Convert condition to a bool by comparing equal to 0.0.
  llvm::Value *condition = _builder->CreateFCmpONE(
      expr, llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(0.0)), "ifcond");

  llvm::BasicBlock *cond_true = llvm::BasicBlock::Create(llvm::getGlobalContext());
  if_statement->body->emit(this);

  _builder->CreateCondBr(condition, cond_true, _insert_point);
}

/**
 *
 */
void Generator::emitBodyStatement(BodyStatement *body_statement) {

  for (int i = 0; i < body_statement->statements.size(); ++i) {
    Statement *stmt = body_statement->statements.at(i);
    stmt->emit(this);
    delete stmt;
  }
}