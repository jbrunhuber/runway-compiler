#include <codegen/scope_block.hpp>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  _symtable[identifier] = symbol;
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  return _symtable[identifier];
}