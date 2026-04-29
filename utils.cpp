#include "utils.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <vector>

using namespace std;

string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

string toLower(const string& s) {
    string r = s;
    transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

bool isLikelyName(const string& s) {
    if (s.size() < 2 || s.size() > 60) return false;
    for (char c : s)
        if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') return false;
    return true;
}

int editDistance(const string& a, const string& b) {
    int m = (int)a.size(), n = (int)b.size();
    vector<vector<int>> dp(m+1, vector<int>(n+1));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            dp[i][j] = (a[i-1]==b[j-1])
                ? dp[i-1][j-1]
                : 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
    return dp[m][n];
}

string findClosestCareer(const string& input, const vector<string>& names) {
    string lo = toLower(input);
    int best = INT_MAX;
    string result = names.empty() ? input : names[0];
    for (const auto& n : names) {
        int d = editDistance(lo, toLower(n));
        if (d < best) { best = d; result = n; }
    }
    return result;
}

string repeat(char c, int n) {
    return string(n, c);
}

string gradeOf(double pct) {
    if (pct >= 80) return "Excellent";
    if (pct >= 50) return "Good";
    return "Gap";
}