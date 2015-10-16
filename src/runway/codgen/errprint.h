//
// errprint.h
// Macro definitions for error prints
//
// Created by Joshua Brunhuber on 23.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_CODGEN_ERRPRINT_H
#define RUNWAY_CODGEN_ERRPRINT_H

#define ERR_PRINTLN(action)           (std::cerr << "CODEGEN ERROR: " << action << std::endl)
#define ERR_PRINT(action)             (std::cerr << "CODEGEN ERROR: " << action << " ")

#endif /* RUNWAY_CODGEN_ERRPRINT_H */
