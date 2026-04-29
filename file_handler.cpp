#include "file_handler.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
vector<Career> loadCareers(const string& filename) {
    vector<Career> careers;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[ERROR] Cannot open '" << filename << "'\n";
        return careers;
    }
    string line;
    int lineNo = 0;
    while (getline(file, line)) {
        lineNo++;
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        string namePart, skillsPart, catPart, descPart;
        if (!getline(ss, namePart,   '|')) continue;
        if (!getline(ss, skillsPart, '|')) continue;
        if (!getline(ss, catPart,    '|')) continue;
        getline(ss, descPart, '|');
        Career c;
        c.name        = trim(namePart);
        c.category    = trim(catPart);
        c.description = trim(descPart);
        stringstream sk(skillsPart);
        string seg;
        while (getline(sk, seg, ',')) {
            seg = trim(seg);
            size_t pos = seg.find(':');
            if (pos == string::npos) continue;
            string sname  = toLower(trim(seg.substr(0, pos)));
            string sweigh = trim(seg.substr(pos + 1));
            if (sname.empty() || sweigh.empty()) continue;
            try {
                double w = stod(sweigh);
                c.skills.push_back(sname);
                c.skillWeights[sname] = w;
            } catch (...) {
                cerr << "[WARN] Bad weight at line " << lineNo << ": " << seg << "\n";
            }
        }
        if (!c.name.empty() && !c.skills.empty())
            careers.push_back(c);
    }
    file.close();
    return careers;
}
const Career* findCareer(const string& name, const vector<Career>& careers) {
    for (const auto& c : careers)
        if (toLower(c.name) == toLower(name))
            return &c;
    return nullptr;
}
vector<string> careerNames(const vector<Career>& careers) {
    vector<string> names;
    names.reserve(careers.size());
    for (const auto& c : careers) names.push_back(c.name);
    return names;
}