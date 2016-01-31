//
// Created by Joshua Brunhuber on 31.01.16.
//

#ifndef RUNWAY_SYMTABLE_HPP
#define RUNWAY_SYMTABLE_HPP

#include "scope_block.hpp"

class Symtable {
 public:
  std::vector<ScopeBlock *> blocks;
};

#endif //RUNWAY_SYMTABLE_HPP
