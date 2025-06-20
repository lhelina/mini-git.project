#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

string getCommitIDFromBranch(const string& branchName) {
    ifstream branchFile(".minigit/" + branchName);
    string commitID;
    getline(branchFile, commitID);
    return commitID;
}

void checkout(const string& branchName) {
    string commitID = getCommitIDFromBranch(branchName);
    string commitPath = ".minigit/commits/" + commitID;

    if (!fs::exists(commitPath) || !fs::is_directory(commitPath)) {
        cout << "Commit path is not a directory: " << commitPath << endl;
        return;
    }

    cout << "Switched to branch '" << branchName << "' at commit " << commitID << endl;

    for (const auto& entry : fs::directory_iterator(commitPath)) {
        string fileName = entry.path().filename().string();

        try {
            // Check if file exists → remove it first
            if (fs::exists(fileName)) {
                fs::remove(fileName);
            }

            fs::copy(entry.path(), fileName);
        } catch (fs::filesystem_error& e) {
            cout << "Failed to copy " << fileName << ": " << e.what() << endl;
        }
    }

    // Update HEAD
    ofstream head(".minigit/HEAD");
    head << branchName;
    head.close();
}

int main() {
    string branch;
    cout << "Enter branch name to checkout: ";
    getline(cin, branch);
    checkout(branch);
    return 0;
}
