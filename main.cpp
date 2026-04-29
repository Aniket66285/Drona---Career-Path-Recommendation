#include "models.h"
#include "utils.h"
#include "career.h"
#include "comparison.h"
#include "roadmap.h"
#include "graph.h"
#include "file_handler.h"
#include "ui.h"
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;
static void clearBuf() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
static string inputLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return trim(s);
}
static int inputInt(const string& prompt) {
    int v;
    cout << prompt;
    while (!(cin >> v)) { clearBuf(); cout << "  Enter a number: "; }
    clearBuf();
    return v;
}
static vector<string> collectSkills() {
    vector<string> skills;
    cout << "\n  Enter your skills one per line \n  Type 'done' when finished\n\n";
    while (true) {
        string s = inputLine("Skill: ");
        if (toLower(s) == "done") break;
        if (!s.empty()) 
        { 
            skills.push_back(toLower(s)); cout << " Added: " << s <<"\n"; 
        }
    }
    return skills;
}

static vector<string> collectInterests() {
    cout << "\n  Enter interests(e.g. data, web, security)\n  Type 'done' when finished.\n\n";
    vector<string> ints;
    while (true) {
        string s = inputLine("interest: ");
        if (toLower(s) == "done") break;
        if (!s.empty()) ints.push_back(toLower(s));
    }
    return ints;
}
static string pickCareer(const vector<Career>& careers, const string& prompt) {
    auto names = careerNames(careers);
    cout << "\n  Available careers\n";
    for (int i = 0; i < (int)names.size(); i++)
        cout << "    [" << (i+1) << "] " << names[i] << "\n";
    while (true) {
        int ch = inputInt("\n  " + prompt + " (number): ");
        if (ch >= 1 && ch <= (int)names.size()) return names[ch - 1];
        cout << "Try again\n";
    }
}
static void handleRecommendation(UI& ui, const vector<Career>& careers, const UserProfile& user) {
    UserProfile u = user;
    u.skills    = collectSkills();
    u.interests = collectInterests();
    if (u.skills.empty()) { ui.printError("enter at least one skill."); return; }

    cout << "\nAnalysing " << u.skills.size() << "skill(s) across " << careers.size() << " careers...\n";
    vector<MatchResult> top3 = getTop3(u, careers);
    ui.printTop3(top3);

    if (top3.size() < 2) return;
    cout << "\n  Compare two top matches? (y/n): ";
    char yn; cin >> yn; clearBuf();
    if (tolower(yn) != 'y') return;

    cout << "\nWhich two careers would you like to compare?\n";
    for (int i = 0; i < (int)top3.size(); i++)
        cout << "    [" << (i+1) << "] " << top3[i].careerName << "\n";
    int a = inputInt("  Select first  (number): ") - 1;
    int b = inputInt("  Select second (number): ") - 1;
    if (a < 0 || a >= (int)top3.size() || b < 0 || b >= (int)top3.size() || a == b) {
        ui.printError("Invalid selection."); return;
    }
    const Career* ca = findCareer(top3[a].careerName, careers);
    const Career* cb = findCareer(top3[b].careerName, careers);
    if (!ca || !cb) { ui.printError("Career data missing."); return; }

    CompareResult cmp = compareCareers(top3[a], top3[b], *ca, *cb, u);
    ui.printCompare(cmp);
}

static void handleCareerPaths(UI& ui, const vector<Career>& careers, const UserProfile&) {
    string chosen = pickCareer(careers, "Select a career for its roadmap");
    const Career* c = findCareer(chosen, careers);
    if (!c) { ui.printError("Career not found."); return; }

    cout << "\n  Do you want to mark skills you already have? (y/n): ";
    char yn; cin >> yn; clearBuf();
    vector<string> mySkills;
    if (tolower(yn) == 'y') mySkills = collectSkills();

    Roadmap rdm = buildRoadmap(*c, mySkills);
    ui.printRoadmap(rdm);

    cout << "  Show prerequisite learning order for a specific skill? (y/n): ";
    char yn2; cin >> yn2; clearBuf();
    if (tolower(yn2) == 'y') {
        SkillGraph sg = buildDefaultSkillGraph();
        string startSkill = toLower(inputLine("  > Enter skill name: "));
        if (sg.hasSkill(startSkill)) {
            vector<string> order = sg.learningOrder(startSkill);
            cout << "\n  BFS learning order from '" << startSkill << "':\n";
            for (int i = 0; i < (int)order.size(); i++)
                cout << "    " << (i+1) << ". " << order[i] << "\n";
            cout << "\n";
        } else {
            cout << "  (Skill not in dependency graph — no chain available)\n\n";
        }
    }
}
static void handleCompare(UI& ui, const vector<Career>& careers, const UserProfile& user) {
    cout << "\n  Select the FIRST career:\n";
    string nameA = pickCareer(careers, "First career");
    cout << "\n  Select the SECOND career:\n";
    string nameB = pickCareer(careers, "Second career");

    if (nameA == nameB) { ui.printError("Please pick two different careers."); return; }
    const Career* ca = findCareer(nameA, careers);
    const Career* cb = findCareer(nameB, careers);
    if (!ca || !cb) { ui.printError("Career data missing."); return; }

    UserProfile u = user;
    cout << "\n  Enter your skills so we can show what you already have:\n";
    u.skills = collectSkills();

    vector<MatchResult> mr = getTop3(u, {*ca, *cb});
    MatchResult mA, mB;
    for (auto& r : mr) {
        if (r.careerName == nameA) mA = r;
        else                       mB = r;
    }
    mA.careerName = nameA;
    mB.careerName = nameB;

    CompareResult cmp = compareCareers(mA, mB, *ca, *cb, u);
    ui.printCompare(cmp);
}

int main() {
    UI ui;
    ui.printBanner();

    vector<Career> careers = loadCareers("career.txt");
    if (careers.empty()) { ui.printError("Could not load career.txt. Exiting."); return 1; }
    cout << "  Loaded " << careers.size() << " careers.\n";

    SkillGraph skillGraph = buildDefaultSkillGraph();

    UserProfile user;
    user.name = inputLine("\n  Enter your name: ");
    if (!isLikelyName(user.name)) { ui.printError("Invalid name Exiting."); return 0; }
    user.age = inputInt("  Enter your age : ");
    if (user.age <= 0 || user.age > 120) { ui.printError("Invalid age. Exiting."); return 0; }
    cout << "\n  Welcome, " << user.name<<endl;
    bool running = true;
    while (running) {
        ui.printMenu();
        int choice = inputInt("");
        switch (choice) {
            case 1: handleRecommendation(ui, careers, user); break;
            case 2: handleCareerPaths(ui, careers, user);    break;
            case 3: handleCompare(ui, careers, user);        break;
            case 0: running = false;                         break;
            default: ui.printError("Invalid option. Enter 0-3.");
        }
    }

    cout << "\n  Goodbye, " << user.name << "! Best of luck on your career journey.\n\n";
    return 0;
}