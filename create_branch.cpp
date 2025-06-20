#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

string getCurrentBranch() {
    ifstream head(".minigit/HEAD");
    string branch;
    getline(head, branch);
    return branch;
}

string getCurrentCommitID(const string& branchName) {
    ifstream branchFile(".minigit/" + branchName);
    string commitID;
    getline(branchFile, commitID);
    return commitID;
}

void createBranch(const string& newBranch) {
    string currentBranch = getCurrentBranch();
    if (currentBranch.empty()) {
        cout << "HEAD file is empty or missing.\n";
        return;
    }

    string currentCommitID = getCurrentCommitID(currentBranch);
    if (currentCommitID.empty()) {
        cout << "Current commit ID is missing or empty.\n";
        return;
    }

    ofstream newBranchFile(".minigit/" + newBranch);
    newBranchFile << currentCommitID;
    newBranchFile.close();

    cout << "Created branch '" << newBranch << "' at commit " << currentCommitID << endl;
}

int main() {
    string newBranch;
    cout << "Enter new branch name: ";
    getline(cin, newBranch);
    createBranch(newBranch);
    return 0;
}


