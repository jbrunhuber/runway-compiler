//
// codegen_test.cpp
// Provides unittests for testing of the codegen functionality
//
// Created by Joshua Brunhuber on 18.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <gtest/gtest.h>
#include <parser/Parser.hpp>

#include "codegen/Generator.hpp"

/**
 * Create main-function and test the result IR
 */
TEST(CODEGEN, BASE) {

  std::unique_ptr<Generator> generator(new Generator());
  generator->construct();
}

/**
 *
 */
TEST(CODEGEN, ASSIGNMENT_EXPR) {

  const char *expected_ir = "; Module ID = 'runway'"
      ""
      "@0 = private unnamed_addr constant [12 x i8] c\"hello world\00\""
      ""
      "define void @main() {"
      "entrypoint:"
      "  %test_identifier = alloca i8*"
      "  store i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0), i8** %test_identifier"
      "  ret void"
      "}";
  const char *result_ir = "none";

  //mock vars for assignment expr
  std::string identifier = "test_identifier";
  Operator assignment_operator = Operator::ASSIGNMENT;

  //mock vars for primary expression (assignment value)
  PrimaryExpression *expr_to_assign = new PrimaryExpression;
  ExpressionType expr_type = ExpressionType::STRING;
  expr_to_assign->string_value = "hello world";
  expr_to_assign->expr_type = expr_type;

  std::unique_ptr<Generator> generator(new Generator());
  AssignmentExpression *assignment_expr = new AssignmentExpression;
  assignment_expr->assignment_operator = assignment_operator;
  //assignment_expr->identifier = identifier;

  generator->construct();
  assignment_expr->emit(generator.get());

  result_ir = generator->getIR().c_str();
  std::cout << result_ir << std::endl;
  ASSERT_EQ(result_ir, expected_ir);

}

/**
 * Test for a primary expression: numeric
 */
TEST(CODEGEN, PRIMARY_EXPR_NUM) {

  //create my compare strings
  const char *expected_ir = "double 1.330000e+01";
  const char *result_ir;

  //create my instances
  std::unique_ptr<Generator> generator(new Generator());
  std::unique_ptr<PrimaryExpression> primary(new PrimaryExpression());

  //create a mock double value primary expression
  primary->expr_type = ExpressionType::FLOAT;
  primary->double_value = 13.3;

  //get the result as an llvm::Value from the generator
  std::unique_ptr<llvm::Value> llvm_result_value(
      generator->emitPrimaryExpression(primary.get()));

  //now i ask the generator for the llvm assembly and compare it with our expected code
  result_ir = (generator->getIR()).c_str();
  EXPECT_STREQ(result_ir, expected_ir);

  //i additionally check that the result llvm::Value is not null
  EXPECT_FALSE(llvm_result_value.get() == nullptr);
}

/**
 * Test for a primary expression: bool
 */
TEST(CODEGEN, PRIMARY_EXPR_BOOL) {

  //create my compare strings
  const char *expected_ir = "";  //TODO
  const char *result_ir;

  //create my instances
  std::unique_ptr<Generator> generator(new Generator());
  std::unique_ptr<PrimaryExpression> primary(new PrimaryExpression());

  //create a mock bool primary expression
  primary->expr_type = ExpressionType::BOOL;
  primary->bool_value = true;

  //get the result as an llvm::Value from the generator
  std::unique_ptr<llvm::Value> llvm_result_value(
      generator->emitPrimaryExpression(primary.get()));

  //now i ask the generator for the llvm assembly and compare it with our expected code
  result_ir = (generator->getIR()).c_str();
  EXPECT_STREQ(result_ir, expected_ir);

  //i additionally check that the result llvm::Value is not null
  EXPECT_FALSE(llvm_result_value.get() == nullptr);
}

/**
 * Test for a primary expression: string
 */
TEST(CODEGEN, PRIMARY_EXPR_STR) {

  //create my compare strings
  const char *result_ir = "none";
  const char *expected_ir =
      "; ModuleID = 'top'\n"
          "@0 = internal unnamed_addr constant [47 x i8] c\"just a basic test string to write in our ir :)\0A\00\"\n"
          "define void @main() {\n"
          "entrypoint:\n"
          "}";

  //create my instances
  Generator *generator = new Generator;
  std::unique_ptr<PrimaryExpression> primary(new PrimaryExpression());

  //create a mock string primary expression
  primary->expr_type = ExpressionType::STRING;
  primary->string_value = "just a basic test string to write in our ir :)";
  generator->construct();  //the builder requires a function and basic block, otherwise segfault

  //get the result as an llvm::Value from the generator
  llvm::Value *llvm_result_value = generator->emitPrimaryExpression(
      primary.get());

  //now i ask the generator for the llvm assembly and compare it with our expected code
  result_ir = (generator->getIR()).c_str();
  EXPECT_STREQ(result_ir, expected_ir);

  //i additionally check that the result llvm::Value is not null
  EXPECT_FALSE(llvm_result_value == nullptr);

  delete generator;
}

/**
 * Test for a variable declaration statement
 */
TEST(CODEGEN, VAR_DECLARATION_STMT) {

  //create my compare strings
  const char *expected_ir = "; ModuleID = 'runway'"
      ""
      "define void @main() {\n"
      "entry:\n"
      "  %0 = alloca i32\n"
      "}";
  const char *result_ir;

  //create my instances
  std::unique_ptr<Generator> generator(new Generator());
  std::unique_ptr<PrimaryExpression> variable_identifier(
      new PrimaryExpression());
  std::unique_ptr<PrimaryExpression> assignment_value(new PrimaryExpression());
  std::unique_ptr<VariableDeclarationStatement> variable_declaration_stmt(
      new VariableDeclarationStatement());

  //create mock variable declaration statement

  assignment_value->int_value = 13;
  //variable_declaration_stmt->identifier = "test"; TODO
  //variable_declaration_stmt->expression_to_assign = assignment_value.get();

  //emits code for variable declaration statement
  generator->construct();
  generator->emitVariableDeclarationStatement(variable_declaration_stmt.get());

  //now i ask the generator for the llvm assembly and compare it with our expected code
  result_ir = (generator->getIR()).c_str();
  EXPECT_STREQ(result_ir, expected_ir);
}

/**
 * Test for a function call postfix expression
 */
TEST(CODEGEN, PRINT_POSTFIX_FUNCTION_CALL) {

  //create my compare strings
  const char *expected_ir = "";  //TODO
  const char *result_ir;

  //create my instances
  std::unique_ptr<Generator> generator(new Generator());
  std::unique_ptr<PrimaryExpression> function_call_identifier(
      new PrimaryExpression());
  std::unique_ptr<PrimaryExpression> arg_parameter(new PrimaryExpression());
  std::unique_ptr<FunctionCallPostfixExpression> function_call(
      new FunctionCallPostfixExpression);

  //identifier
  function_call_identifier->string_value = "print";
  function_call_identifier->expr_type = ExpressionType::IDENTIFIER;

  //assign the value
  arg_parameter->string_value = "hello world";
  arg_parameter->expr_type = ExpressionType::STRING;

  //create a parameter list
  std::vector<Expression *> args;
  args.push_back(arg_parameter.get());

  //fill mock function call expression
  //function_call->identifier = function_call_identifier.get();
  function_call->arguments = args;

  //emir IR
  generator->construct();
  function_call->emit(generator.get());

  //now i ask the generator for the llvm assembly and compare it with our expected code
  result_ir = (generator->getIR()).c_str();
  EXPECT_STREQ(result_ir, expected_ir);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
