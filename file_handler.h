#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include "models.h"
#include <string>
#include <vector>
std::vector<Career> loadCareers(const std::string& filename);
const Career* findCareer(const std::string& name,
                         const std::vector<Career>& careers);
std::vector<std::string> careerNames(const std::vector<Career>& careers);

#endif
