
#pragma once
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;
// ─────────────────────────────────────────────────────────────
//  Cấu trúc Sinh Viên (đỉnh của đồ thị)
//  - id      : mã sinh viên (duy nhất)
//  - name    : tên hiển thị
//  - hobbies : tập chỉ mục sở thích (tham chiếu HobbyTable)
//  - habits  : tập chỉ mục thói quen (HabitTable)
//  - friends : tập ID bạn bè (cạnh vô hướng)
// ─────────────────────────────────────────────────────────────
struct Student {
    string id;
    string name;
    unordered_set<int> hobbies;
    unordered_set<int> habits;
    unordered_set<string> friends;

    bool isFriend(const string& other) const { return friends.count(other); }
};