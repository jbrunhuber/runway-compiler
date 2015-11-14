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

TEST(CODEGEN, DECLARATION_DOUBLE) {

  const char *expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "}";

  VariableDeclarationStatement *variableDeclarationStatement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::DOUBLE;

  //identifier
  IdentifierPrimaryExpression *identifierPrimaryExpression = new IdentifierPrimaryExpression;
  identifierPrimaryExpression->type = ExpressionType::IDENTIFIER;
  identifierPrimaryExpression->string_value = "d";

  //build variableDeclarationStatement
  variableDeclarationStatement->type = type_primary_expression;
  variableDeclarationStatement->identifier = identifierPrimaryExpression;
  variableDeclarationStatement->expression_to_assign = nullptr;

  Generator *generator = new Generator;
  generator->construct();

  variableDeclarationStatement->emit(generator);

  EXPECT_TRUE(strcmp(expected, generator->getIR().c_str()));
}