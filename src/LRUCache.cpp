#include "../include/LRUCache.hpp"

using namespace std;

LRUCache::LRUCache(size_t cap) : capacity(cap), currentSize(0) {
    head = new LRUNode("");
    tail = new LRUNode("");
    head->next = tail;
    tail->prev = head;
}

LRUCache::~LRUCache() {
    clear();
    delete head;
    delete tail;
}

void LRUCache::addToHead(LRUNode* node) {
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
}

void LRUCache::removeNode(LRUNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUCache::moveToHead(LRUNode* node) {
    removeNode(node);
    addToHead(node);
}

// LRUNode* LRUCache::removeTail() {
//     LRUNode* lastNode = tail->prev;
//     removeNode(lastNode);
//     return lastNode;
// }

LRUNode* LRUCache::removeTail() {
    if (tail->prev == head) return nullptr;  // list empty
    LRUNode* lastNode = tail->prev;
    removeNode(lastNode);
    return lastNode;
}


// void LRUCache::access(const string& key) {
//     auto it = keyToNode.find(key);
    
//     if (it != keyToNode.end()) {
//         // Key exists, move to head
//         moveToHead(it->second);
//     } else {
//         // New key, add to head
//         LRUNode* newNode = new LRUNode(key);
//         keyToNode[key] = newNode;
//         addToHead(newNode);
//         currentSize++;
//     }
// }

void LRUCache::access(const string& key) {
    auto it = keyToNode.find(key);
    
    if (it != keyToNode.end()) {
        // Key exists, move to head
        moveToHead(it->second);
    } else {
        // If full, evict the least recently used node first
        if (currentSize >= capacity) {
            evictLRU();
        }
        // Now add new node
        LRUNode* newNode = new LRUNode(key);
        keyToNode[key] = newNode;
        addToHead(newNode);
        currentSize++;
    }
}


string LRUCache::evictLRU() {
    if (currentSize == 0) {
        return "";
    }
    
    LRUNode* lruNode = removeTail();
    string evictedKey = lruNode->key;
    keyToNode.erase(evictedKey);
    delete lruNode;
    currentSize--;
    
    return evictedKey;
}

void LRUCache::remove(const string& key) {
    auto it = keyToNode.find(key);
    if (it != keyToNode.end()) {
        LRUNode* node = it->second;
        removeNode(node);
        keyToNode.erase(key);
        delete node;
        currentSize--;
    }
}

void LRUCache::clear() {
    while (currentSize > 0) {
        evictLRU();
    }
}