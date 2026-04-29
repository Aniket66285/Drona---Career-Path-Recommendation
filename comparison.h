#ifndef COMPARISON_H
#define COMPARISON_H
#include "models.h"
#include <vector>
#include <string>
CompareResult compareCareers(const MatchResult& a,
                             const MatchResult& b,
                             const Career& careerA,
                             const Career& careerB,
                             const UserProfile& user);

#endif
