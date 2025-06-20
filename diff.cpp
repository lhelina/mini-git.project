#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;

// Read file lines into a vector
vector<string> readLines(const string& path) {
    vector<string> lines;
    ifstream file(path);
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Show differences between two files line-by-line
void showDiff(const string& file1, const string& file2, const string& filename) {
    vector<string> lines1 = readLines(file1);
    vector<string> lines2 = readLines(file2);

    cout << "🔍 Differences in file: " << filename << endl;

    int maxLines = max(lines1.size(), lines2.size());
    for (int i = 0; i < maxLines; ++i) {
        string a = (i < lines1.size()) ? lines1[i] : "";
        string b = (i < lines2.size()) ? lines2[i] : "";

        if (a != b) {
            cout << "- " << a << endl;
            cout << "+ " << b << endl;
        }
    }
    cout << endl;
}

int main() {
    string commit1, commit2;
    cout << "Enter first commit ID: ";
    getline(cin, commit1);
    cout << "Enter second commit ID: ";
    getline(cin, commit2);

    string path1 = ".minigit/commits/" + commit1;
    string path2 = ".minigit/commits/" + commit2;

    if (!fs::exists(path1) || !fs::exists(path2)) {
        cout << "❌ One of the commit folders does not exist!" << endl;
        return 1;
    }

    // Compare all files in commit1 against commit2
    for (const auto& file : fs::directory_iterator(path1)) {
        string filename = file.path().filename().string();
        string filePath1 = path1 + "/" + filename;
        string filePath2 = path2 + "/" + filename;

        if (fs::exists(filePath2)) {
            showDiff(filePath1, filePath2, filename);
        } else {
            cout << "⚠️ File " << filename << " exists in commit1 but not in commit2.\n";
        }
    }

    // Now check if any extra files in commit2
    for (const auto& file : fs::directory_iterator(path2)) {
        string filename = file.path().filename().string();
        string filePath1 = path1 + "/" + filename;
        if (!fs::exists(filePath1)) {
            cout << "⚠️ File " << filename << " exists in commit2 but not in commit1.\n";
        }
    }

    return 0;
}
