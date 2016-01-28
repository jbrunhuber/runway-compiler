//
// tokenizer.cpp
// Implementation of the tokenizer
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <iostream>

#include <tokenizer/tokenizer.hpp>

/**
 * Initializes member attributes
 */
Tokenizer::Tokenizer(std::string &source)
    : _position(0),
      _line_count(1),
      _length(source.length()) {

  this->_source = source;

  //musst (!) step twice because we'll need to init lookahead token
  step();
  step();
}

/**
 * Returns the next token
 */
void Tokenizer::Next(Token &token) {

  token.textual_content = "";

  skip();

  token.line_count = _line_count;

  //end of file
  if (_current == -1) {
    token.token_type = TokenType::T_EOF;
    return;
  }

  //punctuators
  if (validate_punctuator(_current)) {
    std::string textual_content = ReadPunctuator();
    token.token_type = TokenType::PUNCTUATOR;
    token.textual_content = textual_content;
    return;
  }

  //numeric literal
  if (TokenizeNumericLiteral(token)) {
    return;
  }

  //string literal
  if (TokenizeStringLiteral(token)) {
    return;
  }

  /**
   * If the first letter of the next token matches an identifier or a valid literal for booleans, keywords and references
   */
  if (validate_identifier_cf(_current)) {
    std::string read = ReadIdentifier();

    //std::cout << "found identifier:" << read << std::endl;

    //bool literal
    if (read == "true" || read == "false") {
      if (read == "true") {
        token.bool_content = true;
      } else {
        token.bool_content = false;
      }
      token.token_type = TokenType::BOOL_LITERAL;
      return;
    }

    //keyword
    if (contains_keyword(read)) {
      token.token_type = TokenType::KEYWORD;
      token.textual_content = read;
      return;
      //reference literal
    } else if (read == "null") {
      token.token_type = TokenType::REFERENCE_LITERAL;
      token.textual_content = read;
      return;
      //identifier
    } else {
      token.token_type = TokenType::IDENTIFIER;
      token.textual_content = read;
      return;
    }
  }
  return;
}

/**
 * Steps to the next token
 */
void Tokenizer::step() {

  if (_current == '\n') {
    ++_line_count;
  }

  _current = _lookahead;
  if (_position < _length) {
    _lookahead = _source[_position++];
  } else {
    _lookahead = -1;
  }
}

/**
 * Skips whitespace and other unnecessary character
 */
void Tokenizer::skip() {

  while (isspace(_current)) {
    step();
  }

  //File Header (e. g. BOM)
  if (_current == (char) 0xEF || _current == (char) 0xBB
      || _current == (char) 0xBF) {  //Ignore ﻿ BOM Header in text-files
    step();
  }
}

/**
 * Check if a character is a valid first char of an identifier
 */
bool Tokenizer::validate_identifier_cf(char c) {

  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '|') || (c == '$') || c == '_');
}

/**
 * Checks if a character is a valid identifier char
 */
bool Tokenizer::validate_identifier_c(char c) {

  return validate_identifier_cf(c) || (c >= '0' && c <= '9');
}

/**
 * Checks if a character matches a punctuator
 */
bool Tokenizer::validate_punctuator(char c) {

  if (_punctuator.find(c) != std::string::npos) {
    return true;
  }
  return false;
}

/**
 * Reads a string literal
 */
std::string Tokenizer::ReadTextualLiteral() {

  std::stringstream sstream;
  while (_current != '"') {
    sstream << _current;
    step();
  }
  step();  //steps ending "
  return sstream.str();
}

/**
 * Reads a identifier
 */
std::string Tokenizer::ReadIdentifier() {

  std::stringstream sstream;
  while (validate_identifier_c(_current)) {
    sstream << _current;
    step();
  }
  return sstream.str();
}

/**
 * Reads punctuator
 */
std::string Tokenizer::ReadPunctuator() {

  std::stringstream sstream;

  if (validate_punctuator(_current)) {
    sstream << _current;
  }

  bool increment = _current == '+' && _lookahead == '+';
  bool decrement = _current == '-' && _lookahead == '-';
  bool add = _current == '+' && _lookahead == '=';
  bool sub = _current == '-' && _lookahead == '=';
  bool mul = _current == '*' && _lookahead == '=';
  bool div = _current == '/' && _lookahead == '=';
  bool compare = (_current == '<' || _current == '>' || _current == '=' || _current == '!') && _lookahead == '=';
  bool range = _current == '.' && _lookahead == '.';
  bool bool_equality = (_current == '&' && _lookahead == '&') || (_current == '|' && _lookahead == '|');

  //Checks if it's a double character punctuator
  if (increment || decrement || add || sub || mul || div || compare || bool_equality || range) {
    sstream << _lookahead;
    step();
  }

  step();

  return sstream.str();
}

/**
 * Numeric literal
 */
bool Tokenizer::TokenizeNumericLiteral(Token &token) {

  if (isdigit(_current)) {

    bool floating_point = false;

    std::stringstream sstream;
    while (isdigit(_current) || _current == '.') {
      if (!floating_point && _current == '.') {
        floating_point = true;
      }
      sstream << _current;
      step();
    }

    double number;
    sstream >> number;

    if (floating_point) {
      if (_current == 'f') {
        std::cout << "float" << std::endl;
        token.token_type = TokenType::NUMERIC_LITERAL_FLOAT;
        step();
      } else {
        std::cout << "double" << std::endl;
        token.token_type = TokenType::NUMERIC_LITERAL_DOUBLE;
      }
    } else {
      token.token_type = TokenType::NUMERIC_LITERAL_INT;
    }
    token.numeric_content = number;
    return true;
  }
  return false;
}

/**
 * String literal
 */
bool Tokenizer::TokenizeStringLiteral(Token &token) {

  if (_current == '"') {
    step();
    std::string read = ReadTextualLiteral();
    token.token_type = TokenType::TEXTUAL_LITERAL;
    token.textual_content = read;
    return true;
  }
  return false;
}
