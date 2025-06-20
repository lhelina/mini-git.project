#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <functional> // for std::hash

namespace fs = std::filesystem;
using namespace std;

string getFileHash(const string& content) {
    hash<string> hasher;
    size_t hashed = hasher(content);
    return to_string(hashed);  // converts number to string
}

void addFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: File not found!" << endl;
        return;
    }

    // Read the entire file content
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Hash the content
    string hash = getFileHash(content);

    // Save the file content to .minigit/objects/<hash>
    fs::path objectPath = ".minigit/objects/" + hash;
    ofstream objectFile(objectPath);
    objectFile << content;
    objectFile.close();

    // Append to the staging area
    ofstream stage(".minigit/stage", ios::app);  // open in append mode
    stage << filename << " " << hash << endl;
    stage.close();

    cout << "File '" << filename << "' added and staged with hash: " << hash << endl;
}

int main() {
    string filename;
    cout << "Enter file to add: ";
    cin >> filename;
    addFile(filename);
    return 0;
}

