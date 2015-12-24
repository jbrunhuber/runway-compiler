/*
 * phi_entry.hpp
 *
 *  Created on: 21.12.2015
 *      Author: josh
 */

#ifndef INCLUDE_CODEGEN_PHI_ENTRY_HPP_
#define INCLUDE_CODEGEN_PHI_ENTRY_HPP_

#include <string>
#include <map>

#include <llvm/IR/Value.h>

class phi_entry {

 public:
  //first
  llvm::BasicBlock *first_block;
  llvm::Value *first_value;

  //second
  llvm::BasicBlock *second_block;
  llvm::Value *second_value;

  std::string identifier;
};

#endif /* INCLUDE_CODEGEN_PHI_ENTRY_HPP_ */
