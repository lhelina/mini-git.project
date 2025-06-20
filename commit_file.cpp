#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <ctime>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string generateCommitID() {
    time_t now = time(0);
    return to_string(now);
}

string getCurrentBranch() {
    ifstream head(".minigit/HEAD");
    string branch;
    getline(head, branch);
    head.close();
    return branch;
}

vector<string> getStagedFiles() {
    vector<string> files;
    ifstream stage(".minigit/stage");
    string file;
    while (getline(stage, file)) {
        if (!file.empty()) {
            files.push_back(file);
        }
    }
    stage.close();
    return files;
}

void commit(string message) {
    vector<string> stagedFiles = getStagedFiles();
    if (stagedFiles.empty()) {
        cout << "No staged files. Add files before committing." << endl;
        return;
    }

    string commitID = generateCommitID();
    string commitPath = ".minigit/commits/" + commitID;
    fs::create_directories(commitPath); // This ensures it's a folder

    string branch = getCurrentBranch();
    string branchPath = ".minigit/" + branch;

    string parentID = "";
    ifstream branchFile(branchPath);
    if (branchFile.is_open()) {
        getline(branchFile, parentID);
        branchFile.close();
    }

    for (const string& file : stagedFiles) {
        if (fs::exists(file)) {
            fs::copy(file, commitPath + "/" + file, fs::copy_options::overwrite_existing);
        }
    }

    ofstream metaFile(commitPath + "/.meta");
    metaFile << "Message: " << message << endl;
    metaFile << "Timestamp: " << time(0) << endl;
    metaFile << "Parent: " << parentID << endl;
    metaFile << "Branch: " << branch << endl;
    metaFile.close();

    ofstream branchOut(branchPath);
    branchOut << commitID;
    branchOut.close();

    cout << "Commit saved with ID: " << commitID << endl;

    ofstream clearStage(".minigit/stage");
    clearStage.close();
}

int main() {
    string msg;
    cout << "Enter commit message: ";
    getline(cin, msg);
    commit(msg);
    return 0;
}
