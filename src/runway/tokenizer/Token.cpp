//
// token.cpp
// Implementation of a token
//
// Created by Joshua Brunhuber on 14.08.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <tokenizer/token.hpp>

void Token::printTokenInformation() {

  std::cout << "current ";
  if (token_type == TokenType::BOOL_LITERAL) {
    std::cout << "bool:" << bool_content << std::endl;
  } else if (token_type == TokenType::COMMENT) {
    std::cout << "comment" << std::endl;
  } else if (token_type == TokenType::IDENTIFIER) {
    std::cout << "identifier:" << textual_content << std::endl;
  } else if (token_type == TokenType::KEYWORD) {
    std::cout << "keyword:" << textual_content << std::endl;
  } else if (token_type == TokenType::NUMERIC_LITERAL_FLOAT) {
    std::cout << "number:" << numeric_content << std::endl;
  } else if (token_type == TokenType::PUNCTUATOR) {
    std::cout << "punctuator:" << textual_content << std::endl;
  } else if (token_type == TokenType::TEXTUAL_LITERAL) {
    std::cout << "string:" << textual_content << std::endl;
  } else if (token_type == TokenType::T_EOF) {
    std::cout << "EOF Token" << std::endl;
  }
}
