#ifndef LOG_LIST_H
#define LOG_LIST_H

#include <cstring>
#include <iostream>
#include <cstdlib>

// Structure to represent a log entry
struct LogEntry {
    char* timestamp;    // Format: "YYYY-MM-DD HH:MM:SS"
    char* log_level;   // "INFO", "WARNING", "ERROR", "DEBUG"
    char* message;     // Log message content
    
    LogEntry* next;    // Pointer to next entry in linked list
    
    // Constructor
    inline LogEntry(const char* ts, const char* level, const char* msg) {
        // Allocate memory and copy timestamp
        timestamp = new char[strlen(ts) + 1];
        strcpy(timestamp, ts);
        
        // Allocate memory and copy log level
        log_level = new char[strlen(level) + 1];
        strcpy(log_level, level);
        
        // Allocate memory and copy message
        message = new char[strlen(msg) + 1];
        strcpy(message, msg);
        
        next = nullptr;
    }
    
    // Destructor
    inline ~LogEntry() {
        delete[] timestamp;
        delete[] log_level;
        delete[] message;
    }
};

// Linked List class to store log entries
class LogList {
private:
    LogEntry* head;    // Head of the linked list
    int size;          // Number of entries in the list
    
public:
    // Constructor
    inline LogList() {
        head = nullptr;
        size = 0;
    }
    
    // Destructor
    inline ~LogList() {
        clear();
    }
    
    // Add a new log entry to the list
    inline void addEntry(const char* timestamp, const char* log_level, const char* message) {
        LogEntry* newEntry = new LogEntry(timestamp, log_level, message);
        
        // Insert at the beginning for O(1) insertion
        newEntry->next = head;
        head = newEntry;
        size++;
    }
    
    // Get the head of the list
    inline LogEntry* getHead() const {
        return head;
    }
    
    // Get the size of the list
    inline int getSize() const {
        return size;
    }
    
    // Display all log entries
    inline void displayAll() const {
        LogEntry* current = head;
        int index = 1;
        
        if (current == nullptr) {
            std::cout << "No log entries found.\n";
            return;
        }
        
        std::cout << "\n=== All Log Entries ===\n";
        while (current != nullptr) {
            std::cout << "[" << index << "] " << current->timestamp 
                      << " [" << current->log_level << "] " 
                      << current->message << "\n";
            current = current->next;
            index++;
        }
        std::cout << "\nTotal entries: " << size << "\n";
    }
    
    // Clear all entries
    inline void clear() {
        LogEntry* current = head;
        while (current != nullptr) {
            LogEntry* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }
};

#endif // LOG_LIST_H