//
// additive_codegen_test.cpp
// Provides unittests for testing of the codegen (additive) functionality
//
// Created by Joshua Brunhuber on 15.11.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/Nodes.hpp>

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

  node->lhs_multiplicative_expression = number1;
  node->rhs_additive_expression = number2;
  node->additive_operator = Operator::SUM;

  Generator *generator = new Generator;
  generator->construct();

  llvm::ConstantInt *llvm_int = (llvm::ConstantInt*)node->emit(generator);
  EXPECT_EQ(llvm_int->getZExtValue(), (test_operand_1 + test_operand_2));
}