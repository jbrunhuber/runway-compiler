//
// scope_block.cpp
// Contains symbols for a block
//
// Created by Joshua Brunhuber on 20.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <codegen/scope_block.hpp>

void scope_block::set(std::string identifier, rw_symtable_entry *symbol) {

  _symtable[identifier] = symbol;
}

rw_symtable_entry *scope_block::get(std::string identifier) {

  return _symtable[identifier];
}
