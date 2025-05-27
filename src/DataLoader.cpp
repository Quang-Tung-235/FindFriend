#include "DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <algorithm>
using namespace std;

namespace DataLoader {

// ─────────────── Tiện ích đọc tập số nguyên (hobby / habit) ────────────────
static void parseIntSet(const string& line, unordered_set<int>& out){
    istringstream ss(line); string tok;
    while (ss >> tok) {
        if (!tok.empty() && tok.back() == ',') tok.pop_back();
        if (tok.empty()) continue;
        out.insert(stoi(tok));
    }
}

// ─────────────── Nạp bảng hobby / habit ─────────────────────────────────────
void loadHobbyTable(const string& path, HobbyTable& tbl){
    ifstream in(path);
    if(!in){ cerr << "Cannot open "<<path<<""; return; }
    string line; while(getline(in,line)) if(!line.empty()) tbl.add(line);
}
void loadHabitTable(const string& path, HabitTable& tbl){
    ifstream in(path);
    if(!in){ cerr << "Cannot open "<<path<<""; return; }
    string line; while(getline(in,line)) if(!line.empty()) tbl.add(line);
}

// ─────────────── Cấu trúc tạm giữ dữ liệu sinh viên ────────────────────────
struct RawStudent {
    string id, name;
    vector<string> friendTok;          // giữ nguyên token dòng 3
    unordered_set<int> hobbies, habits;
};

// ─────────────── Hàm chính nạp sinh viên (hai lượt) ─────────────────────────
void loadStudents(const string& path, Graph& g,
                  const HobbyTable&, const HabitTable&){
    ifstream in(path);
    if(!in){ cerr << "Cannot open "<<path<<""; return; }

    vector<RawStudent> raw;            // bộ đệm toàn file
    string line;

    // ---------- LƯỢT 1: đọc toàn bộ file vào raw ----------------------------
    while (getline(in, line)) {
        if (line.empty()) continue;
        RawStudent rs; rs.id = line;                 // dòng 1: ID
        getline(in, rs.name);                         // dòng 2: Tên

        // dòng 3: danh sách bạn (token phân cách bởi khoảng trắng hoặc phẩy)
        getline(in, line);
        istringstream fss(line); string tok;
        while (fss >> tok) {
            if (!tok.empty() && tok.back() == ',') tok.pop_back();
            if (!tok.empty()) rs.friendTok.push_back(tok);
        }

        // dòng 4: hobbies (index)
        getline(in, line); parseIntSet(line, rs.hobbies);
        // dòng 5: habits  (index)
        getline(in, line); parseIntSet(line, rs.habits);

        raw.push_back(move(rs));
    }

    // ---------- Chuẩn bị ánh xạ --------------------------------------------
    vector<string> idx2id; idx2id.reserve(raw.size());
    for (auto& rs : raw) idx2id.push_back(rs.id);   // index → ID

    unordered_map<string,string> short2id; // 2‑3 số cuối → ID
    for (const auto& id : idx2id) {
        if (id.size() >= 2) short2id[id.substr(id.size() - 2)] = id;
        if (id.size() >= 3) short2id[id.substr(id.size() - 3)] = id;
    }

    // ---------- LƯỢT 2: chuyển token bạn bè → ID thật & thêm vào Graph ------
    for (auto& rs : raw) {
        Student s; s.id = rs.id; s.name = rs.name;
        s.hobbies.swap(rs.hobbies);
        s.habits.swap(rs.habits);

        for (const auto& t : rs.friendTok) {
            // 1) Nếu token toàn chữ số
            bool allDigit = !t.empty() && all_of(t.begin(), t.end(), ::isdigit);
            if (allDigit) {
                long val = stol(t);
                // a) token == chỉ‑số 0..N‑1
                if (val >= 0 && val < (long)idx2id.size()) { s.friends.insert(idx2id[val]); continue; }
                // b) token dài 8 ký số → ID đầy đủ
                if (t.size() == 8) { s.friends.insert(t); continue; }
                // c) token 2‑3 ký số → tra short2id
                auto itS = short2id.find(t);
                if (itS != short2id.end()) { s.friends.insert(itS->second); continue; }
            }
            // 2) Trường hợp khác (chuỗi alphanum) – coi như ID thẳng
            s.friends.insert(t);
        }
        g.addStudent(s);
    }
}

} // namespace DataLoader
