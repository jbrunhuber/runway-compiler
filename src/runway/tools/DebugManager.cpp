//
// DebugManager.cpp
// Debugging tools
//
// Created by Joshua Brunhuber on 23.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "tools/DebugManager.hpp"

bool DebugManager::debug = false;

/**
 * Prints debug message if debugging is enabled
 */
void DebugManager::printMessage(std::string info) {

  if(debug) {
    std::cout << "INFO: \t" << info << std::endl;
  }
}

/**
 * Prints debug message with module info if debugging is enabled
 */
void DebugManager::printMessage(std::string info, ModuleInfo runway_module) {

  if(debug) {
    std::string runway_module_info;
    if(runway_module == ModuleInfo::CODEGEN) {
      runway_module_info = "codegen";
    } else if(runway_module == ModuleInfo::PARSER) {
      runway_module_info = "parser";
    } else if(runway_module == ModuleInfo::TOKENIZER) {
      runway_module_info = "tokenizer";
    }
    std::cout << "[" << runway_module_info << "]\t" << info << std::endl;
  }
}
