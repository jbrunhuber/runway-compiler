//
// phi_generator.cpp
// Code generation implementation
//
// Created by Joshua Brunhuber on 21.12.2015
// phi_generator.hpp
// code generation visitor
//
// Created by Joshua Brunhuber on 22.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <codegen/phi_generator.hpp>
#include <parser/Nodes.hpp>

llvm::Value *phi_generator::emitAssignmentExpression(AssignmentExpression *assignment_expr) {

  llvm::Value *assignment = doAssignment(assignment_expr);;

  std::string identifier = assignment_expr->identifier->string_value;

  phi_entry *phi_e = phi_entries_table[identifier];

  phi_e->phi_table[_insert_point] = assignment;


  return assignment;
};