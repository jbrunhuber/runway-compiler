#include <codegen/scope_block.hpp>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  values[identifier] = symbol;
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  SymtableEntry *entry = nullptr;
  if (values.find(identifier) != values.end()) {
    entry = parent->get(identifier); //not found, look in the parent scope if a symbol exists
  } else if (parent != nullptr) {
    entry = values[identifier]; //found
  }
  return entry;
}