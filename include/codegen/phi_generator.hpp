//
// phi_generator.hpp
// Code generation implementation
//
// Created by Joshua Brunhuber on 21.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_PHI_GENERATOR_HPP
#define RUNWAY_CODEGEN_PHI_GENERATOR_HPP

#include "base_generator.hpp"
#include "phi_entry.hpp"

class phi_generator : public base_generator {

 public:

  phi_generator(llvm::BasicBlock *, llvm::IRBuilder<> *, llvm::Module *);
  std::vector<phi_entry *> phi_entries_table;
  llvm::Value *emitAssignmentExpression(AssignmentExpression *);
  phi_entry *get(std::string);
  void setSymtable(std::stack<scope_block *> *stack);
};

#endif /* defined (RUNWAY_CODEGEN_PHI_GENERATOR_HPP) */