#include "career.h"
#include "utils.h"
#include <algorithm>
#include <set>
using namespace std;
double scoreCareer(const vector<string>& userSkills,
                   const Career& career,
                   vector<string>& outMatched,
                   vector<string>& outMissing)
     {
    double total   = 0.0;
    double matched = 0.0;
    set<string> userSet;
    for (const auto& s : userSkills) userSet.insert(toLower(s));
    for (auto it = career.skillWeights.begin(); it != career.skillWeights.end(); ++it) {
        const string& sk = it->first;
        double w = it->second;
        total += w;
        if (userSet.count(toLower(sk))) {
            matched += w;
            outMatched.push_back(sk);
        } else {
            outMissing.push_back(sk);
        }
    }
    return (total > 0.0) ? (matched / total) * 100.0 : 0.0;
}
vector<MatchResult> getTop3(const UserProfile& user,
                            const vector<Career>& careers)
{
    set<string> intSet;
    for (const auto& i : user.interests) intSet.insert(toLower(i));
    vector<MatchResult> all;
    all.reserve(careers.size());
    for (const auto& c : careers) {
        MatchResult r;
        r.careerName = c.name;
        r.category   = c.category;
        r.rawPercent = scoreCareer(user.skills, c, r.matchedSkills, r.missingSkills);
        bool boosted = false;
        string lname = toLower(c.name);
        string lcat  = toLower(c.category);
        for (const auto& kw : intSet) {
            if (lname.find(kw) != string::npos ||
                lcat.find(kw)  != string::npos) {
                boosted = true; break;
            }
        }
        r.matchPercent = min(100.0, r.rawPercent * (boosted ? 1.15 : 1.0));
        r.grade        = gradeOf(r.matchPercent);
        all.push_back(r);
    }
   sort(all.begin(), all.end(), [](const MatchResult& a, const MatchResult& b){
        return a.matchPercent > b.matchPercent;
    });
    vector<MatchResult> top;
    for (int i = 0; i < min(3, (int)all.size()); i++) {
        all[i].rank = i + 1;
        top.push_back(all[i]);
    }
    return top;
}