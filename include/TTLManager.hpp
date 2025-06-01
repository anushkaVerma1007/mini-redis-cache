#ifndef TTLMANAGER_HPP
#define TTLMANAGER_HPP

#include <string>
#include <queue>
#include <vector>

using namespace std;

struct TTLEntry {
    string key;
    long long expiryTime;
    
    TTLEntry(const string& k, long long exp) : key(k), expiryTime(exp) {}
    
    // For min-heap (priority queue)
    bool operator>(const TTLEntry& other) const {
        return expiryTime > other.expiryTime;
    }
};

class TTLManager {
private:
    priority_queue<TTLEntry, vector<TTLEntry>, greater<TTLEntry>> minHeap;
    
public:
    TTLManager();
    ~TTLManager();
    
    void addKey(const string& key, long long expiryTime);
    vector<string> getExpiredKeys(long long currentTime);
    void removeExpiredKeys(long long currentTime);
    
    size_t size() const { return minHeap.size(); }
    bool empty() const { return minHeap.empty(); }
    void clear();
};

#endif