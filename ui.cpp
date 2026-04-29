#include "ui.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
static const int W = 58;
static string progressBar(double pct, int width = 20) {
    int filled = (int)(pct / 100.0 * width);
    string bar = " ";
    for (int i = 0; i < width; i++)
        bar += (i < filled) ? "X" : "i";
    bar += " ";
    return bar;
}
static string center(const string& s, int w) {
    int pad = w - (int)s.size();
    if (pad <= 0) return s;
    int l = pad / 2, r = pad - l;
    return string(l, ' ') + s + string(r, ' ');
}
void UI::printDivider(char c, int w) const {
     cout << "\n";
}
void UI::printSectionHeader(const string& title) const {
    cout << "\n" <<title<< "\n";
}
void UI::printBanner() const {
    cout << "\n" << center("DRONAA", W) << "\n";
    cout << center("Career Advisor System", W) << "\n";
}
void UI::printMenu() const {
    cout << "  MENU\n";
    cout << "  1 Career Recommendation\n";
    cout << "  2 Career Paths\n";
    cout << "  3 Compare Careers\n";
    cout << "  0 Exit\n";
    cout << "  Choice ";
}
void UI::printTop3(const vector<MatchResult>& top3) const {
    if (top3.empty()) { printError("No matches found"); return; }
    cout << "MATCH RESULTS\n";

    for (const auto& r : top3) {
        int pct = (int)r.matchPercent;
        cout << "\n  Rank " << r.rank << " " << r.careerName << "\n";
        cout << "  Match " << progressBar(pct)<< pct << " percent\n";
        
        if (!r.matchedSkills.empty()) {
            cout << "  Skills you have ";
            for (const auto& s : r.matchedSkills) cout << s << " ";
            cout << "\n";
        }
        if (!r.missingSkills.empty()) {
            cout << "  Skills to learn ";
            for (const auto& s : r.missingSkills) cout << s << " ";
            cout << "\n";
        }
    }
}
void UI::printCompare(const CompareResult& cmp) const {
    cout << "\n  COMPARISON\n";
    cout << "  " << cmp.nameA << " vs " << cmp.nameB << "\n";
    
    cout << "  " << (int)cmp.pctA << " percent match " << cmp.nameA << "\n";
    cout << "  " << (int)cmp.pctB << " percent match " << cmp.nameB << "\n";

    cout << "  VERDICT\n  " << cmp.verdict << "\n";
    cout << "  WINNER " << cmp.winner << "\n";
}

void UI::printRoadmap(const Roadmap& rdm) const {
    cout << "\n  ROADMAP " << rdm.careerName << "\n";
    
    for (const auto& ph : rdm.phases) {
        cout << "  PHASE " << ph.number << " " << ph.title << "\n";
        cout << "  Duration " << ph.duration << "\n";
        cout << "  Focus ";
        for (const auto& sk : ph.skills) cout << sk << " ";
        cout << "\n";
    }
}

void UI::printError(const string& msg) const {
    cout << "ERROR"<< msg << "\n";
}

void UI::printSuccess(const string& msg) const {
    cout << "\n SUCCESS"<< msg << "\n";
}