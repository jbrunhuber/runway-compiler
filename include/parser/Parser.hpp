//
// Parser.cpp
// Parser for the runway language
//
// Created by Joshua Brunhuber on 30.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_PARSER_PARSER_HPP
#define RUNWAY_PARSER_PARSER_HPP

#include <cstring>
#include <vector>
#include <iostream>
#include <logger.h>

#define IS_IDENTIFIER_V(token, value)        ((token.token_type == TokenType::KEYWORD) && (token.textual_content == value))
#define IS_IDENTIFIER(value)                 (IS_IDENTIFIER_V(_current_token, value))
#define IS_PUNCTUATOR_V(token, value)        ((token.token_type == TokenType::PUNCTUATOR) && (token.textual_content == value))
#define IS_PUNCTUATOR(value)                 (IS_PUNCTUATOR_V(_current_token, value))
#define IS_KEYWORD_V(token, value)           (token.token_type == TokenType::KEYWORD) && (token.textual_content == value)
#define IS_KEYWORD(value)                    (IS_KEYWORD_V(_current_token, value))
#define IS_TOKEN_TYPE_V(token, token_t)      (token.token_type == token_t)
#define IS_TOKEN_TYPE(token_t)               (IS_TOKEN_TYPE_V(_current_token, token_t))

#define ERROR_PRINT(action)                  (std::cerr << "Line " << _current_token.line_count << ": " << action << std::endl)

#include <parser/NodesForward.hpp>
#include <parser/Nodes.hpp>
#include <tokenizer/Tokenizer.hpp>

class Parser {
 public:
  Parser(Tokenizer *tokenizer);

  bool parseStatement(Statement **);
  bool parseExpressionStatement(Statement **);
  bool parseIfStatement(IfStatement **);
  bool parseExpression(Expression **);
  bool parseBodyStatement(BodyStatement **);
  bool parseVariableDeclarationStatement(VariableDeclarationStatement **);
  bool parseWhileStatement(WhileLoopStatement **);
  bool parseForStatement(ForStatement **);

  void factor(Expression **);
  bool parseAssignmentExpression(Expression **);
  bool parseLogicalOrExpression(Expression **);
  bool parseLogicalAndExpression(Expression **);
  bool parseEqualityExpression(Expression **);
  bool parseRelationalExpression(Expression **);
  bool parseAdditiveExpression(Expression **);
  bool parseMultiplicativeExpression(Expression **);
  bool parseUnaryExpression(Expression **);
  bool parsePostFixExpression(Expression **);
  bool parsePrimaryExpression(Expression **);
  bool parseFunctionCallExpression(Expression **);

 private:
  Token _current_token;
  Token _lookahead_token;
  Tokenizer *_tokenizer;

  bool isType(Token &t);
  void nextToken();
};

#endif /* defined(RUNWAY_PARSER_PARSER_HPP) */
