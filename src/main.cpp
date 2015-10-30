//
// main.cpp
// Runway Compiler
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>


#include "../include/tokenizer/Tokenizer.hpp"
#include "../include/parser/Parser.hpp"

std::string getSourceCode(std::string path);

/**
 * for testing
 */
const char *DEFAULT_DEBUG_PATH = "/users/josh/Desktop/";

/**
 *
 */
int main(int argc, char **argv) {

  std::cout << "Welcome to Runway!" << std::endl;

  DebugManager::debug = true; //prints debug messages

  //read the source code
  std::string runway_source_path = DEFAULT_DEBUG_PATH;
  std::string runway_source_code = getSourceCode(runway_source_path + "Runway.rw");

  //create instances
  std::unique_ptr<Tokenizer> tokenizer(new Tokenizer(runway_source_code));
  std::unique_ptr<Parser> parser(new Parser(tokenizer.get()));
  std::unique_ptr<Generator> generator(new Generator());

  //parse and emit the code
  generator->construct();

  Statement *super_node = 0;
  while (parser->parseStatement(&super_node)) {
    super_node->emit(generator.get());
  }

  generator->finalize();

  //write the emitted code as llvm assembly
  std::ofstream ir_file;
  ir_file.open(runway_source_path + "out.ll", std::ios::out | std::ios::trunc);
  ir_file << generator->getIR();
  ir_file.close();

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
