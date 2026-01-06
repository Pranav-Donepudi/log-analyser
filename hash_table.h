#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cctype>

// Structure to represent a hash table entry (for chaining)
struct HashNode {
    char* key;         // Error message or keyword
    int count;         // Frequency count
    HashNode* next;    // Pointer to next node in chain
    
    // Constructor
    inline HashNode(const char* k, int c) {
        key = new char[strlen(k) + 1];
        strcpy(key, k);
        count = c;
        next = nullptr;
    }
    
    // Destructor
    inline ~HashNode() {
        delete[] key;
    }
};

// Hash Table class with chaining for collision resolution
class HashTable {
private:
    static const int TABLE_SIZE = 101;  // Prime number for better distribution
    HashNode** buckets;                 // Array of pointers to hash nodes
    int totalEntries;                   // Total number of entries
    
    // Hash function (djb2 algorithm)
    inline int hashFunction(const char* key) const {
        unsigned long hash = 5381;
        int c;
        while ((c = *key++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % TABLE_SIZE;
    }
    
    // Helper function to convert string to lowercase for case-insensitive comparison
    inline void toLowerCase(char* str) const {
        for (int i = 0; str[i]; i++) {
            str[i] = tolower(str[i]);
        }
    }
    
public:
    // Constructor
    inline HashTable() {
        buckets = new HashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            buckets[i] = nullptr;
        }
        totalEntries = 0;
    }
    
    // Destructor
    inline ~HashTable() {
        clear();
        delete[] buckets;
    }
    
    // Insert or update a key with its count
    inline void insert(const char* key) {
        int index = hashFunction(key);
        HashNode* current = buckets[index];
        
        // Search for existing key in the chain
        while (current != nullptr) {
            if (strcmp(current->key, key) == 0) {
                // Key found, increment count
                current->count++;
                return;
            }
            current = current->next;
        }
        
        // Key not found, create new node and insert at the beginning of chain
        HashNode* newNode = new HashNode(key, 1);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        totalEntries++;
    }
    
    // Get the count for a specific key
    inline int getCount(const char* key) const {
        int index = hashFunction(key);
        HashNode* current = buckets[index];
        
        // Search for the key in the chain
        while (current != nullptr) {
            if (strcmp(current->key, key) == 0) {
                return current->count;
            }
            current = current->next;
        }
        
        // Key not found
        return 0;
    }
    
    // Display all entries in the hash table
    inline void displayAll() const {
        bool hasEntries = false;
        
        std::cout << "\n=== Hash Table Entries ===\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = buckets[i];
            while (current != nullptr) {
                std::cout << "Key: \"" << current->key 
                          << "\" -> Count: " << current->count << "\n";
                current = current->next;
                hasEntries = true;
            }
        }
        
        if (!hasEntries) {
            std::cout << "No entries found.\n";
        } else {
            std::cout << "\nTotal unique entries: " << totalEntries << "\n";
        }
    }
    
    // Clear all entries
    inline void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = buckets[i];
            while (current != nullptr) {
                HashNode* next = current->next;
                delete current;
                current = next;
            }
            buckets[i] = nullptr;
        }
        totalEntries = 0;
    }
    
    // Get total number of entries
    inline int getTotalEntries() const {
        return totalEntries;
    }
};

#endif // HASH_TABLE_H