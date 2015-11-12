/*
 * debug.h
 *
 *	Dirty things for debugging. Remove for official stable getIR
 *
 *  Created on: 11.08.2015
 *      Author: jbrunhuber
 */

#ifndef RUNWAY_PARSER_DEBUG
#define RUNWAY_PARSER_DEBUG

#define DEBUG_PRINTLN(action)							(std::cout << action << std::endl)
#define DEBUG_PRINT(action)							(std::cout << action << " ")

int or_expr_cnt = 0;
int and_expr_cnt = 0;

#endif /* defined(RUNWAY_PARSER_DEBUG) */
