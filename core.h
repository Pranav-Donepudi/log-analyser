#ifndef CORE_H
#define CORE_H

#include "log_list.h"
#include "hash_table.h"
#include "kmp.h"
#include <cstring>
#include <iostream>

// Core application logic - completely independent of UI
class LogAnalyzer {
private:
    LogList logList;           // Linked list to store all log entries
    HashTable errorTable;      // Hash table to count ERROR frequency
    KMP kmpMatcher;            // KMP pattern matcher
    
    // Helper function to check if log level is ERROR
    bool isErrorLevel(const char* level) const;
    
public:
    // Constructor
    LogAnalyzer();
    
    // Destructor
    ~LogAnalyzer();
    
    // Add a log entry to the system
    void addLog(const char* timestamp, const char* log_level, const char* message);
    
    // Display all log entries
    void displayAllLogs() const;
    
    // Count and display ERROR frequency using hash table
    void analyzeErrorFrequency() const;
    
    // Search for a keyword in log messages using KMP
    // Returns the number of matches found
    int searchKeyword(const char* keyword, bool caseSensitive = true) const;
    
    // Display logs containing a specific keyword
    void displayLogsWithKeyword(const char* keyword, bool caseSensitive = true) const;
    
    // Get statistics
    int getTotalLogs() const;
    int getErrorCount() const;
    
    // Clear all data
    void clearAll();
    
    // Load sample data for testing
    void loadSampleData();
};

#endif // CORE_H
