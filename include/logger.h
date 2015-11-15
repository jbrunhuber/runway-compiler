//
// Macro definitions for prints
//
// Created by Joshua Brunhuber on 23.09.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_LOGGER_H
#define RUNWAY_LOGGER_H

#define ERR_PRINTLN(action)           (std::cerr << "ERROR: " << action << std::endl)
#define ERR_PRINT(action)             (std::cerr << "ERROR: " << action << " ")

#define WARN_PRINTLN(action)          (std::cerr << "WARN: " << action << std::endl)
#define WARN_PRINT(action)            (std::cerr << "WARN: " << action << " ")

#define DEBUG_PRINTLN(action)         (std::cerr << "INFO: " << action << std::endl)
#define DEBUG_PRINT(action)           (std::cerr << "INFO: " << action << " ")

#endif /* RUNWAY_LOGGER_H */
