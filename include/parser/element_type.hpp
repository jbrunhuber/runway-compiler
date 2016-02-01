//
// element_type.hpp
// Represent the type of a symbol-table entry
//
// Created by Joshua Brunhuber on 29.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_BASE_ELEMENTTYPE_HPP
#define RUNWAY_BASE_ELEMENTTYPE_HPP

/**
 *
 */
enum class ElementType {
  STRING,
  BOOL,
  EXPR,
  FLOAT,
  DOUBLE,
  INTEGER,
  NULL_PTR,
  UNDEFINED,
  IDENTIFIER,
};

#endif /* defined (RUNWAY_BASE_ELEMENTTYPE_HPP) */