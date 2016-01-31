#include <codegen/scope_block.hpp>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  symbol->identifier = identifier;
  symtable.push_back(symbol);
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  SymtableEntry *entry = nullptr;
  for (unsigned i = 0; i < symtable.size(); ++i) {
    entry = symtable.at(i);

    if (entry == nullptr) continue;
    if (entry->identifier == identifier) return entry;
  }
  return entry;
}

ScopeBlock *ScopeBlock::buildEmptyBlock() {

  ScopeBlock *entry = new ScopeBlock;
  entry->identifier = "block";
  entry->type = ExpressionType::BLOCK;
  return entry;
}

ScopeBlock::ScopeBlock() { }