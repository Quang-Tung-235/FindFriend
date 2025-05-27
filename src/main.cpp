#include "CLI.hpp"
#include "DataLoader.hpp"
using namespace std;

int main() {
    HobbyTable hb;
    HabitTable ht;

    DataLoader::loadHobbyTable("data/hobbyc.txt", hb);
    DataLoader::loadHabitTable("data/habitc.txt", ht);

    Graph g;
    DataLoader::loadStudents("data/student_info.txt", g, hb, ht);

    CLI cli(g, hb, ht);
    cli.run();

    return 0;
}
