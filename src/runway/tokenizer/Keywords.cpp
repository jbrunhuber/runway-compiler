//
// Keywords.cpp
// Implementation of the keywords
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include "tokenizer/Keywords.hpp"

/**
 *
 */
std::vector<std::string> KEYWORDS = { "import", "return", "final", "class",
    "enum", "for", "while", "continue", "break", "switch", "if", "else",
    "switch", "case", "default", "this", "super", "static", "private", "public",
    "protected", "void", "int8", "int16", "int32", "int64", "char", "bool" };

/**
 * Checks if a keywords is contained in the list
 */
bool containKeyword(std::string keyword) {

  if (std::find(KEYWORDS.begin(), KEYWORDS.end(), keyword)
      != KEYWORDS.end()) {
    return true;
  }
  return false;
}
