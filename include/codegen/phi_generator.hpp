//
// phi_generator.hpp
// Code generation implementation
//
// Created by Joshua Brunhuber on 21.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_PHI_GENERATOR_HPP
#define RUNWAY_CODEGEN_PHI_GENERATOR_HPP

#include <codegen/base_generator.hpp>
#include <codegen/phi_entry.hpp>

class PhiGenerator: public BaseGenerator {

 public:

  PhiGenerator(llvm::BasicBlock *, llvm::IRBuilder<> *, llvm::Module *);
  std::vector<PhiEntry *> phi_entries_table;
  llvm::Value *EmitAssignmentExpression(AssignmentExpression *) override;
  PhiEntry *get(std::string);
  void setSymtable(std::stack<ScopeBlock *> *stack);
};

#endif /* defined (RUNWAY_CODEGEN_PHI_GENERATOR_HPP) */