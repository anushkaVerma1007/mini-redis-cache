#include "../include/TTLManager.hpp"

using namespace std;

TTLManager::TTLManager() {}

TTLManager::~TTLManager() {
    clear();
}

void TTLManager::addKey(const string& key, long long expiryTime) {
    minHeap.emplace(key, expiryTime);
}

vector<string> TTLManager::getExpiredKeys(long long currentTime) {
    vector<string> expiredKeys;
    
    // Create a temporary copy to check expired keys without modifying original
    auto tempHeap = minHeap;
    
    while (!tempHeap.empty() && tempHeap.top().expiryTime <= currentTime) {
        expiredKeys.push_back(tempHeap.top().key);
        tempHeap.pop();
    }
    
    return expiredKeys;
}

void TTLManager::removeExpiredKeys(long long currentTime) {
    // Create new heap without expired keys
    priority_queue<TTLEntry, vector<TTLEntry>, greater<TTLEntry>> newHeap;
    
    while (!minHeap.empty()) {
        TTLEntry entry = minHeap.top();
        minHeap.pop();
        
        if (entry.expiryTime > currentTime) {
            newHeap.push(entry);
        }
    }
    
    minHeap = move(newHeap);
}

void TTLManager::clear() {
    while (!minHeap.empty()) {
        minHeap.pop();
    }
}