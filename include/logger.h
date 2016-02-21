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

#define WARN_PRINTLN(action)          (std::cout << "WARN: " << action << std::endl)
#define WARN_PRINT(action)            (std::cout << "WARN: " << action << " ")

#define DEBUG_PRINTLN(action)         (std::cout << "INFO: " << action << std::endl)
#define DEBUG_PRINT(action)           (std::cout << "INFO: " << action << " ")

#define INTERNAL_ERROR(reason)           (std::cerr << "An internal error occurred. The reason is: " << reason << std::endl)

#endif /* RUNWAY_LOGGER_H */
