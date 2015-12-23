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
  std::string symbol_identifier;
  std::map<llvm::BasicBlock *, llvm::Value *> phi_table;
};

#endif /* INCLUDE_CODEGEN_PHI_ENTRY_HPP_ */
