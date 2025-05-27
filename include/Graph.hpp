#pragma once
#include "Student.hpp"
#include <unordered_map>
#include <optional>
using namespace std;
class Graph {
public:
    // Thêm / xoá sinh viên
    bool addStudent(const Student& s);
    bool removeStudent(const string& id);

    // Thêm / xoá quan hệ bạn bè (vô hướng)
    bool addFriendship(const string& a, const string& b);
    bool removeFriendship(const string& a, const string& b);

    optional<Student*>       getMutable(const string& id);
    optional<const Student*> get(const string& id) const;

    const unordered_map<string, Student>& data() const { return map_; }

    // Xuất đồ thị sang định dạng DOT để vẽ bằng Graphviz
    string toDot() const;
private:
    unordered_map<string, Student> map_; // id → Student
};