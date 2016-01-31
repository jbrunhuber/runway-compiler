//
// Created by Joshua Brunhuber on 31.01.16.
//

#ifndef RUNWAY_SYMTABLE_HPP
#define RUNWAY_SYMTABLE_HPP

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

#endif //RUNWAY_SYMTABLE_HPP