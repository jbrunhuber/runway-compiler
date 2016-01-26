//
// additive_codegen_test.cpp
// Provides unittests for testing of the codegen (additive) functionality
//
// Created by Joshua Brunhuber on 15.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/nodes.hpp>

/**
 *
 */
TEST(CODEGEN, ADDITIVE_INT) {

  int test_operand_1 = 4;
  int test_operand_2 = 6;

  ExpressionType int_type = ExpressionType::INTEGER;

  AdditiveExpression *node = new AdditiveExpression;

  PrimaryExpression *number1 = new PrimaryExpression;
  number1->type = int_type;
  number1->int_value = test_operand_1;

  PrimaryExpression *number2 = new PrimaryExpression;
  number2->type = int_type;
  number2->int_value = test_operand_2;

  node->lhs_expr = number1;
  node->rhs_expr = number2;
  node->additive_operator = Operator::SUM;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  llvm::ConstantInt *llvm_int = (
      llvm::ConstantInt *) node->Emit(generator);
  EXPECT_EQ(llvm_int->getZExtValue(), (test_operand_1 + test_operand_2));
}

/**
 *
 */
TEST(CODEGEN, ADDITIVE_FLOAT) {

  std::string expected = "; ModuleID = 'runway'\n"
      "\n"
      "define void @main() {\n"
      "entrypoint:\n"
      "  %test_add = alloca double\n"
      "  store double 0x4026333333333334, double* %test_add\n"
      "}\n";

  double test_operand_1 = 4.7;
  double test_operand_2 = 6.4;

  ExpressionType expr_type = ExpressionType::DOUBLE;

  AdditiveExpression *node = new AdditiveExpression;

  PrimaryExpression *number1 = new PrimaryExpression;
  number1->type = expr_type;
  number1->double_value = test_operand_1;

  PrimaryExpression *number2 = new PrimaryExpression;
  number2->type = expr_type;
  number2->double_value = test_operand_2;

  node->lhs_expr = number1;
  node->rhs_expr = number2;
  node->additive_operator = Operator::SUM;

  VariableDeclarationStatement *decl = new VariableDeclarationStatement;
  PrimaryExpression *type = new PrimaryExpression;
  type->type = expr_type;
  decl->type = type;

  IdentifierPrimaryExpression *identifierPrimaryExpression = new IdentifierPrimaryExpression;
  identifierPrimaryExpression->type = expr_type;
  identifierPrimaryExpression->string_value = "test_add";
  decl->identifier = identifierPrimaryExpression;

  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->type = expr_type;
  assignment_expr->identifier = identifierPrimaryExpression;
  assignment_expr->expression_to_assign = node;

  decl->expression_to_assign = assignment_expr;

  BaseGenerator *generator = new BaseGenerator;
  generator->construct();

  decl->Emit(generator);

  EXPECT_EQ(expected, generator->GetIR());
}
