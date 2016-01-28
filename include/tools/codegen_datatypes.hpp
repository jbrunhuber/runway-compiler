//
// codegen_datatypes.hpp
// helper functions for datatypes (codegen)
//
// Created by Joshua Brunhuber on 20.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOOLS_DATATYPES_HPP
#define RUNWAY_TOOLS_DATATYPES_HPP

#include <parser/nodes.hpp>

enum class ExpressionType;

bool isBoolType(llvm::Value *);

llvm::Value *CreateLlvmIntValue(int64_t, ExpressionType);

llvm::Value *createLlvmFpValue(double, ExpressionType);

llvm::Value *CreateLlvmBoolValue(bool);

#endif  /* defined(RUNWAY_TOOLS_DATATYPES_HPP) */
