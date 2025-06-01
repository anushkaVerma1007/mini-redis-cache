#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "../include/Cache.hpp"
#include "../include/utils.hpp"

using namespace std;

class MiniRedisCLI {
private:
    Cache* cache;
    bool running;
    
    void printWelcome() {
        cout << "╔══════════════════════════════════════╗" << endl;
        cout << "║           MINI-REDIS CACHE           ║" << endl;
        cout << "║        High-Performance Cache        ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
        cout << "\nSupported Commands:" << endl;
        cout << "  SET key value [ttl]  - Store key-value with optional TTL" << endl;
        cout << "  GET key             - Retrieve value by key" << endl;
        cout << "  DELETE key          - Remove key" << endl;
        cout << "  EXISTS key          - Check if key exists" << endl;
        cout << "  EXPIRE key seconds  - Set expiration time" << endl;
        cout << "  FLUSH               - Clear all data" << endl;
        cout << "  STATS               - Show cache statistics" << endl;
        cout << "  HELP                - Show this help" << endl;
        cout << "  QUIT                - Exit program" << endl;
        cout << "\nReady for commands...\n" << endl;
    }
    
    void printHelp() {
        cout << "\n=== MINI-REDIS COMMANDS ===" << endl;
        cout << "SET key value [ttl]    Store a key-value pair with optional TTL in seconds" << endl;
        cout << "GET key                Retrieve the value for a key" << endl;
        cout << "DELETE key             Remove a key and its value" << endl;
        cout << "EXISTS key             Check if a key exists and is not expired" << endl;
        cout << "EXPIRE key seconds     Set expiration time for an existing key" << endl;
        cout << "FLUSH                  Clear the entire cache" << endl;
        cout << "STATS                  Display cache statistics and performance metrics" << endl;
        cout << "HELP                   Show this help message" << endl;
        cout << "QUIT                   Exit the program" << endl;
        cout << "\nExamples:" << endl;
        cout << "  > SET user:1 john 300    # Store 'john' with 5 min TTL" << endl;
        cout << "  > GET user:1             # Retrieve value" << endl;
        cout << "  > EXISTS user:1          # Check existence" << endl;
        cout << "  > EXPIRE user:1 60       # Set 1 min expiry" << endl;
        cout << "  > DELETE user:1          # Remove key" << endl;
        cout << "========================\n" << endl;
    }
    
    string toUpper(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
    
    void handleSetCommand(const vector<string>& tokens) {
        if (tokens.size() < 3) {
            cout << "Error: SET requires at least key and value" << endl;
            cout << "Usage: SET key value [ttl]" << endl;
            return;
        }
        
        string key = tokens[1];
        string value = tokens[2];
        int ttl = -1;
        
        if (tokens.size() >= 4) {
            try {
                ttl = stoi(tokens[3]);
                if (ttl <= 0) {
                    cout << "Error: TTL must be positive" << endl;
                    return;
                }
            } catch (const exception& e) {
                cout << "Error: Invalid TTL value" << endl;
                return;
            }
        }
        
        if (cache->set(key, value, ttl)) {
            if (ttl > 0) {
                cout << "OK (expires in " << ttl << " seconds)" << endl;
            } else {
                cout << "OK" << endl;
            }
        } else {
            cout << "Error: Failed to set key" << endl;
        }
    }
    
    void handleGetCommand(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Error: GET requires a key" << endl;
            cout << "Usage: GET key" << endl;
            return;
        }
        
        string key = tokens[1];
        string value;
        
        if (cache->get(key, value)) {
            cout << "\"" << value << "\"" << endl;
        } else {
            cout << "(nil)" << endl;
        }
    }
    
    void handleDeleteCommand(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Error: DELETE requires a key" << endl;
            cout << "Usage: DELETE key" << endl;
            return;
        }
        
        string key = tokens[1];
        
        if (cache->del(key)) {
            cout << "(integer) 1" << endl;
        } else {
            cout << "(integer) 0" << endl;
        }
    }
    
    void handleExistsCommand(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Error: EXISTS requires a key" << endl;
            cout << "Usage: EXISTS key" << endl;
            return;
        }
        
        string key = tokens[1];
        
        if (cache->exists(key)) {
            cout << "(integer) 1" << endl;
        } else {
            cout << "(integer) 0" << endl;
        }
    }
    
    void handleExpireCommand(const vector<string>& tokens) {
        if (tokens.size() < 3) {
            cout << "Error: EXPIRE requires key and seconds" << endl;
            cout << "Usage: EXPIRE key seconds" << endl;
            return;
        }
        
        string key = tokens[1];
        int seconds;
        
        try {
            seconds = stoi(tokens[2]);
            if (seconds <= 0) {
                cout << "Error: Seconds must be positive" << endl;
                return;
            }
        } catch (const exception& e) {
            cout << "Error: Invalid seconds value" << endl;
            return;
        }
        
        if (cache->expire(key, seconds)) {
            cout << "(integer) 1" << endl;
        } else {
            cout << "(integer) 0" << endl;
        }
    }
    
    void handleFlushCommand() {
        cache->flush();
        cout << "OK" << endl;
    }
    
    void processCommand(const string& input) {
        if (input.empty()) return;
        
        vector<string> tokens = Utils::splitString(input, ' ');
        if (tokens.empty()) return;
        
        string command = toUpper(tokens[0]);
        
        if (command == "SET") {
            handleSetCommand(tokens);
        }
        else if (command == "GET") {
            handleGetCommand(tokens);
        }
        else if (command == "DELETE" || command == "DEL") {
            handleDeleteCommand(tokens);
        }
        else if (command == "EXISTS") {
            handleExistsCommand(tokens);
        }
        else if (command == "EXPIRE") {
            handleExpireCommand(tokens);
        }
        else if (command == "FLUSH") {
            handleFlushCommand();
        }
        else if (command == "STATS") {
            cache->showStats();
        }
        else if (command == "HELP") {
            printHelp();
        }
        else if (command == "QUIT" || command == "EXIT") {
            running = false;
            cout << "Goodbye!" << endl;
        }
        else {
            cout << "Error: Unknown command '" << command << "'" << endl;
            cout << "Type HELP for available commands" << endl;
        }
    }
    
public:
    MiniRedisCLI() {
        cache = new Cache();
        running = true;
    }
    
    ~MiniRedisCLI() {
        delete cache;
    }
    
    void run() {
        printWelcome();
        
        string input;
        while (running) {
            cout << "mini-redis> ";
            getline(cin, input);
            
            // Trim whitespace
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);
            
            if (!input.empty()) {
                processCommand(input);
            }
        }
    }
};

int main() {
    try {
        MiniRedisCLI cli;
        cli.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}