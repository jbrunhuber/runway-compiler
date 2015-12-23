//
// Created by Joshua Brunhuber on 21.12.2015
//
// phi_generator.cpp
// code generation visitor with phi node functionality
//

#include <codegen/phi_generator.hpp>

llvm::Value *phi_generator::emitAssignmentExpression(AssignmentExpression *assignment_expr) {

  //emit the assignment instructions for the IR
  llvm::Value *assignment = doAssignment(assignment_expr);

  //determine symbols name as identifier for the phi table
  std::string identifier = assignment_expr->identifier->string_value;

  //create a new phi entry for the symbol
  phi_entry *phi_e = phi_entries_table[identifier];
  phi_e->phi_table[_insert_point] = assignment;

  //write the phi entry in the phi table
  phi_entries_table[identifier] = phi_e;

  return assignment;
};