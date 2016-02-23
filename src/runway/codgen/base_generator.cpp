#include <codegen/base_generator.hpp>
#include <codegen/phi_generator.hpp>

BaseGenerator::BaseGenerator() : insert_point(nullptr) {

  this->symtable = new Symtable;
  this->module = new llvm::Module("runway", llvm::getGlobalContext());
  this->builder = new llvm::IRBuilder<>(module->getContext());
}

BaseGenerator::BaseGenerator(llvm::Module *llvm_module, llvm::IRBuilder<> *llvm_builder, llvm::BasicBlock *insert) {

  this->symtable = new Symtable;
  this->module = llvm_module;
  this->builder = llvm_builder;
  this->insert_point = insert;
}

BaseGenerator::~BaseGenerator() {

  delete module;
  delete builder;
}

void BaseGenerator::EmitBlockStatement(BlockStatement *block) {

  ScopeBlock *scope = new ScopeBlock;
  scope->id = ++scopeid;
  symtable->Push(scope);
  for (unsigned i = 0; i < block->statements.size(); ++i) {
    Statement *stmt = block->statements.at(i);
    stmt->Emit(this);
  }
  symtable->Pop();

  delete block;
}

llvm::Value *BaseGenerator::EmitIdentifierPrimaryExpression(Expression *expr) {

  IdentifierPrimaryExpression *identifier_expr = (IdentifierPrimaryExpression *) expr;
  std::string identifier_name = identifier_expr->string_value;

  SymtableEntry *symbol = symtable->Get(identifier_name);


  if (symbol == nullptr) {
    //std::cerr << "use of undeclared identifier " << identifier_name << std::endl;
    return nullptr;
  }
  return symbol->llvm_ptr;
}

llvm::Value *BaseGenerator::EmitFunctionCallPostFixExpression(FunctionCallPostfixExpression *func_call_expr) {

  std::string identifier = func_call_expr->identifier->string_value;
  if (!identifier.compare("print")) {
    create_print_function(func_call_expr->arguments.at(0), false);
  } else if (!identifier.compare("println")) {
    create_print_function(func_call_expr->arguments.at(0), true);
  }
  delete func_call_expr;
  return nullptr;
}

void BaseGenerator::create_print_function(Expression *parameter_expr, bool new_line) {

  const std::string const_function_name = "printf";
  const std::string const_form_string_arg = "%s";
  const std::string const_form_int_arg = "%i";
  const std::string const_form_float_arg = "%f";
  const std::string const_new_line_string = "\n";

  llvm::Value *llvm_parameter_value = parameter_expr->Emit(this);

  if (llvm_parameter_value == nullptr) {
    INTERNAL_ERROR("error emitting parameter value");
    return;
  }

  //when it's a pointer type load the value
  if (llvm_parameter_value->getType()->isPointerTy()) {
    llvm_parameter_value = builder->CreateLoad(llvm_parameter_value);
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
        llvm::Function::Create(llvm_printf_type, llvm::Function::ExternalLinkage, const_function_name, module);
    llvm_print_func->setCallingConv(llvm::CallingConv::C);
    _functions[const_function_name] = llvm_print_func;
  }

  /**
   * Determine which kind of expression we have (numeric, string, bool, ...)
   */
  llvm::Type *llvm_parameter_type = llvm_parameter_value->getType();
  std::string printf_parameter_format;

  if (llvm_parameter_type->isFloatTy() || llvm_parameter_type->isDoubleTy()) {
    printf_parameter_format = const_form_float_arg;
  } else if (llvm_parameter_type->isIntegerTy(8)) {
    std::cout << "str" << std::endl;
    printf_parameter_format = const_form_string_arg;
  } else if (llvm_parameter_type->isIntegerTy()) {
    printf_parameter_format = const_form_int_arg;
  }

  if (new_line) {
    printf_parameter_format.append(const_new_line_string);
  }

  std::vector<llvm::Value *> param_values;
  param_values.push_back(builder->CreateGlobalStringPtr(printf_parameter_format));

  param_values.push_back(llvm_parameter_value);
  llvm::ArrayRef<llvm::Value *> llvm_func_arguments(param_values);

  /**
   * Finally create the function call call
   */
  builder->CreateCall(llvm_print_func, llvm_func_arguments);
}

