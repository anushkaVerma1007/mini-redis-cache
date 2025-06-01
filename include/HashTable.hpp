#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>
#include <vector>
#include <list>

using namespace std;

struct HashNode {
    string key;
    string value;
    long long expiryTime;
    
    HashNode(const string& k, const string& v, long long exp = -1) 
        : key(k), value(v), expiryTime(exp) {}
};

class HashTable {
private:
    vector<list<HashNode>> table;
    size_t tableSize;
    size_t numElements;
    static const size_t INITIAL_SIZE = 16;
    static const double LOAD_FACTOR_THRESHOLD;
    
    size_t hash(const string& key) const;
    void resize();
    
public:
    HashTable();
    ~HashTable();
    
    bool insert(const string& key, const string& value, long long expiryTime = -1);
    bool get(const string& key, string& value) const;
    bool remove(const string& key);
    bool exists(const string& key) const;
    bool updateExpiry(const string& key, long long expiryTime);
    void clear();
    
    size_t size() const { return numElements; }
    vector<string> getAllKeys() const;
    vector<string> getExpiredKeys(long long currentTime) const;
};

#endif