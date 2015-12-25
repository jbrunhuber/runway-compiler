//
// phi_generator.cpp
// A code generator which counts the phi values in each block
//
// Created by Joshua Brunhuber on 21.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <codegen/phi_generator.hpp>

PhiGenerator::PhiGenerator(llvm::BasicBlock *insert_block, llvm::IRBuilder<> *builder, llvm::Module *module) {

  this->_insert_point = insert_block;
  this->_builder = builder;
  this->_module = module;
}

llvm::Value *PhiGenerator::emitAssignmentExpression(AssignmentExpression *assignment_expr) {

  //emit the assignment instructions for the IR
  llvm::Value *assignment = doAssignment(assignment_expr);

  //determine symbols name as identifier for the phi table
  std::string identifier = assignment_expr->identifier->string_value;

  //create a new phi entry for the symbol
  PhiEntry *phi_e = get(identifier);
  if (phi_e == nullptr) {
    phi_e = new PhiEntry;
    phi_e->first_value = assignment;
    phi_e->first_block = _insert_point;
    phi_e->phi_count = 1;
  } else {
    phi_e->second_value = assignment;
    phi_e->second_block = _insert_point;
    phi_e->phi_count = 2;
  }

  phi_e->identifier = identifier;

  //write the phi entry in the phi table
  phi_entries_table.push_back(phi_e);

  return assignment;
}

PhiEntry *PhiGenerator::get(std::string identifier) {

  for (int i = 0; i < phi_entries_table.size(); ++i) {
    PhiEntry *entry = phi_entries_table.at(i);
    if (entry->identifier.compare(identifier) == 0) {
      return entry;
    }
  }
  return nullptr;
}


void PhiGenerator::setSymtable(std::stack<ScopeBlock *> *stack) {

  _block_stack = stack;
}