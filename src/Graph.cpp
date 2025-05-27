#include "Graph.hpp"
#include <sstream>
#include <unordered_set>
using namespace std;

bool Graph::addStudent(const Student& s) {
    return map_.emplace(s.id, s).second; // false nếu trùng id
}

bool Graph::removeStudent(const string& id) {
    if (map_.erase(id) == 0) return false;
    for (auto& [_, st] : map_)
        st.friends.erase(id); // xoá cạnh liên quan
    return true;
}

bool Graph::addFriendship(const string& a, const string& b) {
    auto itA = map_.find(a); auto itB = map_.find(b);
    if (itA == map_.end() || itB == map_.end() || a == b) return false;
    itA->second.friends.insert(b);
    itB->second.friends.insert(a);
    return true;
}

bool Graph::removeFriendship(const string& a, const string& b) {
    auto itA = map_.find(a); auto itB = map_.find(b);
    if (itA == map_.end() || itB == map_.end()) return false;
    itA->second.friends.erase(b);
    itB->second.friends.erase(a);
    return true;
}

optional<Student*> Graph::getMutable(const string& id) {
    auto it = map_.find(id);
    return it == map_.end() ? nullopt : optional<Student*>{ &it->second };
}

optional<const Student*> Graph::get(const string& id) const {
    auto it = map_.find(id);
    return it == map_.end() ? nullopt : optional<const Student*>{ &it->second };
}

// Xuất định dạng DOT cho Graphviz
string Graph::toDot() const {
    ostringstream oss;
    oss << "graph G {\n  overlap=false;\n  splines=true;\n";
    for (auto& [id, st] : map_)
        oss << "  \"" << id << "\" [label=\"" << id << "\"];\n";

    unordered_set<string> seen;
    for (auto& [id, st] : map_) {
        for (auto& f : st.friends) {
            string key = id < f ? id + "_" + f : f + "_" + id;
            if (seen.insert(key).second)
                oss << "  \"" << id << "\" -- \"" << f << "\";\n";
        }
    }
    oss << "}\n";
    return oss.str();
}
