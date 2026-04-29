#include "roadmap.h"
#include "utils.h"
#include <algorithm>
#include <set>
using namespace std;
static const string PHASE_TITLES[4] = {
    "Foundation & Basics",
    "Core Skill Development",
    "Advanced Techniques",
    "Mastery & Real Projects"
};
static const string PHASE_DURATIONS[4] = {
    "3-5 weeks",
    "4-6 weeks",
    "6-8 weeks",
    "8-12 weeks"
};
static const string PHASE_TIPS[4] = {
    "Start with free online tutorials; build a strong base before moving forward.",
    "Work on guided projects and follow official documentation closely.",
    "Join communities, contribute to open-source, or build portfolio pieces.",
    "Apply everything in a capstone project and prepare for interviews."
};
Roadmap buildRoadmap(const Career& career,
                     const vector<string>& userSkills)
{
    set<string> userSet;
    for (const auto& s : userSkills) userSet.insert(toLower(s));

    vector<pair<string,double>> sorted(career.skillWeights.begin(),
                                       career.skillWeights.end());
    sort(sorted.begin(), sorted.end(),
         [](const pair<string,double>& x, const pair<string,double>& y){
             return x.second > y.second;
         });

    int total    = (int)sorted.size();
    int perPhase = max(1, (total + 3) / 4);

    Roadmap rdm;
    rdm.careerName = career.name;
    rdm.overview   = "A structured " + to_string(total) +
                     "-skill path from beginner to job-ready " +
                     career.name + ".";

    for (int p = 0; p < 4; p++) {
        Phase phase;
        phase.number   = p + 1;
        phase.title    = PHASE_TITLES[p];
        phase.duration = PHASE_DURATIONS[p];
        phase.tip      = PHASE_TIPS[p];

        int start = p * perPhase;
        int end   = min(start + perPhase, total);
        for (int i = start; i < end; i++) {
            string sk = sorted[i].first;
            bool known = userSet.count(toLower(sk)) > 0;
            phase.skills.push_back(known ? "[v] " + sk : sk);
        }
        if (!phase.skills.empty())
            rdm.phases.push_back(phase);
    }
    return rdm;
}