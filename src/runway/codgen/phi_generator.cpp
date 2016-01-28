#include <codegen/phi_generator.hpp>

PhiGenerator::PhiGenerator(llvm::BasicBlock *insert_block, llvm::IRBuilder<> *builder, llvm::Module *module) {

  this->insert_point = insert_block;
  this->builder = builder;
  this->module = module;
}

llvm::Value *PhiGenerator::EmitAssignmentExpression(AssignmentExpression *assignment_expr) {

  //emit the assignment instructions for the IR
  llvm::Value *assignment = DoAssignment(assignment_expr);

  //determine symbols name as identifier for the phi table
  std::string identifier = assignment_expr->identifier->string_value;

  //create a new phi entry for the symbol
  PhiEntry *phi_e = get(identifier);
  if (phi_e == nullptr) {
    phi_e = new PhiEntry;
    phi_e->first_value = assignment;
    phi_e->first_block = insert_point;
    phi_e->phi_count = 1;
  } else {
    phi_e->second_value = assignment;
    phi_e->second_block = insert_point;
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

  block_stack = stack;
}
