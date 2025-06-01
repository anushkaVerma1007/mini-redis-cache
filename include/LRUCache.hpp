#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include <string>
#include <unordered_map>

using namespace std;

struct LRUNode {
    string key;
    LRUNode* prev;
    LRUNode* next;
    
    LRUNode(const string& k) : key(k), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<string, LRUNode*> keyToNode;
    LRUNode* head;
    LRUNode* tail;
    size_t capacity;
    size_t currentSize;
    
    void addToHead(LRUNode* node);
    void removeNode(LRUNode* node);
    void moveToHead(LRUNode* node);
    LRUNode* removeTail();
    
public:
    LRUCache(size_t cap);
    ~LRUCache();
    
    void access(const string& key);
    string evictLRU();
    void remove(const string& key);
    void clear();
    
    size_t size() const { return currentSize; }
    bool isFull() const { return currentSize >= capacity; }
    void setCapacity(size_t cap) { capacity = cap; }
};

#endif