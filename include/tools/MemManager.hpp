//
// MemManager.cpp
//
// Created by Joshua Brunhuber on 31.10.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//
class MemManager {
  
public:
  
  void registerPtr(llvm::Value *v);
  
  void deleteAll();
  
private:
  std::vector<llvm::Value*> _elements;
};