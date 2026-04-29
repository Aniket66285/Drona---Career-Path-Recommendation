#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Career {
    string              name;
    string              category;
    vector<string>      skills;
    map<string,double>  skillWeights;
    string              description;
};

struct MatchResult {
    string          careerName;
    string          category;
    double          matchPercent;
    double          rawPercent;
    vector<string>  matchedSkills;
    vector<string>  missingSkills;
    string          grade;
    int             rank;
};

struct CompareResult {
    string  nameA, nameB;
    double  pctA, pctB;
    string  winner;
    vector<pair<string,bool>> skillA;
    vector<pair<string,bool>> skillB;
    string  verdict;
};

struct Phase {
    int             number;
    string          title;
    vector<string>  skills;
    string          tip;
    string          duration;
};

struct Roadmap {
    string          careerName;
    vector<Phase>   phases;
    string          overview;
};

struct UserProfile {
    string          name;
    int             age;
    vector<string>  skills;
    vector<string>  interests;
};

#endif