#pragma once
#include "Graph.hpp"
#include "Recommender.hpp"
#include "DataTables.hpp"
using namespace std;
/* CLI – Giao diện dòng lệnh đơn giản (menu chạy vòng lặp) */
class CLI {
public:
    CLI(Graph& g, HobbyTable& hb, HabitTable& ht);
    void run();
private:
    Graph& g_;
    HobbyTable& hbTbl_;
    HabitTable& htTbl_;
    Recommender recomm_;
    void menu();             // hiển thị menu + dispatch
    void addStudentFlow();   // nhập sinh viên mới
    void addFriendshipFlow();// nhập cặp ID để kết bạn
    void suggestFlow();      // gợi ý bạn bè
    void exportDotFlow();    // ghi graph.dot
    void listStudentsFlow();  
};