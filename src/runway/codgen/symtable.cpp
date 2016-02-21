//
// Created by Joshua Brunhuber on 31.01.16.
//

#include <codegen/symtable.hpp>
#include <logger.h>
#include <iostream>

void Symtable::Push(ScopeBlock *scope_block) {

  scope_block->parent = current_scope;
  current_scope = scope_block;
  blocks.push_front(scope_block);
}

void Symtable::Pop() {

  blocks.pop_front();
}

SymtableEntry *Symtable::Get(std::string identifier) {

  return current_scope->get(identifier);
}