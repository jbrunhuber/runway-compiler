#include <codegen/scope_block.hpp>
#include <iostream>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  values[identifier] = symbol;
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  SymtableEntry *entry = nullptr;
  std::map<std::string, SymtableEntry *>::iterator iterator = values.find(identifier);
  if (iterator == values.end()) {
    if (parent != nullptr) {
      return parent->get(identifier); //not found, look in the parent scope if a symbol exists
    } else {
      // current is root, use of undeclared identifier
    }
  } else {
    entry = iterator->second; //found but entry is null
  }
  return entry;
}
