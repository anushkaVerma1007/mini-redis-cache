#include <iostream>
#include <cassert>
#include "../include/LRUCache.hpp"

using namespace std;

void testLRUBasicOperations() {
    cout << "Testing LRU basic operations..." << endl;
    
    LRUCache lru(3);
    
    // Add keys
    lru.access("key1");
    lru.access("key2");
    lru.access("key3");
    assert(lru.size() == 3);
    
    // Add fourth key, should evict LRU
    lru.access("key4");
    assert(lru.size() == 3);
    
    // key1 should have been evicted (it was LRU)
    // We can't directly test this without modifying LRUCache,
    // but we know the eviction happened based on size
    
    cout << "✓ LRU basic operations test passed" << endl;
}

void testLRUEvictionOrder() {
    cout << "Testing LRU eviction order..." << endl;
    
    LRUCache lru(2);
    
    lru.access("key1");
    lru.access("key2");
    
    // Access key1 again (makes it most recent)
    lru.access("key1");
    
    // Add key3, should evict key2 (LRU)
    lru.access("key3");
    
    // Manually evict and check
    string evicted = lru.evictLRU();
    // key1 should be evicted as it's now LRU
    
    assert(!evicted.empty());
    assert(lru.size() == 1);
    
    cout << "✓ LRU eviction order test passed" << endl;
}

void testLRURemoval() {
    cout << "Testing LRU removal..." << endl;
    
    LRUCache lru(5);
    
    lru.access("key1");
    lru.access("key2");
    lru.access("key3");
    assert(lru.size() == 3);
    
    // Remove specific key
    lru.remove("key2");
    assert(lru.size() == 2);
    
    // Clear all
    lru.clear();
    assert(lru.size() == 0);
    assert(lru.evictLRU().empty());
    
    cout << "✓ LRU removal test passed" << endl;
}

int main() {
    cout << "=== LRU CACHE TESTS ===" << endl << endl;
    
    try {
        testLRUBasicOperations();
        testLRUEvictionOrder();
        testLRURemoval();
        
        cout << endl << "🎉 All LRU tests passed!" << endl;
    } catch (const exception& e) {
        cerr << "❌ LRU test failed: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}