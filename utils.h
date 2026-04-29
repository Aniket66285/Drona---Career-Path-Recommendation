#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::string trim(const std::string& s);
std::string toLower(const std::string& s);

bool isLikelyName(const std::string& s);

std::string findClosestCareer(const std::string& input,
                              const std::vector<std::string>& names);

int editDistance(const std::string& a, const std::string& b);

std::string repeat(char c, int n);

std::string gradeOf(double pct);

#endif
