#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Get the commit ID from a given branch
string getCommitFromBranch(const string& branchName) {
    ifstream branch(".minigit/" + branchName);
    string commitID;
    getline(branch, commitID);
    return commitID;
}

// Simulate a three-way merge (very simplified)
void mergeCommits(const string& branchName) {
    string currentCommit = getCommitFromBranch("HEAD");
    string mergeCommit = getCommitFromBranch(branchName);

    if (mergeCommit.empty()) {
        cout << "Branch '" << branchName << "' not found or empty." << endl;
        return;
    }

    string mergePath = ".minigit/commits/" + mergeCommit;

    if (!fs::exists(mergePath)) {
        cout << "Commit path " << mergePath << " does not exist." << endl;
        return;
    }

    cout << "Merging files from branch: " << branchName << endl;

    for (const auto& entry : fs::directory_iterator(mergePath)) {
        string filename = entry.path().filename().string();

        fs::path dest = filename;

        // Only try to copy regular files, not folders or other things
        if (fs::is_regular_file(entry)) {
            try {
                fs::copy_file(entry.path(), dest, fs::copy_options::overwrite_existing);
                cout << "Merged new file: " << filename << endl;
            } catch (const fs::filesystem_error& e) {
                cout << "CONFLICT or error: " << filename << " → " << e.what() << endl;
            }
        }
    }

    cout << "Merge completed." << endl;
}

int main() {
    string branchName;
    cout << "Enter branch to merge into current: ";
    getline(cin, branchName);

    mergeCommits(branchName);
    return 0;
}

