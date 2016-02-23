#include <codegen/scope_block.hpp>
#include <iostream>
#include <logger.h>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  values[identifier] = symbol;
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  SymtableEntry *entry = nullptr;
  std::map<std::string, SymtableEntry *>::iterator iterator = values.find(identifier);
  if (iterator == values.end()) { // not found
    if (parent != nullptr) {
      entry = parent->get(identifier); // not found, look in the parent scope if the symbol exists
    } else { // current is root, use of undeclared identifier
      ERR_PRINTLN("use of undeclared identifier " << identifier);
    }
  } else { //found
    entry = iterator->second;
    if (entry == nullptr) {
      INTERNAL_ERROR("Symtable entry is null");
    }
  }
  return entry;
}
