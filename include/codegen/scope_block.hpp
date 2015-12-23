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

class rw_symtable_entry;

class scope_block {

 private:
  std::map<std::string, rw_symtable_entry *> _symtable;
  std::map<std::string, rw_symtable_entry *> _phi_values;

 public:

  llvm::BasicBlock *block;

  /*
   * Adds a new symboltable entry to the current scope if it doesn't exists,
   * otherwise it overrides an existing symtable entry
   *
   * @param the symbol identifier
   * @param the symboltable entry
   */
  void set(std::string, rw_symtable_entry *);

  /*
   * Returns a symboltable entry
   *
   * @param		symbol identifier
   * @return  rw_symtable_entry
   */
  rw_symtable_entry *get(std::string);

  /*
   * Adds a new phi entry to the current scope if it doesn't exists,
   * otherwise it overrides an existing phi entry
   *
   * @param   the symbol identifier
   * @param   the symboltable entry
   */
  void setPhi(std::string, rw_symtable_entry *);

  /*
   * Returns a phi entry
   *
   * @param   phi symbol identifier
   * @return  rw_symtable_entry
   */
  rw_symtable_entry *getPhi(std::string);

  /*
   * Returns a map with all registered phi values
   *
   * @return  phi map
   */
  std::map<std::string, rw_symtable_entry *> getPhiMap();
};

#endif /* defined(RUNWAY_CODEGEN_SCOPEBLOCK_HPP) */
