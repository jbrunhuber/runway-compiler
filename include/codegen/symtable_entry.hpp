//
// rw_symtable_entry.hpp
//
// Created by Joshua Brunhuber on 04.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_SYMTABLEENTRY_HPP
#define RUNWAY_CODEGEN_SYMTABLEENTRY_HPP

#include <llvm/IR/Value.h>

#include <parser/Nodes.hpp>

enum class ExpressionType;

/**
 * Manages symbol table entries and stores information to it
 */
class SymtableEntry {

 public:
  ExpressionType type;    //the stored type
  llvm::Value *llvm_ptr;  //the llvm pointer to the value
  ~SymtableEntry();   //destructor
};

#endif /* defined(RUNWAY_CODEGEN_SYMTABLEENTRY_HPP) */
