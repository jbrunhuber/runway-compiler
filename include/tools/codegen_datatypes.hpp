//
// codegen_datatypes.hpp
// helper functions for datatypes (codegen)
//
// Created by Joshua Brunhuber on 20.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOOLS_DATATYPES_HPP
#define RUNWAY_TOOLS_DATATYPES_HPP

#include <iostream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>

#include <parser/element_type.hpp>

bool isBoolType(llvm::Value *);

llvm::Value *CreateLlvmIntValue(int64_t, ElementType);

llvm::Value *createLlvmFpValue(double, ElementType);

llvm::Value *CreateLlvmBoolValue(bool);

#endif  /* defined(RUNWAY_TOOLS_DATATYPES_HPP) */