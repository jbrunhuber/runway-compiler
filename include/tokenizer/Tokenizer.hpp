//
// tokenizer.hpp
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

#include <tokenizer/keywords.hpp>
#include <tokenizer/token.hpp>

class Tokenizer {

 public:
  Tokenizer(std::string &source);
  void Next(Token& token);

 private:
  std::string _source;
  std::string _punctuator = "+-*/=.<>(){};:[]!|&";
  char _current;
  char _lookahead;
  uint32_t _position;
  uint32_t _line_count;
  size_t _length;

  std::string ReadTextualLiteral();
  std::string ReadIdentifier();
  std::string ReadPunctuator();
  double ReadNumeric();
  bool TokenizeNumericLiteral(Token &token);
  bool TokenizeStringLiteral(Token &token);

  void step();
  void skip();
  bool validate_identifier_cf(char c);
  bool validate_identifier_c(char c);
  bool validate_punctuator(char c);
};

#endif /* defined(RUNWAY_TOKENIZER_TOKENIZER_HPP) */
