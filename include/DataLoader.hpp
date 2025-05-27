#pragma once
#include "Graph.hpp"
#include "DataTables.hpp"
#include <string>
using namespace std;
namespace DataLoader {
    void loadHobbyTable(const string& path, HobbyTable& tbl);    // nạp hobbyc.txt
    void loadHabitTable(const string& path, HabitTable& tbl);    // nạp habitc.txt
    void loadStudents(const string& path, Graph& g,             // nạp student_info.txt
                      const HobbyTable& htbl, const HabitTable& habtbl);
    void saveStudents(const string& path, const Graph& g,
                      const HobbyTable& htbl, const HabitTable& habtbl);
}