void BaseGenerator::EmitVariableDeclarationStatement(VariableDeclarationStatement *var_decl_stmt) {

  std::string identifier = var_decl_stmt->identifier->string_value;

  llvm::Type *llvm_variable_type = nullptr;
  ElementType expression_type = var_decl_stmt->type->type;

  //set the llvm type
  if (expression_type == ElementType::BOOL) {
    llvm_variable_type = llvm::Type::getInt1Ty(llvm::getGlobalContext());
  } else if (expression_type == ElementType::FLOAT) {
    llvm_variable_type = llvm::Type::getFloatTy(llvm::getGlobalContext());
  } else if (expression_type == ElementType::DOUBLE) {
    llvm_variable_type = llvm::Type::getDoubleTy(llvm::getGlobalContext());
  } else if (expression_type == ElementType::INTEGER) {
    llvm_variable_type = llvm::Type::getInt32Ty(llvm::getGlobalContext());
  } else if (expression_type == ElementType::STRING) {
    llvm_variable_type = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
  }

  //allocate stack space
  llvm::AllocaInst *llvm_alloca_inst = new llvm::AllocaInst(llvm_variable_type, identifier, insert_point);

  //save the pointer of the value in the symbol table
  SymtableEntry *variable_declaration_entry = new SymtableEntry;

  variable_declaration_entry->llvm_ptr = llvm_alloca_inst;
  variable_declaration_entry->type = expression_type;
  variable_declaration_entry->identifier = identifier;

  symtable->current_scope->set(identifier, variable_declaration_entry);

  //if there's a assignment beside the variable declaration, emit the rhs assignment value
  AssignmentExpression *assignment_expr = var_decl_stmt->expression_to_assign;
  if (assignment_expr != nullptr) {
    assignment_expr->Emit(this);
  }
}

llvm::Value *BaseGenerator::EmitAssignmentExpression(AssignmentExpression *assignment_expr) {

  //just refer to DoAssignment in the base code_generator because we don't have phi_nodes here
  return DoAssignment(assignment_expr);
}

llvm::Value *BaseGenerator::DoAssignment(AssignmentExpression *assignment_expr) {

  std::string identifier = assignment_expr->identifier->string_value;

  //check if the type in assignment expression matches the allocated type
  SymtableEntry *symbol = symtable->Get(identifier);
  //when there's no value in symbol table print error
  if (symbol == nullptr) {
    ERR_PRINTLN("Use of undeclared identifier " << identifier);
    return nullptr;
  }

  ElementType declared_type = symbol->type;
  ElementType assigned_type = assignment_expr->expression_to_assign->type;

  llvm::Value *llvm_emitted_assignment_value = assignment_expr->expression_to_assign->Emit(this);

  if ((declared_type == ElementType::FLOAT || declared_type == ElementType::DOUBLE)
      && assigned_type == ElementType::INTEGER) {
    //cast from int to float/double
    llvm::ConstantInt *integer_value = (llvm::ConstantInt *) llvm_emitted_assignment_value;
    llvm_emitted_assignment_value = createLlvmFpValue(integer_value->getSExtValue(), declared_type);
  } else if (declared_type == ElementType::FLOAT && assigned_type == ElementType::DOUBLE) {
    WARN_PRINTLN("precision loss: conversion from double to float is currently not supported");
    return nullptr;
  } else if (declared_type == ElementType::DOUBLE && assigned_type == ElementType::FLOAT) {
    llvm::ConstantFP *fp_value = (llvm::ConstantFP *) llvm_emitted_assignment_value;
    double value = fp_value->getValueAPF().convertToFloat();
    llvm_emitted_assignment_value = createLlvmFpValue(value, ElementType::DOUBLE);
  }

  new llvm::StoreInst(llvm_emitted_assignment_value, symbol->llvm_ptr, false, insert_point);

  delete assignment_expr;
  return llvm_emitted_assignment_value;
}

llvm::Value *BaseGenerator::EmitLogicalOrExpression(LogicalOrExpression *expr) {

  llvm::Value *lhs_value = expr->lhs_expr->Emit(this);
  llvm::Value *rhs_value = expr->rhs_expr->Emit(this);

  return builder->CreateOr(lhs_value, rhs_value, "or");
}

llvm::Value *BaseGenerator::EmitLogicalAndExpression(LogicalAndExpression *expr) {

  llvm::Value *lhs_value = expr->lhs_expr->Emit(this);
  llvm::Value *rhs_value = expr->rhs_expr->Emit(this);

  return builder->CreateAnd(lhs_value, rhs_value, "and");
}

