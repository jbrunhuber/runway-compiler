//
// scope_block.hpp
// Contains symbols for a block
//
// Created by Joshua Brunhuber on 20.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_SCOPEBLOCK_HPP
#define RUNWAY_CODEGEN_SCOPEBLOCK_HPP

#include <map>
#include <vector>

#include "symtable_entry.hpp"

class SymtableEntry;

class ScopeBlock: public SymtableEntry {

 private:
  std::vector<SymtableEntry *> symtable;
  std::map<std::string, SymtableEntry *> _phi_values;
  ScopeBlock();

 public:

  llvm::BasicBlock *block;

  /*
   * Adds a new symboltable entry to the current scope if it doesn't exists,
   * otherwise it overrides an existing symtable entry
   *
   * @param the symbol identifier
   * @param the symboltable entry
   */
  void set(std::string, SymtableEntry *);

  /*
   * Returns a symboltable entry
   *
   * @param		symbol identifier
   * @return  rw_symtable_entry
   */
  SymtableEntry *get(std::string);

  /*
   * Adds a new phi entry to the current scope if it doesn't exists,
   * otherwise it overrides an existing phi entry
   *
   * @param   the symbol identifier
   * @param   the symboltable entry
   */
  void setPhi(std::string, SymtableEntry *);

  /*
   * Returns a phi entry
   *
   * @param   phi symbol identifier
   * @return  rw_symtable_entry
   */
  SymtableEntry *getPhi(std::string);

  /*
   * Returns a map with all registered phi values
   *
   * @return  phi map
   */
  std::map<std::string, SymtableEntry *> getPhiMap();

  /**
   * Sets the attributes to identify this object as a 'ScopeBlock'
   */
  static ScopeBlock *buildEmptyBlock();

};

#endif /* defined(RUNWAY_CODEGEN_SCOPEBLOCK_HPP) */
