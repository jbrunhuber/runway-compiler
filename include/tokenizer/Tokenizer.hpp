//
// Tokenizer.hpp
// Tokenizes tokens
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOKENIZER_TOKENIZER_HPP
#define RUNWAY_TOKENIZER_TOKENIZER_HPP

#include <string>
#include <cctype> //isspace
#include <sstream>

#include "Token.hpp"
#include "Keywords.hpp"

class Tokenizer {

 public:
  Tokenizer(std::string &source);
  void next(Token& token);

 private:
  std::string _source;
  std::string _punctuator = "+-*/=.<>(){};:[]!|&";
  char _current;
  char _lookahead;
  uint32_t _position;
  uint32_t _line_count;
  size_t _length;

  void step();
  void skipWhitespace();
  bool isValidIdentifierFirst(char c);
  bool isValidIdentifier(char c);
  bool isPunctuator(char c);

  std::string readTextualLiteral();
  std::string readIdentifier();
  std::string readPunctuator();

  double readNumeric();

  bool tokenizeNumericLiteral(Token &token);
  bool tokenizeStringLiteral(Token &token);
};

#endif /* defined(RUNWAY_TOKENIZER_TOKENIZER_HPP) */
