#include "DataTables.hpp"
#include <algorithm>
using namespace std;

int HobbyTable::index(const string& s) const {
    auto it = find(list.begin(), list.end(), s);
    return it == list.end() ? -1 : static_cast<int>(it - list.begin());
}

int HobbyTable::add(const string& s) {
    int idx = index(s);
    if (idx >= 0) return idx;
    list.push_back(s);
    return static_cast<int>(list.size()) - 1;
}

int HabitTable::index(const string& s) const {
    auto it = find(list.begin(), list.end(), s);
    return it == list.end() ? -1 : static_cast<int>(it - list.begin());
}

int HabitTable::add(const string& s) {
    int idx = index(s);
    if (idx >= 0) return idx;
    list.push_back(s);
    return static_cast<int>(list.size()) - 1;
}
