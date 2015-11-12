//
// Operator.hpp
// enumeration for operators included in runway
//
// Created by Joshua Brunhuber on 04.08.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_PARSER_OPERATOR_HPP
#define RUNWAY_PARSER_OPERATOR_HPP

/**
 * Operators
 */
enum class Operator {

  SUM,
  SUB,
  MUL,
  DIV,
  ASSIGNMENT,
  ASSIGNMENT_ADD,
  ASSIGNMENT_SUB,
  COMPARE_GREATER,
  COMPARE_EQUAL,
  COMPARE_LESS,
  COMPARE_UNEQUAL,
  COMPARE_GREATER_OR_EQUAL,
  COMPARE_LESS_OR_EQUAL,
  INCREMENT,
  DECREMENT,
  NEGATE,
  NEGATIVE,
  NONE
};

#endif /* defined(RUNWAY_PARSER_OPERATOR_HPP) */
