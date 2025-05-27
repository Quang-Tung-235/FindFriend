#include "CLI.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

CLI::CLI(Graph& g, HobbyTable& hb, HabitTable& ht)
    : g_(g), hbTbl_(hb), htTbl_(ht), recomm_(g) {}

void CLI::run() {
    while (true) menu();
}

void CLI::menu() {
    cout << "\n=== MENU ===\n"
         << "1) Thêm sinh viên\n"
         << "2) Kết bạn\n"
         << "3) Gợi ý bạn bè\n"
         << "4) Xuất DOT\n"
         << "5) Liệt kê sinh viên\n"
         << "0) Thoát\n"
         << "Lựa chọn: ";
    int c; cin >> c;
    switch (c) {
        case 1: addStudentFlow(); break;
        case 2: addFriendshipFlow(); break;
        case 3: suggestFlow(); break;
        case 4: exportDotFlow(); break;
        case 5: listStudentsFlow(); break;
        case 0: exit(0);
        default: cout << "Sai lựa chọn!\n";
    }
}

void CLI::addStudentFlow() {
    Student s;
    cout << "ID: ";   cin >> s.id;
    cout << "Ten: ";  cin >> s.name;

    string buf;
    cout << "So thich (phan tach ,): ";
    cin >> buf;
    istringstream hs(buf);
    string tok;
    while (getline(hs, tok, ',')) 
        if (!tok.empty()) s.hobbies.insert(hbTbl_.add(tok));

    cout << "Thoi quen (phan tach ,): ";
    cin >> buf;
    istringstream hts(buf);
    while (getline(hts, tok, ',')) 
        if (!tok.empty()) s.habits.insert(htTbl_.add(tok));

    if (!g_.addStudent(s)) 
        cout << "Trung ID!\n";
}

void CLI::addFriendshipFlow() {
    string a, b;
    cout << "ID A: "; cin >> a;
    cout << "ID B: "; cin >> b;
    if (!g_.addFriendship(a, b)) 
        cout << "Khong thanh cong (ID sai?)\n";
}

void CLI::suggestFlow() {
    string id; 
    cout << "Nhap ID: "; 
    cin >> id;
    auto list = recomm_.suggest(id, 5);
    for (auto& c : list)
        cout << " -> " << c.id << " (score " << c.score << ")\n";
}

void CLI::exportDotFlow() {
    ofstream("graph.dot") << g_.toDot();
    cout << "Đa xuat graph.dot (dot -Tpng graph.dot -o graph.png)\n";
}

void CLI::listStudentsFlow() {
    cout << "===== DANH SÁCH SINH VIÊN =====\n";
    cout << "ID | Tên | Sở thích | Thói quen | Bạn bè (ID - Tên)\n";
    cout << "--------------------------------------------------------------------------\n";

    for (const auto& [id, st] : g_.data()) {
        cout << id << " | " << st.name << " | ";
        // Sở thích
        bool first = true;
        for (int idx : st.hobbies) { 
            if (!first) cout << ','; 
            first = false; 
            if (idx < (int)hbTbl_.list.size()) 
                cout << hbTbl_.list[idx]; 
        }
        cout << " | ";
        // Thói quen
        first = true;
        for (int idx : st.habits) { 
            if (!first) cout << ','; 
            first = false; 
            if (idx < (int)htTbl_.list.size()) 
                cout << htTbl_.list[idx]; 
        }
        cout << " | ";
        // Bạn bè: xử lý chuỗi có thể chứa khoảng trắng hoặc dấu phẩy
        first = true;
        for (const auto& fidRaw : st.friends) {
            istringstream ss(fidRaw);
            string tok;
            while (ss >> tok) {  // tách theo whitespace
                if (!tok.empty() && tok.back() == ',') tok.pop_back();
                if (tok.empty()) continue;
                auto fOpt = g_.get(tok);
                string fname = fOpt ? ("(" + (*fOpt)->name + ")") : "";
                if (!first) cout << ',';
                first = false;
                cout << tok << fname;
            }
        }
        cout << "\n";
    }
}
