//
// DebugManager.hpp
// Debugging tools
//
// Created by Joshua Brunhuber on 23.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOOLS_DEBUG_HPP
#define RUNWAY_TOOLS_DEBUG_HPP

#include <iostream>
#include <tools/ModuleInfo.hpp>

class DebugManager {
 public:
  static bool debug;
  static void printMessage(std::string info);
  static void printMessage(std::string info, ModuleInfo runway_module);
};

#endif /* defined (RUNWAY_TOOLS_DEBUG_HPP) */
