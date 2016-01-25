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

  _source = source;

  //musst (!) step twice because we'll need to init lookahead token
  step();
  step();
}

/**
 * Returns the next token
 */
void Tokenizer::next(Token &token) {

  token.textual_content = "";

  skipWhitespace();

  token.line_count = _line_count;

  //end of file
  if (_current == -1) {
    token.token_type = TokenType::T_EOF;
    return;
  }

  //punctuators
  if (isPunctuator(_current)) {
    std::string textual_content = readPunctuator();
    token.token_type = TokenType::PUNCTUATOR;
    token.textual_content = textual_content;
    return;
  }

  //numeric literal
  if (tokenizeNumericLiteral(token)) {
    return;
  }

  //string literal
  if (tokenizeStringLiteral(token)) {
    return;
  }

  /**
   * If the first letter of the next token matches an identifier or a valid literal for booleans, keywords and references
   */
  if (isValidIdentifierFirst(_current)) {
    std::string read = readIdentifier();

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
    if (containsKeyword(read)) {
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
void Tokenizer::skipWhitespace() {

  while (isspace(_current)) {
    step();
  }

  //File Header (e. g. BOM)
  if (_current == (char) 0xEF || _current == (char) 0xBB || _current == (char) 0xBF) {  //Ignore ﻿ BOM Header in text-files
    step();
  }
}

/**
 * Check if a character is a valid first char of an identifier
 */
bool Tokenizer::isValidIdentifierFirst(char c) {

  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '|') || (c == '$') || c == '_');
}

/**
 * Checks if a character is a valid identifier char
 */
bool Tokenizer::isValidIdentifier(char c) {

  return isValidIdentifierFirst(c) || (c >= '0' && c <= '9');
}

/**
 * Checks if a character matches a punctuator
 */
bool Tokenizer::isPunctuator(char c) {

  if (_punctuator.find(c) != std::string::npos) {
    return true;
  }
  return false;
}

/**
 * Reads a string literal
 */
std::string Tokenizer::readTextualLiteral() {

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
std::string Tokenizer::readIdentifier() {

  std::stringstream sstream;
  while (isValidIdentifier(_current)) {
    sstream << _current;
    step();
  }
  return sstream.str();
}

/**
 * Reads punctuator
 */
std::string Tokenizer::readPunctuator() {

  std::stringstream sstream;

  if (isPunctuator(_current)) {
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
bool Tokenizer::tokenizeNumericLiteral(Token &token) {

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
bool Tokenizer::tokenizeStringLiteral(Token &token) {

  if (_current == '"') {
    step();
    std::string read = readTextualLiteral();
    token.token_type = TokenType::TEXTUAL_LITERAL;
    token.textual_content = read;
    return true;
  }
  return false;
}
