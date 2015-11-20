//
// codegen.h
// codegen help functions
//
// Created by Joshua Brunhuber on 20.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_H
#define RUNWAY_CODEGEN_H

#include <tools/codegen_datatypes.hpp>

/**
 * Create logical compare result (OR/AND) value
 *
 * param: lhs compare value
 * param: rhs compare value
 * param: compare operator (OR/AND)
 */
llvm::Value *createCompareResult(llvm::Value *, llvm::Value *, Operator);

#endif  /* defined(RUNWAY_CODEGEN_H) */