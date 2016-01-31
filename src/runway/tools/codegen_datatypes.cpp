//
// datatypes.cpp
// datatypes functionality
//
// Created by Joshua Brunhuber on 20.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <tools/codegen_datatypes.hpp>

/**
 * checks if a llvm value is a bool type
 */
bool isBoolType(llvm::Value *val) {

  if (val == nullptr) {
    return false;
  }
  return (val->getType() == llvm::Type::getInt1Ty(llvm::getGlobalContext()));
}

/**
 * Creates a llvm value from a integer value (just i32 supported yet)
 *
 * param integer_value: the integer value to create
 * param type: the integer type to specify the size (short, int, long,)
 */
llvm::Value *CreateLlvmIntValue(int64_t integer_value, ElementType type) {

  uint16_t integer_size = 0;
  llvm::Type *llvm_type = nullptr;
  if (type == ElementType::INTEGER) {
    integer_size = 32;
    llvm_type = llvm::Type::getInt32Ty(llvm::getGlobalContext());
  } else if (type == ElementType::BOOL) {
    integer_size = 1;
    llvm_type = llvm::Type::getInt1Ty(llvm::getGlobalContext());
  } else {
    std::cerr << "invalid type" << std::endl;
    return nullptr;
  }
  return llvm::ConstantInt::getIntegerValue(llvm_type, llvm::APInt(integer_size, integer_value));
}

/**
 * Creates a llvm value from a floating point value and the type (float/double)
 *
 * param fp_value: the floating point value to create
 * param type: the type of the floating point precision (float/double)
 *
 * return: created llvm value
 */
llvm::Value *createLlvmFpValue(double fp_value, ElementType type) {

  bool double_precision;
  if (type == ElementType::FLOAT) {
    double_precision = false;
  } else if (type == ElementType::DOUBLE) {
    double_precision = true;
  } else {
    std::cerr << "invalid type" << std::endl;
    return nullptr;
  }

  if (double_precision) {
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()), fp_value);
  } else {
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(llvm::getGlobalContext()), fp_value);
  }
}

llvm::Value *CreateLlvmBoolValue(bool b) {

  uint8_t integer_value = 0;
  if (b) {
    integer_value = 1;
  }
  return llvm::ConstantInt::getIntegerValue(llvm::Type::getInt1Ty(llvm::getGlobalContext()),
                                            llvm::APInt(1, integer_value));
}
