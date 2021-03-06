//
// token.hpp
// simple token for the runway language
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOKENIZER_TOKEN_HPP
#define RUNWAY_TOKENIZER_TOKEN_HPP

#include <string>
#include <iostream>

#include <tokenizer/token_type.hpp>

class Token {
 public:

  std::string textual_content;
  double numeric_content;
  bool bool_content;
  TokenType token_type;
  uint32_t line_count;
};

#endif /* defined(RUNWAY_TOKENIZER_TOKEN_HPP) */
