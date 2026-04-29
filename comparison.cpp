#include "comparison.h"
#include "utils.h"
#include <set>
#include <sstream>
using namespace std;
CompareResult compareCareers(const MatchResult& a,
                             const MatchResult& b,
                             const Career& careerA,
                             const Career& careerB,
                             const UserProfile& user)
{
    CompareResult cr;
    cr.nameA = a.careerName;
    cr.nameB = b.careerName;
    cr.pctA  = a.matchPercent;
    cr.pctB  = b.matchPercent;
    set<string> userSet;
    for (const auto& s : user.skills) userSet.insert(toLower(s));
    set<string> allSkills;
    for (const auto& p : careerA.skillWeights) allSkills.insert(p.first);
    for (const auto& p : careerB.skillWeights) allSkills.insert(p.first);
    for (const auto& sk : allSkills) {
        bool inA = careerA.skillWeights.count(sk) > 0;
        bool inB = careerB.skillWeights.count(sk) > 0;
        bool has = userSet.count(toLower(sk)) > 0;
        if (inA) cr.skillA.push_back({sk, has});
        if (inB) cr.skillB.push_back({sk, has});
    }
    cr.winner = (cr.pctA >= cr.pctB) ? cr.nameA : cr.nameB;
    ostringstream oss;
    int gapA = (int)a.missingSkills.size();
    int gapB = (int)b.missingSkills.size();
    oss<< cr.winner << "is stronger match at "
        << (int)(cr.pctA >= cr.pctB ? cr.pctA : cr.pctB) << "% vs "
        << (int)(cr.pctA >= cr.pctB ? cr.pctB : cr.pctA) << "%.\n";
    oss << "• " << cr.nameA << " has " << gapA << " skill gap(s); "
        << cr.nameB << " has " << gapB << " skill gap(s).\n";
    if (gapA < gapB)
        oss << "You would reach " << cr.nameA << "faster with fewer skills to learn.";
    else if (gapB < gapA)
        oss << "You would reach" << cr.nameB << "readiness faster skills to learn.";
    else
        oss << "Both of careers have equal skill gaps choose based on your passion.";
cr.verdict = oss.str();
    return cr;
}