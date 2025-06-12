# Mini-Redis Cache System

A high-performance, in-memory cache system implemented in C++ that mimics core Redis functionality. This project demonstrates advanced data structures, algorithms, and systems programming concepts.

## 🚀 Features

### Core Operations
- **SET** - Store key-value pairs with optional TTL
- **GET** - Retrieve values by key
- **DELETE** - Remove keys from cache
- **EXISTS** - Check key existence
- **EXPIRE** - Set expiration time for keys
- **FLUSH** - Clear entire cache

### Advanced Features
- **Custom Hash Table** - Efficient collision handling with chaining
- **LRU Eviction** - Least Recently Used algorithm with O(1) operations
- **TTL Management** - Min-heap based expiration tracking
- **Memory Management** - Automatic eviction when memory limits exceeded
- **Performance Metrics** - Real-time statistics and throughput monitoring

## Architecture

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   Cache Layer   │────│   Hash Table     │────│   Data Storage  │
│   (Commands)    │    │   (O(1) lookup)  │    │   (Key-Value)   │
└─────────────────┘    └──────────────────┘    └─────────────────┘
         │                       │                       │
         │              ┌──────────────────┐             │
         └──────────────│   LRU Cache      │─────────────┘
                        │   (Eviction)     │
                        └──────────────────┘
                               │
                      ┌──────────────────┐
                      │   TTL Manager    │
                      │   (Expiration)   │
                      └──────────────────┘
```

## 🛠️ Building and Running

### Prerequisites
- C++17 compatible compiler (g++, clang++)
- Make build system

### Quick Start
```bash
# Clone and build
git clone https://github.com/anushkaVerma1007/mini-redis-cache.git
cd mini-redis-cache
make

# Run the cache
make run

# Run tests
make test

# Performance benchmark
make perf
```

## 💻 Usage Examples

### Interactive CLI
```bash
$ ./mini-redis
╔══════════════════════════════════════╗
║           MINI-REDIS CACHE           ║
║        High-Performance Cache        ║
╚══════════════════════════════════════╝

mini-redis> SET user:1001 john
OK

mini-redis> SET session:abc active 300
OK (expires in 300 seconds)

mini-redis> GET user:1001
"john"

mini-redis> EXISTS user:1001
(integer) 1

mini-redis> EXPIRE user:1001 60
(integer) 1

mini-redis> STATS
=== CACHE STATISTICS ===
Total Keys: 2
Memory Usage: 156 B / 100.00 MB
Memory Usage %: 0.00%
Total Operations: 6
Operations/sec: 245.67
LRU Cache Size: 2 / 10000
TTL Entries: 2
========================
```

### Command Reference
| Command | Syntax | Description | Example |
|---------|--------|-------------|---------|
| SET | `SET key value [ttl]` | Store key-value with optional TTL | `SET user john 300` |
| GET | `GET key` | Retrieve value | `GET user` |
| DELETE | `DELETE key` | Remove key | `DELETE user` |
| EXISTS | `EXISTS key` | Check existence | `EXISTS user` |
| EXPIRE | `EXPIRE key seconds` | Set expiration | `EXPIRE user 60` |
| FLUSH | `FLUSH` | Clear all data | `FLUSH` |
| STATS | `STATS` | Show statistics | `STATS` |

## 🧪 Testing

### Unit Tests
```bash
# Run all tests
make test

# Run specific tests
./test_cache    # Core functionality tests
./test_lru      # LRU algorithm tests
```

### Test Coverage
- ✅ Basic CRUD operations
- ✅ TTL expiration handling
- ✅ LRU eviction policy
- ✅ Memory management
- ✅ Performance benchmarks
- ✅ Edge cases and error handling

## 📊 Performance

### Benchmarks
- **Throughput**: 100,000+ operations/second
- **Memory Efficiency**: O(1) space per key-value pair
- **Time Complexity**:
  - SET/GET/DELETE: O(1) average case
  - LRU operations: O(1)
  - TTL cleanup: O(log n)

## 🔧 Technical Implementation

### Data Structures Used
1. **Custom Hash Table**
   - Chaining collision resolution
   - Dynamic resizing (load factor: 0.75)
   - Consistent O(1) average performance

2. **LRU Cache**
   - Doubly-linked list + hash map
   - O(1) access, insertion, deletion
   - Perfect for cache eviction

3. **TTL Manager**
   - Min-heap (priority queue) for expiration times
   - Efficient background cleanup
   - O(log n) insertion/removal

4. **Memory Management**
   - Real-time usage tracking
   - Configurable memory limits
   - Automatic eviction policies

### Key Algorithms
- **Hash Function**: STL hash with modulo distribution
- **LRU Policy**: Move-to-front on access
- **TTL Expiration**: Lazy deletion + periodic cleanup
- **Memory Eviction**: LRU-based with memory pressure detection

## 📈 Future Enhancements

### Potential Features
- [ ] Persistence to disk
- [ ] TCP support (Redis protocol)
- [ ] Redis-like data types
- [ ] Logging, config, clustering

## 🤝 Contributing

1.Fork & clone

2.git checkout -b feature/new-feature

3.Commit & PR

---

*Built with ❤️ and C++*
