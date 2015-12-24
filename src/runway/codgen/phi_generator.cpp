//
// Created by Joshua Brunhuber on 21.12.2015
//
// phi_generator.cpp
// code generation visitor with phi node functionality
//

#include <codegen/phi_generator.hpp>

phi_generator::phi_generator(llvm::BasicBlock *insert_block, llvm::IRBuilder<> *builder, llvm::Module *module) {

  this->_insert_point = insert_block;
  this->_builder = builder;
  this->_module = module;
}

llvm::Value *phi_generator::emitAssignmentExpression(AssignmentExpression *assignment_expr) {

  //emit the assignment instructions for the IR
  llvm::Value *assignment = doAssignment(assignment_expr);

  //determine symbols name as identifier for the phi table
  std::string identifier = assignment_expr->identifier->string_value;

  //create a new phi entry for the symbol
  phi_entry *phi_e = get(identifier);
  if (phi_e == nullptr) {
    phi_e = new phi_entry;
    phi_e->first_value = assignment;
    phi_e->first_block = _insert_point;
  } else {
    phi_e->second_value = assignment;
    phi_e->second_block = _insert_point;
  }

  phi_e->identifier = identifier;

  //write the phi entry in the phi table
  phi_entries_table.push_back(phi_e);

  return assignment;
}

phi_entry *phi_generator::get(std::string identifier) {

  for (int i = 0; i < phi_entries_table.size(); ++i) {
    phi_entry *entry = phi_entries_table.at(i);
    if (entry->identifier.compare(identifier) == 0) {
      return entry;
    }
  }
  return nullptr;
}


void phi_generator::setSymtable(std::stack<scope_block *> *stack) {

  _block_stack = stack;
}