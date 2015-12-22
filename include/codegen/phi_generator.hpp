//
// phi_generator.hpp
// code generation visitor
//
// Created by Joshua Brunhuber on 22.12.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODEGEN_PHIGENERATOR_HPP
#define RUNWAY_CODEGEN_PHIGENERATOR_HPP

class phi_generator : public base_generator {


  llvm::Value *emitAssignmentExpression(AssignmentExpression *assignment_expr);
};


#endif /* RUNWAY_CODEGEN_PHIGENERATOR_HPP */
