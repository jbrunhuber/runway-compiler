//
// errprint.h
// macros for error printing functionality
//
// Created by Joshua Brunhuber on 10.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_ERRPRINT_H
#define RUNWAY_ERRPRINT_H

#define ERR_PRINTLN(action)						(std::cerr << "ERROR: " << action << std::endl)
#define ERR_PRINT(action)							(std::cerr << "ERROR: " << action << " ")

#endif /* defined(RUNWAY_ERRPRINT_H) */
