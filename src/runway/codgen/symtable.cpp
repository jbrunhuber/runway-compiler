//
// Created by Joshua Brunhuber on 31.01.16.
//

#include <codegen/symtable.hpp>

void Symtable::Push(ScopeBlock *scope_block) {

  blocks.push_front(scope_block);
}

void Symtable::Pop() {

  blocks.pop_front();
}

SymtableEntry *Symtable::Get(std::string identifier) {

  return nullptr;
}

ScopeBlock *Symtable::GetCurrentScope() {

  return nullptr;
}