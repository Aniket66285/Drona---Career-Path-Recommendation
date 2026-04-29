#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>

class SkillGraph {
public:

    void addDependency(const std::string& learnFirst,
                       const std::string& learnSecond);

    std::vector<std::string> learningOrder(const std::string& startSkill) const;

    std::vector<std::string> prereqs(const std::string& skill) const;

    bool hasSkill(const std::string& skill) const;

    void addSkill(const std::string& skill);

private:

    std::map<std::string, std::vector<std::string>> adj;

    std::map<std::string, std::vector<std::string>> rev;
};

SkillGraph buildDefaultSkillGraph();

#endif
