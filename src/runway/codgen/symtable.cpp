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

  ScopeBlock *block = nullptr;
  SymtableEntry *entry = nullptr;
  for (unsigned i = 0; i < blocks.size(); ++i) {
    entry = block->get(identifier);
    if (entry != nullptr) return entry;
  }
  return entry;
}

ScopeBlock *Symtable::GetCurrentScope() {

  return blocks.front();
}