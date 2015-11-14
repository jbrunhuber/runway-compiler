//
// runway_test.cpp
// Provides unittests for testing of the codegen functionality
//
// Created by Joshua Brunhuber on 14.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/Parser.hpp>

/**
 *
 */
int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/**
 * Tests the allocation of a double
 */
TEST(CODEGEN, DECLARATION_DOUBLE) {

  const char *expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "}";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::DOUBLE;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "d";

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = nullptr;

  Generator *generator = new Generator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_TRUE(strcmp(expected, generator->getIR().c_str()));
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_DOUBLE) {

  const char *expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "  store double 1.330000e+01, double* %d\n"
      "}";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::DOUBLE;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "d";

  //double assignment value
  PrimaryExpression *assignment_value = new PrimaryExpression;
  assignment_value->type = ExpressionType::DOUBLE;
  assignment_value->double_value = 13.3;

  //expression node
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = ExpressionType::DOUBLE;
  assignment_expr->identifier = identifier_primary_expression;
  assignment_expr->expression_to_assign = assignment_value;

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = assignment_expr;

  Generator *generator = new Generator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_TRUE(strcmp(expected, generator->getIR().c_str()));
}