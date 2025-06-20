#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = filesystem;

void init() {
    if (!fs::exists(".minigit")) {
        fs::create_directory(".minigit");
        fs::create_directory(".minigit/objects");
        fs::create_directory(".minigit/commits");

        ofstream head(".minigit/HEAD");
        head << "master";
        head.close();

        cout << "Initialized empty MiniGit repository in .minigit/" << endl;
    } else {
        cout << "MiniGit repository already initialized." << endl;
    }
}

int main() {
    init();
    return 0;
}
