#include "../include/utils.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

long long Utils::getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto timestamp = chrono::duration_cast<chrono::seconds>(now.time_since_epoch());
    return timestamp.count();
}

vector<string> Utils::splitString(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

void Utils::logMessage(const string& message) {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    cout << "[" << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] " << message << endl;
}

string Utils::formatMemorySize(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double size = bytes;
    
    while (size >= 1024 && unit < 3) {
        size /= 1024;
        unit++;
    }
    
    stringstream ss;
    ss << fixed << setprecision(2) << size << " " << units[unit];
    return ss.str();
}