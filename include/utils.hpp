#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <string>
#include <vector>

using namespace std;

class Utils {
public:
    static long long getCurrentTimestamp();
    static vector<string> splitString(const string& str, char delimiter);
    static void logMessage(const string& message);
    static string formatMemorySize(size_t bytes);
};

#endif