llvm::Value *BaseGenerator::EmitAdditiveExpression(AdditiveExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_expr->Emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_expr->Emit(this);

  llvm::Value *llvm_result_value = nullptr;

  bool
      floating_point = llvm_lhs_value->getType()->isFloatingPointTy() || llvm_rhs_value->getType()->isFloatingPointTy();

  if (expr->additive_operator == Operator::SUM) {
    if (floating_point) {
      llvm_result_value = builder->CreateFAdd(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = builder->CreateAdd(llvm_lhs_value, llvm_rhs_value);
    }
  } else if (expr->additive_operator == Operator::SUB) {
    if (floating_point) {
      llvm_result_value = builder->CreateFSub(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = builder->CreateSub(llvm_lhs_value, llvm_rhs_value);
    }
  } else {
    ERR_PRINTLN("undefined operator for additive operation");
  }

  delete expr;

  return llvm_result_value;
}

llvm::Value *BaseGenerator::EmitMultiplicativeExpression(MultiplicativeExpression *expr) {

  llvm::Value *llvm_lhs_value = expr->lhs_expr->Emit(this);
  llvm::Value *llvm_rhs_value = expr->rhs_expr->Emit(this);

  llvm::Value *llvm_result_value = nullptr;

  bool
      floating_point = llvm_lhs_value->getType()->isFloatingPointTy() || llvm_rhs_value->getType()->isFloatingPointTy();

  if (expr->multiplicative_operator == Operator::MUL) {
    if (floating_point) {
      llvm_result_value = builder->CreateFMul(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = builder->CreateMul(llvm_lhs_value, llvm_rhs_value);
    }
  } else if (expr->multiplicative_operator == Operator::DIV) {
    if (floating_point) {
      llvm_result_value = builder->CreateFMul(llvm_lhs_value, llvm_rhs_value);
    } else {
      llvm_result_value = builder->CreateMul(llvm_lhs_value, llvm_rhs_value);
    }
    llvm_result_value = builder->CreateExactSDiv(llvm_lhs_value, llvm_rhs_value);
  } else {
    ERR_PRINTLN("undefined operator for multiplicative operation");
  }

  delete expr;

  return llvm_result_value;
}

llvm::Value *BaseGenerator::EmitPrimaryExpression(PrimaryExpression *expr) {

  //determine the type of the declared expression
  ElementType expr_type = expr->type;

  if (expr_type == ElementType::BOOL) {

    delete expr;
    return CreateLlvmBoolValue(expr->bool_value);
  } else if (expr_type == ElementType::STRING) {

    delete expr;
    return builder->CreateGlobalStringPtr(expr->string_value);
  } else if (expr_type == ElementType::FLOAT || expr_type == ElementType::DOUBLE) {

    delete expr;
    return createLlvmFpValue(expr->double_value, expr_type);
  } else if (expr_type == ElementType::INTEGER) {

    delete expr;
    return CreateLlvmIntValue(expr->int_value, expr_type);
  }
  return nullptr;
}

llvm::Value *BaseGenerator::EmitEqualityExpression(EqualityExpression *expr) {

  Expression *lhs_expr = expr->lhs_expr;
  Expression *rhs_expr = expr->rhs_expr;

  llvm::Value *lhs_value = lhs_expr->Emit(this);
  llvm::Value *rhs_value = rhs_expr->Emit(this);

  llvm::Value *result_value = nullptr;

  Operator compare_operator = expr->compare_operator;
  if (compare_operator == Operator::COMPARE_EQUAL) {
    result_value = builder->CreateICmpEQ(lhs_value, rhs_value);

  } else if (compare_operator == Operator::COMPARE_GREATER) {
    result_value = builder->CreateICmpSGT(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_GREATER_OR_EQUAL) {
    result_value = builder->CreateICmpSGE(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_LESS) {
    result_value = builder->CreateICmpSLT(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_LESS_OR_EQUAL) {
    result_value = builder->CreateICmpSLE(rhs_value, lhs_value);

  } else if (compare_operator == Operator::COMPARE_UNEQUAL) {
    result_value = builder->CreateICmpNE(rhs_value, lhs_value);

  } else {
    ERR_PRINTLN("INVALID COMPARE OPERATOR");
  }
  return result_value;
}

llvm::Value *BaseGenerator::EmitUnaryExpression(UnaryExpression *expr) {

  PostFixExpression *lhs_expr = expr->postfix_expr;

  std::unique_ptr<llvm::Value> value(lhs_expr->Emit(this));

  Operator unary_operator = expr->unary_operator;
  if (unary_operator == Operator::INCREMENT) {
    return value.get();

  } else if (unary_operator == Operator::DECREMENT) {
    llvm::LoadInst *load_instr = builder->CreateLoad(value.get());
    llvm::Value *dec = builder->CreateSub(builder->getInt8(1), load_instr);
    llvm::StoreInst *store_instr = builder->CreateStore(dec, load_instr);
    return value.get();

  } else if (unary_operator == Operator::NEGATE) {
    //impl
  } else if (unary_operator == Operator::NEGATIVE) {
    //impl
  }
  return nullptr;
}

void BaseGenerator::construct() {

  // create main function
  llvm::FunctionType *main_function_type = llvm::FunctionType::get(builder->getVoidTy(), false);
  llvm::Function
      *main_function = llvm::Function::Create(main_function_type, llvm::Function::ExternalLinkage, "main", module);

  _functions["main"] = main_function;

  insert_point = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entrypoint", main_function);
  builder->SetInsertPoint(insert_point);
}

void BaseGenerator::finalize() {

  builder->CreateRetVoid();
}

void BaseGenerator::EmitExpressionStatement(ExpressionStatement *expr_stmt) {

  Expression *expression = expr_stmt->expression;
  expression->Emit(this);
}

std::string BaseGenerator::GetIR() {

  std::string str;
  llvm::raw_string_ostream rso(str);
  module->print(rso, nullptr);
  return str;
}

void BaseGenerator::EmitForStatement(ForStatement *for_statment) {

  llvm::BasicBlock *for_instructions =
      llvm::BasicBlock::Create(llvm::getGlobalContext(), "forinst", _functions["main"], insert_point);

}

void BaseGenerator::EmitIfStatement(IfStatement *if_statement) {

  // emit the condition and cast it to a floating point value
  llvm::Value *cond_val = if_statement->condition->Emit(this);
  cond_val = builder->CreateUIToFP(cond_val, llvm::Type::getDoubleTy(llvm::getGlobalContext()), "bool_convert");

  // create a floating point value for the comparison
  llvm::Value *zero_val = createLlvmFpValue(0.0, ElementType::DOUBLE);

  // convert condition to a boolean by comparing equal to 0.0
  cond_val = builder->CreateFCmpONE(cond_val, zero_val, "condition");

  // create the blocks for the condition branches
  llvm::Function *the_function = builder->GetInsertBlock()->getParent();

  llvm::BasicBlock *then_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then_block", the_function);
  llvm::BasicBlock *else_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else_block");
  llvm::BasicBlock *merge_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge_block");

  builder->CreateCondBr(cond_val, then_block, else_block);

  /**
   * then
   */

  PhiGenerator *phi_gen = new PhiGenerator(then_block, builder, module);
  phi_gen->SetInsertPoint(then_block);

//  phi_gen->setSymtable(block_stack); TODO check
  if_statement->statement->Emit(phi_gen);
  builder->CreateBr(merge_block);
  then_block = builder->GetInsertBlock();

  /**
   * else
   */

  the_function->getBasicBlockList().push_back(else_block);
  phi_gen->SetInsertPoint(else_block);
  if_statement->else_stmt->Emit(phi_gen);
  builder->CreateBr(merge_block);
  else_block = builder->GetInsertBlock();

  /**
   * merge
   */

  the_function->getBasicBlockList().push_back(merge_block);
  builder->SetInsertPoint(merge_block);

  // emit the phi nodes
  for (int i = 0; i < phi_gen->phi_entries_table.size(); ++i) {
    PhiEntry *entry = phi_gen->phi_entries_table.at(i);

    if (entry->phi_count < 2) continue;
    llvm::PHINode *phi_node = builder->CreatePHI(entry->first_value->getType(), entry->phi_count, "phival");

    phi_node->addIncoming(entry->first_value, entry->first_block);
    phi_node->addIncoming(entry->second_value, entry->second_block);
  }
  merge_block = builder->GetInsertBlock();
}

void BaseGenerator::SetInsertPoint(llvm::BasicBlock *insert) {

  insert_point = insert;
  builder->SetInsertPoint(insert);
}