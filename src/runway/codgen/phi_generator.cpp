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

  std::cout << " phi gen " << std::endl;

  //create a new phi entry for the symbol
  phi_entry *phi_e = phi_entries_table[identifier];
  phi_e->phi_table[_insert_point] = assignment;

  //write the phi entry in the phi table
  phi_entries_table[identifier] = phi_e;

  return assignment;
};