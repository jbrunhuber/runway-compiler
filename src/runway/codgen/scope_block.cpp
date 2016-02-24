#include <codegen/scope_block.hpp>
#include <iostream>
#include <logger.h>

void ScopeBlock::Set(std::string identifier, SymtableEntry *symbol) {

  values[identifier] = symbol;
}

SymtableEntry *ScopeBlock::Get(std::string identifier) {

  SymtableEntry *symtable_entry = nullptr;

  std::map<std::string, SymtableEntry *>::iterator scope_blocks_iterator = values.find(identifier);
  bool identifier_found = scope_blocks_iterator != values.end();

  if (identifier_found) {
    symtable_entry = scope_blocks_iterator->second;
    if (symtable_entry == nullptr) {
      INTERNAL_ERROR("Symbol table entry (" << identifier << ") is null");
    }
  } else {
    bool root_scope = (parent == nullptr);
    if (root_scope) {
      ERR_PRINTLN("Use of undeclared identifier " << identifier);
    } else {
      symtable_entry = parent->Get(identifier);
    }
  }
  return symtable_entry;
}
