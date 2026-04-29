#ifndef ROADMAP_H
#define ROADMAP_H

#include "models.h"
#include <vector>

Roadmap buildRoadmap(const Career& career,
                     const std::vector<std::string>& userSkills);

#endif
