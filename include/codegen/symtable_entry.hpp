//
// symtable_entry.hpp
//
// Created by Joshua Brunhuber on 04.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_SYMTABLEENTRY_HPP
#define RUNWAY_CODEGEN_SYMTABLEENTRY_HPP

#include <string>

#include <llvm/IR/Value.h>

#include <parser/element_type.hpp>

enum class ElementType;

/**
 * Manages symbol table entries and stores information to it
 */
class SymtableEntry {

 public:
  ElementType type;    //the stored type
  llvm::Value *llvm_ptr;  //the llvm pointer to the value
  std::string identifier;
  ~SymtableEntry();       //destructor
};

#endif /* defined(RUNWAY_CODEGEN_SYMTABLEENTRY_HPP) */
