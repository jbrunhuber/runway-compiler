//
// rw_symtable_entry.cpp
// code generation visitor
//
// Created by Joshua Brunhuber on 04.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <codegen/symtable_entry.hpp>

/**
 * Release memory for llvm_ptr
 */
rw_symtable_entry::~rw_symtable_entry() {

  delete llvm_ptr;
}
