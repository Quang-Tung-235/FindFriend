#pragma once
#include <string>
#include <vector>
using namespace std;
struct HobbyTable {
    vector<string> list;
    int  index(const string& s) const;
    int  add  (const string& s);
};
struct HabitTable {
    vector<string> list;
    int  index(const string& s) const;
    int  add  (const string& s);
};
