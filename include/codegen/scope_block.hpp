//
// scope_block.hpp
// Contains symbols for a block
//
// Created by Joshua Brunhuber on 20.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_SCOPEBLOCK_HPP
#define RUNWAY_CODEGEN_SCOPEBLOCK_HPP

#include <codegen/symtable_entry.hpp>

class scope_block {
 private:
  std::map<std::string, rw_symtable_entry *> _symtable;

 public:

  //adds a new symbol to the symbol table
  void add(std::string, rw_symtable_entry *);

  //access to a symbol in the current scope
  rw_symtable_entry *get(std::string);
};

#endif /* RUNWAY_CODEGEN_SCOPEBLOCK_HPP */
