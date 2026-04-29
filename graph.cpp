#include "graph.h"
#include <queue>
#include <set>
using namespace std;
void SkillGraph::addSkill(const string& skill) {
    if (!adj.count(skill)) adj[skill] = {};
    if (!rev.count(skill)) rev[skill] = {};
}
void SkillGraph::addDependency(const string& learnFirst,
                               const string& learnSecond)
{
    addSkill(learnFirst);
    addSkill(learnSecond);
    adj[learnFirst].push_back(learnSecond);
    rev[learnSecond].push_back(learnFirst);
}
bool SkillGraph::hasSkill(const string& skill) const {
    return adj.count(skill) > 0;
}
vector<string> SkillGraph::prereqs(const string& skill) const {
    auto it = rev.find(skill);
    return (it != rev.end()) ? it->second : vector<string>{};
}
vector<string> SkillGraph::learningOrder(const string& startSkill) const {
    vector<string> order;
    if (!adj.count(startSkill)) return {startSkill};
    set<string> visited;
    queue<string> q;
    q.push(startSkill);
    visited.insert(startSkill);
    while (!q.empty()) {
        string cur = q.front(); q.pop();
        order.push_back(cur);
        auto it = adj.find(cur);
        if (it == adj.end()) continue;
        for (const auto& next : it->second) {
            if (!visited.count(next)) {
                visited.insert(next);
                q.push(next);
            }
        }
    }
    return order;
}
SkillGraph buildDefaultSkillGraph() {
    SkillGraph g;
    g.addDependency("programming_basics", "python");
    g.addDependency("programming_basics", "c++");
    g.addDependency("programming_basics", "java");
    g.addDependency("programming_basics", "algorithms");
    g.addDependency("python",             "ml");
    g.addDependency("python",             "data_engineering");
    g.addDependency("python",             "deep_learning");
    g.addDependency("algorithms",         "system_design");
    g.addDependency("c++",                "game_dev");
    g.addDependency("c++",                "embedded");
    g.addDependency("c++",                "rtos");
    g.addDependency("java",               "android");
    g.addDependency("java",               "spring_boot");
    g.addDependency("html",               "css");
    g.addDependency("css",                "javascript");
    g.addDependency("javascript",         "react");
    g.addDependency("javascript",         "nodejs");
    g.addDependency("react",              "nextjs");
    g.addDependency("nodejs",             "api_design");
    g.addDependency("api_design",         "system_design");
    g.addDependency("statistics",         "ml");
    g.addDependency("ml",                 "deep_learning");
    g.addDependency("deep_learning",      "nlp");
    g.addDependency("deep_learning",      "computer_vision");
    g.addDependency("ml",                 "tensorflow");
    g.addDependency("sql",                "data_engineering");
    g.addDependency("data_engineering",   "spark");
    g.addDependency("spark",              "hadoop");
    g.addDependency("linux",              "docker");
    g.addDependency("docker",             "kubernetes");
    g.addDependency("kubernetes",         "aws");
    g.addDependency("networking",         "security");
    g.addDependency("security",           "ethical_hacking");
    g.addDependency("networking",         "firewall");
    g.addDependency("linux",              "ci_cd");
    g.addDependency("cryptography",       "ethereum");
    g.addDependency("ethereum",           "solidity");
    g.addDependency("solidity",           "smart_contracts");
    g.addDependency("smart_contracts",    "web3");
    g.addDependency("java",               "kotlin");
    g.addDependency("kotlin",             "android");
    return g;
}