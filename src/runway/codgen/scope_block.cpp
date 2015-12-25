//
// scope_block.cpp
// Contains symbols for a block
//
// Created by Joshua Brunhuber on 20.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <codegen/scope_block.hpp>

void ScopeBlock::set(std::string identifier, SymtableEntry *symbol) {

  _symtable[identifier] = symbol;
}

SymtableEntry *ScopeBlock::get(std::string identifier) {

  return _symtable[identifier];
}
