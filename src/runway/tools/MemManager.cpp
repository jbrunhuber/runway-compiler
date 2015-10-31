//
// MemManager.cpp
//
// Created by Joshua Brunhuber on 31.10.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <vector>

#include <llvm/IR/Value.h>

#include "../../../include/tools/MemManager.hpp"

void MemManager::registerPtr(llvm::Value *v) {
  
  _elements.push_back(v);
}

void MemManager::deleteAll() {
  
  for (int i = 0; i < _elements.size(); ++i) {
    delete _elements[i];
  }
  _elements.clear();
}