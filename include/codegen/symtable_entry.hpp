//
// rw_symtable_entry.hpp
//
// Created by Joshua Brunhuber on 04.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef INCLUDE_CODEGEN_RW_SYMTABLE_ENTRY_HPP_
#define INCLUDE_CODEGEN_RW_SYMTABLE_ENTRY_HPP_

#include <llvm/IR/Value.h>

#include <parser/Nodes.hpp>

enum class ExpressionType;

/**
 * Manages symbol table entries and stores information to it
 */
class rw_symtable_entry {

 public:
  ExpressionType type;    //the stored type
  llvm::Value *llvm_ptr;  //the llvm pointer to the value
  ~rw_symtable_entry();   //destructor
};

#endif /* defined(CODEGEN_RW_SYMTABLE_ENTRY_HPP) */
