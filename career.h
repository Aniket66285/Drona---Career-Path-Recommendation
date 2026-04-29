#ifndef CAREER_H
#define CAREER_H
#include "models.h"
#include <vector>
double scoreCareer(const std::vector<std::string>& userSkills,
                   const Career& career,
                   std::vector<std::string>& outMatched,
                   std::vector<std::string>& outMissing);

std::vector<MatchResult> getTop3(const UserProfile& user,
                                 const std::vector<Career>& careers);

#endif