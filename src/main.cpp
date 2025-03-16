#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::getline;
using std::string;
using std::vector;

string getFoil(string);
vector<string> getValues(string);
int nextStop(string, int);

int main() {
    // Get first file in "In" directory
    std::ifstream inFile;
    string inFileName;
    for (const auto& entry : std::filesystem::directory_iterator("IN")) {
        inFile = std::ifstream(entry.path());
        inFileName = entry.path().string();
        break;
    }
    if (!inFile.is_open()) return 1;
    
    string line;
    string output;
    
    // Skip first line
    getline(inFile, line);
    
    while (true) {
        // Get next line
        getline(inFile, line);
        
        // Skip last line and break
        if (inFile.eof()) break;
        
        // Get values and append to output
        vector<string> values = getValues(line);
        output +=
            values[0] + " "  +  // Count
            values[1] + " (" +  // Name
            values[2] + ") " +  // Set
            values[4] + " "  +  // Collector Number
            values[3] + "\n"    // Foil
        ;
    }
    
    // Delete input file
    inFile.close();
    std::filesystem::remove(inFileName.c_str());
    
    // Create output file
    std::ofstream outFile("OUT.txt");
    outFile << output;
}

string getFoil(string str) {
    if (str == "foil") return "*F*";
    if (str == "etched") return "*E*";
    return "";
}

vector<string> getValues(string line) {
    vector<string> values;
    
    // Count
    int end = nextStop(line, 1);
    values.push_back(line.substr(1, end - 1));
    
    // Name
    int start;
    end = nextStop(line, start = nextStop(line, end + 3) + 3);
    values.push_back(line.substr(start, end - start));
    
    // Set
    end = nextStop(line, start = end + 3);
    values.push_back(line.substr(start, end - start));
    
    // Foil
    end = nextStop(
        line,
        start = nextStop(line, nextStop(line, end + 3) + 3) + 3
    );
    values.push_back(getFoil(line.substr(start, end - start)));
    
    // Collector Number
    end = end = nextStop(
        line,
        start = nextStop(line, nextStop(line, end + 3) + 3) + 3
    );
    values.push_back(line.substr(start, end - start));
    
    return values;
}

int nextStop(string line, int start = 0) {
    return line.find("\",\"", start);
}
