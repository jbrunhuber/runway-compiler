//
// Created by Joshua Brunhuber on 31.01.16.
//

#ifndef RUNWAY_EXPRESSION_TYPE_HPP_HPP
#define RUNWAY_EXPRESSION_TYPE_HPP_HPP

/**
 *
 */
enum class ExpressionType {
  STRING,
  BOOL,
  EXPR,
  FLOAT,
  DOUBLE,
  INTEGER,
  NULL_PTR,
  UNDEFINED,
  IDENTIFIER,
  BLOCK
};

#endif //RUNWAY_EXPRESSION_TYPE_HPP_HPP
