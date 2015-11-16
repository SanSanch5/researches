#ifndef RESULTSANALYSER
#define RESULTSANALYSER

#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <iostream>

enum SortType 
{ 
    MERGE=0, HEAP, QUICK, MEDIAN_QUICK, SMART_QUICK, MERGE_HEAP
}; 

std::vector<std::string> split(const std::string &s, char delim);
std::vector<int> findInsertionEqualN();
std::vector<int> findInsertionBest();

#endif
