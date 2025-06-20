#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// Function to read and print commit info from a given commit ID
void printLogHistory(const string& commitID) {
    string commitPath = ".minigit/commits/" + commitID + "/.meta";
    ifstream meta(commitPath);

    if (!meta) {
        cout << "Could not open meta file for commit " << commitID << endl;
        return;
    }

    string line, message, timestamp, parent;
    while (getline(meta, line)) {
        if (line.rfind("Message:", 0) == 0)
            message = line.substr(8);
        else if (line.rfind("Timestamp:", 0) == 0)
            timestamp = line.substr(10);
        else if (line.rfind("Parent:", 0) == 0) {
            parent = line.substr(7);

            // 🔧 Trim any leading/trailing whitespace from the parent commit ID
            parent.erase(0, parent.find_first_not_of(" \t\n\r"));
            parent.erase(parent.find_last_not_of(" \t\n\r") + 1);
        }
    }

    // Print this commit’s log entry
    cout << "----- Commit History -----" << endl;
    cout << "Commit:  " << commitID << endl;
    cout << "Message: " << message << endl;
    cout << "----------------------------" << endl;

    // Recursively print the parent commit, if it exists
    if (!parent.empty())
        printLogHistory(parent);
}

int main() {
    // Read the current HEAD commit ID
    ifstream head(".minigit/HEAD");
    if (!head) {
        cout << "HEAD file not found." << endl;
        return 1;
    }

    string branchName;
    getline(head, branchName);
    head.close();

    ifstream branch(".minigit/" + branchName);
    if (!branch) {
        cout << "Branch file not found." << endl;
        return 1;
    }

    string currentCommitID;
    getline(branch, currentCommitID);
    branch.close();

    // Start printing history from HEAD commit
    printLogHistory(currentCommitID);

    return 0;
}
