//
// token_type.hpp
// enumeration for token types available in runway
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOKENIZER_TOKENTYPE_HPP
#define RUNWAY_TOKENIZER_TOKENTYPE_HPP

/**
 * Defines all TokenTypes
 */
enum class TokenType {

  TEXTUAL_LITERAL,
  NUMERIC_LITERAL_INT,
  NUMERIC_LITERAL_FLOAT,
  NUMERIC_LITERAL_DOUBLE,
  REFERENCE_LITERAL,
  BOOL_LITERAL,

  PUNCTUATOR,
  KEYWORD,
  IDENTIFIER,

  COMMENT,
  T_EOF
};

#endif /* defined(RUNWAY_TOKENIZER_TOKENTYPE_HPP) */
