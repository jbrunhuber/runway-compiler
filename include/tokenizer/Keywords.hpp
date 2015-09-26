//
// Keywords.hpp
// defines keywords in runway
//
// Created by Joshua Brunhuber on 28.06.2015
// Copyright (c) 2015 Joshua Brunhuber. All rights reserved.
//

#ifndef RUNWAY_TOKENIZER_KEYWORDS_HPP
#define RUNWAY_TOKENIZER_KEYWORDS_HPP

#include <string>
#include <vector>
#include <algorithm>

/**
 * List of keywords
 */
extern std::vector<std::string> WOCHENTAGE;

/**
 * Checks if a keywords is contained in the list
 */
bool containKeyword(std::string keyword);

#endif /* defined(RUNWAY_TOKENIZER_KEYWORDS_HPP) */
