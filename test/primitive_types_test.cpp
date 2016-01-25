//
// primitive_types_test.cpp
// Provides unittests for testing of the codegen functionality
//
// Created by Joshua Brunhuber on 14.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/parser.hpp>

/**
 * Tests the allocation of a double
 */
TEST(CODEGEN, DECLARATION_DOUBLE) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "}\n";

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

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_DOUBLE) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "  store double 1.330000e+01, double* %d\n"
      "}\n";

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

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Tests the allocation of a double
 */
TEST(CODEGEN, DECLARATION_FLOAT) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %f = alloca float\n"
      "}\n";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::FLOAT;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "f";

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = nullptr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_FLOAT) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %f = alloca float\n"
      "  store float 0x402A9999A0000000, float* %f\n"
      "}\n";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::FLOAT;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "f";

  //double assignment value
  PrimaryExpression *assignment_value = new PrimaryExpression;
  assignment_value->type = ExpressionType::FLOAT;
  assignment_value->double_value = 13.3;

  //expression node
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = ExpressionType::FLOAT;
  assignment_expr->identifier = identifier_primary_expression;
  assignment_expr->expression_to_assign = assignment_value;

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = assignment_expr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_INT_TO_DOUBLE) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "  store double 1.300000e+01, double* %d\n"
      "}\n";

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
  assignment_value->type = ExpressionType::INTEGER;
  assignment_value->int_value = 13;

  //expression node
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = ExpressionType::DOUBLE;
  assignment_expr->identifier = identifier_primary_expression;
  assignment_expr->expression_to_assign = assignment_value;

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = assignment_expr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_FLOAT_TO_DOUBLE) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %d = alloca double\n"
      "  store double 0x402A9999A0000000, double* %d\n"
      "}\n";

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
  assignment_value->type = ExpressionType::FLOAT;
  assignment_value->double_value = 13.3;

  //expression node
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = ExpressionType::FLOAT;
  assignment_expr->identifier = identifier_primary_expression;
  assignment_expr->expression_to_assign = assignment_value;

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = assignment_expr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}


/**
 * tests the allocation of an integer
 */
TEST(CODEGEN, DECLARATION_INTEGER) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %test = alloca i32\n"
      "}\n";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::INTEGER;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "test";

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = nullptr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}

/**
 * Allocates a double and assign a test value
 */
TEST(CODEGEN, ASSIGNMENT_INTEGER) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %i = alloca i32\n"
      "  store i32 13, i32* %i\n"
      "}\n";

  VariableDeclarationStatement *variable_declaration_statement = new VariableDeclarationStatement;

  //type
  PrimaryExpression *type_primary_expression = new PrimaryExpression;
  type_primary_expression->type = ExpressionType::INTEGER;

  //identifier
  IdentifierPrimaryExpression *identifier_primary_expression = new IdentifierPrimaryExpression;
  identifier_primary_expression->type = ExpressionType::IDENTIFIER;
  identifier_primary_expression->string_value = "i";

  //double assignment value
  PrimaryExpression *assignment_value = new PrimaryExpression;
  assignment_value->type = ExpressionType::INTEGER;
  assignment_value->int_value = 13;

  //expression node
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = ExpressionType::INTEGER;
  assignment_expr->identifier = identifier_primary_expression;
  assignment_expr->expression_to_assign = assignment_value;

  //build variableDeclarationStatement
  variable_declaration_statement->type = type_primary_expression;
  variable_declaration_statement->identifier = identifier_primary_expression;
  variable_declaration_statement->expression_to_assign = assignment_expr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  variable_declaration_statement->emit(generator);

  EXPECT_EQ(expected, generator->getIR().c_str());
}
