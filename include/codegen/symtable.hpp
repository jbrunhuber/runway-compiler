//
// symtable_entry.hpp
//
// Created by Joshua Brunhuber on 30.01.2016
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_SYMTABLE_HPP
#define RUNWAY_CODEGEN_SYMTABLE_HPP

#include <deque>
#include <string>

#include "scope_block.hpp"
#include "symtable_entry.hpp"

class SymtableEntry;

class Symtable {
 public:
  void Push(ScopeBlock *);
  void Pop();
  SymtableEntry *Get(std::string identifier);
  ScopeBlock *GetCurrentScope();

 private:
  std::deque<ScopeBlock *> blocks;
};

#endif /* defined (RUNWAY_CODEGEN_SYMTABLE_HPP) */