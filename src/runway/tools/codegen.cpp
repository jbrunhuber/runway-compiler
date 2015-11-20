//
// codegen.cpp
// codegen help functions
//
// Created by Joshua Brunhuber on 20.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "tools/codegen.h"

llvm::Value *createCompareResult(llvm::Value *lhs_bool_val, llvm::Value *rhs_bool_val, Operator cmp_operator) {

  if (isBoolType(lhs_bool_val) && isBoolType(rhs_bool_val)) {

    llvm::ConstantInt *llvm_lhs_bool = (llvm::ConstantInt *) lhs_bool_val;
    llvm::ConstantInt *llvm_rhs_bool = (llvm::ConstantInt *) rhs_bool_val;

    bool lhs = llvm_lhs_bool->getSExtValue();
    bool rhs = llvm_rhs_bool->getSExtValue();

    bool cond;
    if (cmp_operator == Operator::LOGICAL_AND) {
      cond = lhs && rhs;
    } else if (cmp_operator == Operator::LOGICAL_OR) {
      cond = lhs || rhs;
    } else {
      ERR_PRINTLN("invalid compare operator");
      return nullptr;
    }

    if (cond) {
      return createLlvmIntValue(true, ExpressionType::BOOL);
    } else {
      return createLlvmIntValue(false, ExpressionType::BOOL);
    }
  }
  return nullptr;
}