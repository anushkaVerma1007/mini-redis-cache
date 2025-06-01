#include "../include/HashTable.hpp"
#include "../include/utils.hpp"
#include <functional>
#include <algorithm>

using namespace std;

const double HashTable::LOAD_FACTOR_THRESHOLD = 0.75;

HashTable::HashTable() : tableSize(INITIAL_SIZE), numElements(0) {
    table.resize(tableSize);
}

HashTable::~HashTable() {
    clear();
}

size_t HashTable::hash(const string& key) const {
    std::hash<std::string> hasher;
    return hasher(key) % tableSize;
}

void HashTable::resize() {
    vector<list<HashNode>> oldTable = move(table);
    size_t oldSize = tableSize;
    
    tableSize *= 2;
    table.clear();
    table.resize(tableSize);
    numElements = 0;
    
    // Rehash all elements
    for (const auto& bucket : oldTable) {
        for (const auto& node : bucket) {
            insert(node.key, node.value, node.expiryTime);
        }
    }
}

bool HashTable::insert(const string& key, const string& value, long long expiryTime) {
    // Check if key already exists and update
    size_t index = hash(key);
    auto& bucket = table[index];
    
    for (auto& node : bucket) {
        if (node.key == key) {
            node.value = value;
            node.expiryTime = expiryTime;
            return true;
        }
    }
    
    // Add new node
    bucket.emplace_back(key, value, expiryTime);
    numElements++;
    
    // Check if resize is needed
    if (static_cast<double>(numElements) / tableSize > LOAD_FACTOR_THRESHOLD) {
        resize();
    }
    
    return true;
}

bool HashTable::get(const string& key, string& value) const {
    size_t index = hash(key);
    const auto& bucket = table[index];
    long long currentTime = Utils::getCurrentTimestamp();
    
    for (const auto& node : bucket) {
        if (node.key == key) {
            // Check if expired
            if (node.expiryTime != -1 && currentTime > node.expiryTime) {
                return false;
            }
            value = node.value;
            return true;
        }
    }
    
    return false;
}

bool HashTable::remove(const string& key) {
    size_t index = hash(key);
    auto& bucket = table[index];
    
    auto it = find_if(bucket.begin(), bucket.end(),
        [&key](const HashNode& node) { return node.key == key; });
    
    if (it != bucket.end()) {
        bucket.erase(it);
        numElements--;
        return true;
    }
    
    return false;
}

bool HashTable::exists(const string& key) const {
    string dummy;
    return get(key, dummy);
}

bool HashTable::updateExpiry(const string& key, long long expiryTime) {
    size_t index = hash(key);
    auto& bucket = table[index];
    
    for (auto& node : bucket) {
        if (node.key == key) {
            // Check if not already expired
            long long currentTime = Utils::getCurrentTimestamp();
            if (node.expiryTime != -1 && currentTime > node.expiryTime) {
                return false;
            }
            node.expiryTime = expiryTime;
            return true;
        }
    }
    
    return false;
}

void HashTable::clear() {
    for (auto& bucket : table) {
        bucket.clear();
    }
    numElements = 0;
}

vector<string> HashTable::getAllKeys() const {
    vector<string> keys;
    long long currentTime = Utils::getCurrentTimestamp();
    
    for (const auto& bucket : table) {
        for (const auto& node : bucket) {
            // Only include non-expired keys
            if (node.expiryTime == -1 || currentTime <= node.expiryTime) {
                keys.push_back(node.key);
            }
        }
    }
    
    return keys;
}

vector<string> HashTable::getExpiredKeys(long long currentTime) const {
    vector<string> expiredKeys;
    
    for (const auto& bucket : table) {
        for (const auto& node : bucket) {
            if (node.expiryTime != -1 && currentTime > node.expiryTime) {
                expiredKeys.push_back(node.key);
            }
        }
    }
    
    return expiredKeys;
}