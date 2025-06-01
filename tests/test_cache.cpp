#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "../include/Cache.hpp"

using namespace std;

void testBasicOperations() {
    cout << "Testing basic operations..." << endl;
    
    Cache cache;
    string value;
    
    // Test SET and GET
    assert(cache.set("key1", "value1"));
    assert(cache.get("key1", value));
    assert(value == "value1");
    
    // Test non-existent key
    assert(!cache.get("nonexistent", value));
    
    // Test EXISTS
    assert(cache.exists("key1"));
    assert(!cache.exists("nonexistent"));
    
    // Test DELETE
    assert(cache.del("key1"));
    assert(!cache.exists("key1"));
    assert(!cache.del("key1")); // Already deleted
    
    cout << "✓ Basic operations test passed" << endl;
}

void testTTLFunctionality() {
    cout << "Testing TTL functionality..." << endl;
    
    Cache cache;
    string value;
    
    // Set key with 2 second TTL
    assert(cache.set("ttl_key", "ttl_value", 2));
    assert(cache.get("ttl_key", value));
    assert(value == "ttl_value");
    
    // Wait for expiration
    this_thread::sleep_for(chrono::seconds(3));
    
    // Key should be expired
    assert(!cache.get("ttl_key", value));
    assert(!cache.exists("ttl_key"));
    
    cout << "✓ TTL functionality test passed" << endl;
}

void testExpireCommand() {
    cout << "Testing EXPIRE command..." << endl;
    
    Cache cache;
    string value;
    
    // Set key without TTL
    assert(cache.set("expire_key", "expire_value"));
    assert(cache.exists("expire_key"));
    
    // Set expiration
    assert(cache.expire("expire_key", 2));
    
    // Key should still exist
    assert(cache.get("expire_key", value));
    assert(value == "expire_value");
    
    // Wait for expiration
    this_thread::sleep_for(chrono::seconds(3));
    
    // Key should be expired
    assert(!cache.get("expire_key", value));
    
    cout << "✓ EXPIRE command test passed" << endl;
}

void testFlushOperation() {
    cout << "Testing FLUSH operation..." << endl;
    
    Cache cache;
    
    // Add multiple keys
    assert(cache.set("key1", "value1"));
    assert(cache.set("key2", "value2"));
    assert(cache.set("key3", "value3"));
    
    // Verify they exist
    assert(cache.exists("key1"));
    assert(cache.exists("key2"));
    assert(cache.exists("key3"));
    
    // Flush cache
    cache.flush();
    
    // Verify all keys are gone
    assert(!cache.exists("key1"));
    assert(!cache.exists("key2"));
    assert(!cache.exists("key3"));
    
    cout << "✓ FLUSH operation test passed" << endl;
}

void testMemoryEviction() {
    cout << "Testing memory eviction..." << endl;
    
    // Create cache with very small memory limit
    Cache cache(1024, 5); // 1KB memory, 5 keys max
    
    // Fill cache beyond capacity
    for (int i = 0; i < 10; i++) {
        string key = "key" + to_string(i);
        string value = "value" + to_string(i);
        cache.set(key, value);
    }
    
    // Some keys should have been evicted
    assert(cache.getKeyCount() <= 5);
    
    cout << "✓ Memory eviction test passed" << endl;
}

void testPerformance() {
    cout << "Testing performance..." << endl;
    
    Cache cache;
    const int NUM_OPERATIONS = 10000;
    
    auto start = chrono::high_resolution_clock::now();
    
    // Perform SET operations
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        string key = "perf_key_" + to_string(i);
        string value = "perf_value_" + to_string(i);
        cache.set(key, value);
    }
    
    // Perform GET operations
    string value;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        string key = "perf_key_" + to_string(i);
        cache.get(key, value);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    double opsPerSecond = (2.0 * NUM_OPERATIONS) / (duration.count() / 1000.0);
    cout << "✓ Performance: " << opsPerSecond << " ops/sec" << endl;
}

int main() {
    cout << "=== MINI-REDIS CACHE TESTS ===" << endl << endl;
    
    try {
        testBasicOperations();
        testTTLFunctionality();
        testExpireCommand();
        testFlushOperation();
        testMemoryEviction();
        testPerformance();
        
        cout << endl << "🎉 All tests passed!" << endl;
    } catch (const exception& e) {
        cerr << "❌ Test failed: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}