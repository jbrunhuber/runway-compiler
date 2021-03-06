//
// main.cpp
// Runway Compiler
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <parser/parser.hpp>
#include <fstream>

std::string getSourceCode(std::string path);

/**
 * for testing
 */
const char *DEFAULT_DEBUG_PATH = "/Users/josh/Documents/";

/**
 *
 */
int main(int argc, char **argv) {

  std::cout << "Welcome to Runway!" << std::endl;

  //read the source code
  std::string runway_source_path = DEFAULT_DEBUG_PATH;
  std::string runway_source_code = getSourceCode(runway_source_path + "Runway.rw");

  //create instances
  Tokenizer *tokenizer = new Tokenizer(runway_source_code);
  Parser *parser = new Parser(tokenizer);
  BaseGenerator *base_code_generator = new BaseGenerator;

  //parse and emit the code
  base_code_generator->construct();

  BlockStatement *init_block = new BlockStatement;
  parser->ParseBlockStatement(&init_block);
  init_block->Emit(base_code_generator);

  base_code_generator->finalize();

  //write the emitted code as llvm assembly
  std::ofstream ir_file;
  ir_file.open(runway_source_path + "out.ll", std::ios::out | std::ios::trunc);
  ir_file << base_code_generator->GetIR();
  ir_file.close();

  delete base_code_generator;
  delete parser;
  delete tokenizer;

  std::cout << "Your outputfile has been created on " << runway_source_path << "out.ll" << std::endl;
  std::cout << "Have a nice day/night" << std::endl;
}

/**
 * Returns the file content of the parameter path file
 */
std::string getSourceCode(std::string path) {

  std::ifstream source_file(path);
  std::stringstream sstream;
  sstream << source_file.rdbuf();
  std::string content = sstream.str();
  source_file.close();
  return content;
}
