#ifndef UI_H
#define UI_H
#include "models.h"
#include <vector>
#include <string>
class UI {
public:
    void printBanner() const;
    void printMenu()   const;

    void printTop3(const std::vector<MatchResult>& top3) const;
    void printCompare(const CompareResult& cmp) const;
    void printRoadmap(const Roadmap& rdm) const;

    void printError(const std::string& msg) const;
    void printSuccess(const std::string& msg) const;
    
    // Changed default to ' ' space to remove all symbols
    void printDivider(char c = ' ', int w = 58) const;
    void printSectionHeader(const std::string& title) const;
};

#endif