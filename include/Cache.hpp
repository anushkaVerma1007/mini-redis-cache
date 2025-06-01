#ifndef CACHE_HPP
#define CACHE_HPP

#include "HashTable.hpp"
#include "LRUCache.hpp"
#include "TTLManager.hpp"
#include <string>
#include <chrono>

using namespace std;

class Cache {
private:
    HashTable* hashTable;
    LRUCache* lruCache;
    TTLManager* ttlManager;
    
    size_t maxMemoryBytes;
    size_t currentMemoryBytes;
    size_t maxKeys;
    
    // Performance metrics
    long long totalOperations;
    chrono::high_resolution_clock::time_point startTime;
    
    void cleanupExpiredKeys();
    void evictIfNeeded();
    size_t estimateKeyMemory(const string& key, const string& value) const;
    
public:
    Cache(size_t maxMem = 1024 * 1024 * 100, size_t maxKeysLimit = 10000); // 100MB default
    ~Cache();
    
    // Core Redis-like operations
    bool set(const string& key, const string& value, int ttlSeconds = -1);
    bool get(const string& key, string& value);
    bool del(const string& key);
    bool exists(const string& key);
    bool expire(const string& key, int seconds);
    void flush();
    
    // Status and metrics
    void showStats() const;
    double getOpsPerSecond() const;
    size_t getMemoryUsage() const { return currentMemoryBytes; }
    size_t getKeyCount() const;
};

#endif