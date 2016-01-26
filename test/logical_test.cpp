//
// multiplicative_codegen_test.cpp
// Provides unittests for testing of the codegen (multiplicative) functionality
//
// Created by Joshua Brunhuber on 15.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/nodes.hpp>

/**
 * OR test
 */
TEST(CODEGEN, LOGICAL_OR) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %test = alloca i1\n"
      "  store i1 true, i1* %test\n"
      "}\n";

  VariableDeclarationStatement *var_declaration_stmt = new VariableDeclarationStatement;
  PrimaryExpression *type_expr = new PrimaryExpression;
  type_expr->type = ExpressionType::BOOL;

  IdentifierPrimaryExpression *identifier = new IdentifierPrimaryExpression;
  identifier->type = ExpressionType::IDENTIFIER;
  identifier->string_value = "test";

  LogicalOrExpression *logical_or_expr = new LogicalOrExpression;

  PrimaryExpression *lhs_bool = new PrimaryExpression;
  lhs_bool->type = ExpressionType::BOOL;
  lhs_bool->bool_value = true;

  PrimaryExpression *rhs_bool = new PrimaryExpression;
  rhs_bool->type = ExpressionType::BOOL;
  rhs_bool->bool_value = false;

  logical_or_expr->lhs_expr = lhs_bool;
  logical_or_expr->rhs_expr = rhs_bool;

  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->expression_to_assign = logical_or_expr;

  var_declaration_stmt->type = type_expr;
  var_declaration_stmt->expression_to_assign = assignment_expr;
  var_declaration_stmt->identifier = identifier;

  assignment_expr->identifier = identifier;

  BaseGenerator *generator = new BaseGenerator;

  generator->construct();

  var_declaration_stmt->Emit(generator);

  EXPECT_EQ(expected, generator->GetIR());
}

/**
 * AND test
 */
TEST(CODEGEN, LOGICAL_AND) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %test = alloca i1\n"
      "  store i1 false, i1* %test\n"
      "}\n";

  VariableDeclarationStatement *var_declaration_stmt = new VariableDeclarationStatement;
  PrimaryExpression *type_expr = new PrimaryExpression;
  type_expr->type = ExpressionType::BOOL;

  IdentifierPrimaryExpression *identifier = new IdentifierPrimaryExpression;
  identifier->type = ExpressionType::IDENTIFIER;
  identifier->string_value = "test";

  LogicalAndExpression *logical_and_expr = new LogicalAndExpression;

  PrimaryExpression *lhs_bool = new PrimaryExpression;
  lhs_bool->type = ExpressionType::BOOL;
  lhs_bool->bool_value = true;

  PrimaryExpression *rhs_bool = new PrimaryExpression;
  rhs_bool->type = ExpressionType::BOOL;
  rhs_bool->bool_value = false;

  logical_and_expr->lhs_expr = lhs_bool;
  logical_and_expr->rhs_expr = rhs_bool;

  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->expression_to_assign = logical_and_expr;

  var_declaration_stmt->type = type_expr;
  var_declaration_stmt->expression_to_assign = assignment_expr;
  var_declaration_stmt->identifier = identifier;

  assignment_expr->identifier = identifier;

  BaseGenerator *generator = new BaseGenerator;

  generator->construct();

  var_declaration_stmt->Emit(generator);

  EXPECT_EQ(expected, generator->GetIR());
}
