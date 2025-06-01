#include "../include/Cache.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

Cache::Cache(size_t maxMem, size_t maxKeysLimit) 
    : maxMemoryBytes(maxMem), currentMemoryBytes(0), maxKeys(maxKeysLimit), totalOperations(0) {
    
    hashTable = new HashTable();
    lruCache = new LRUCache(maxKeys);
    ttlManager = new TTLManager();
    startTime = chrono::high_resolution_clock::now();
}

Cache::~Cache() {
    delete hashTable;
    delete lruCache;
    delete ttlManager;
}

void Cache::cleanupExpiredKeys() {
    long long currentTime = Utils::getCurrentTimestamp();
    vector<string> expiredKeys = hashTable->getExpiredKeys(currentTime);
    
    for (const string& key : expiredKeys) {
        string value;
        if (hashTable->get(key, value)) {
            currentMemoryBytes -= estimateKeyMemory(key, value);
        }
        hashTable->remove(key);
        lruCache->remove(key);
    }
    
    ttlManager->removeExpiredKeys(currentTime);
}

void Cache::evictIfNeeded() {
    while ((currentMemoryBytes > maxMemoryBytes || lruCache->isFull()) && lruCache->size() > 0) {
        string evictedKey = lruCache->evictLRU();
        if (!evictedKey.empty()) {
            string value;
            if (hashTable->get(evictedKey, value)) {
                currentMemoryBytes -= estimateKeyMemory(evictedKey, value);
            }
            hashTable->remove(evictedKey);
        }
    }
}

size_t Cache::estimateKeyMemory(const string& key, const string& value) const {
    return key.size() + value.size() + sizeof(HashNode) + sizeof(LRUNode);
}

bool Cache::set(const string& key, const string& value, int ttlSeconds) {
    totalOperations++;
    cleanupExpiredKeys();
    
    // Calculate memory needed
    size_t memoryNeeded = estimateKeyMemory(key, value);
    
    // Check if key already exists
    string oldValue;
    bool keyExists = hashTable->get(key, oldValue);
    if (keyExists) {
        currentMemoryBytes -= estimateKeyMemory(key, oldValue);
    }
    
    // Add memory for new value
    currentMemoryBytes += memoryNeeded;
    
    // Evict if necessary
    evictIfNeeded();
    
    // Set expiry time
    long long expiryTime = -1;
    if (ttlSeconds > 0) {
        expiryTime = Utils::getCurrentTimestamp() + ttlSeconds;
        ttlManager->addKey(key, expiryTime);
    }
    
    // Insert/update in hash table
    bool success = hashTable->insert(key, value, expiryTime);
    
    if (success) {
        // Update LRU
        lruCache->access(key);
        return true;
    }
    
    return false;
}

bool Cache::get(const string& key, string& value) {
    totalOperations++;
    cleanupExpiredKeys();
    
    if (hashTable->get(key, value)) {
        lruCache->access(key);
        return true;
    }
    
    return false;
}

bool Cache::del(const string& key) {
    totalOperations++;
    cleanupExpiredKeys();
    
    string value;
    if (hashTable->get(key, value)) {
        currentMemoryBytes -= estimateKeyMemory(key, value);
        hashTable->remove(key);
        lruCache->remove(key);
        return true;
    }
    
    return false;
}

bool Cache::exists(const string& key) {
    totalOperations++;
    cleanupExpiredKeys();
    return hashTable->exists(key);
}

bool Cache::expire(const string& key, int seconds) {
    totalOperations++;
    cleanupExpiredKeys();
    
    if (!hashTable->exists(key)) {
        return false;
    }
    
    long long expiryTime = Utils::getCurrentTimestamp() + seconds;
    ttlManager->addKey(key, expiryTime);
    return hashTable->updateExpiry(key, expiryTime);
}

void Cache::flush() {
    totalOperations++;
    
    hashTable->clear();
    lruCache->clear();
    ttlManager->clear();
    currentMemoryBytes = 0;
}

void Cache::showStats() const {
    cout << "\n=== CACHE STATISTICS ===" << endl;
    cout << "Total Keys: " << getKeyCount() << endl;
    cout << "Memory Usage: " << Utils::formatMemorySize(currentMemoryBytes) 
         << " / " << Utils::formatMemorySize(maxMemoryBytes) << endl;
    cout << "Memory Usage %: " << (double(currentMemoryBytes) / maxMemoryBytes * 100) << "%" << endl;
    cout << "Total Operations: " << totalOperations << endl;
    cout << "Operations/sec: " << getOpsPerSecond() << endl;
    cout << "LRU Cache Size: " << lruCache->size() << " / " << maxKeys << endl;
    cout << "TTL Entries: " << ttlManager->size() << endl;
    cout << "========================\n" << endl;
}

double Cache::getOpsPerSecond() const {
    auto currentTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(currentTime - startTime);
    double seconds = duration.count() / 1000.0;
    
    return seconds > 0 ? totalOperations / seconds : 0;
}

size_t Cache::getKeyCount() const {
    return hashTable->size();
}