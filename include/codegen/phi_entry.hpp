//
// phi_entry.hpp
// Wrapper to store the phi value and its containing block
//
// Created by Joshua Brunhuber on 21.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_PHIENTRY_HPP
#define RUNWAY_CODEGEN_PHIENTRY_HPP

#include <string>
#include <map>

#include <llvm/IR/Value.h>

class PhiEntry {

 public:
  //first
  llvm::BasicBlock *first_block;
  llvm::Value *first_value;

  //second
  llvm::BasicBlock *second_block;
  llvm::Value *second_value;

  std::string identifier;

  int phi_count = 0;
};

#endif /* defined (RUNWAY_CODEGEN_PHIENTRY_HPP) */