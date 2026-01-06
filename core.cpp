#include "core.h"
#include <iostream>
#include <cstring>
#include <cctype>
// Constructor
LogAnalyzer::LogAnalyzer() {
    // All data structures are initialized by their constructors
}

// Destructor
LogAnalyzer::~LogAnalyzer() {
    // All data structures clean up automatically via their destructors
}

// Helper function to check if log level is ERROR
bool LogAnalyzer::isErrorLevel(const char* level) const {
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "error") == 0);
}

// Add a log entry to the system
void LogAnalyzer::addLog(const char* timestamp, const char* log_level, const char* message) {
    // Add to linked list
    logList.addEntry(timestamp, log_level, message);
    
    // If it's an ERROR, add to hash table
    if (isErrorLevel(log_level)) {
        errorTable.insert(message);
    }
}

// Display all log entries
void LogAnalyzer::displayAllLogs() const {
    logList.displayAll();
}

// Count and display ERROR frequency using hash table
void LogAnalyzer::analyzeErrorFrequency() const {
    std::cout << "\n=== ERROR Frequency Analysis ===\n";
    errorTable.displayAll();
}

// Search for a keyword in log messages using KMP
int LogAnalyzer::searchKeyword(const char* keyword, bool caseSensitive) const {
    if (keyword == nullptr || strlen(keyword) == 0) {
        return 0;
    }
    
    int matchCount = 0;
    LogEntry* current = logList.getHead();
    
    while (current != nullptr) {
        int count;
        if (caseSensitive) {
            count = kmpMatcher.search(current->message, keyword);
        } else {
            count = kmpMatcher.searchCaseInsensitive(current->message, keyword);
        }
        matchCount += count;
        current = current->next;
    }
    
    return matchCount;
}

// Display logs containing a specific keyword
void LogAnalyzer::displayLogsWithKeyword(const char* keyword, bool caseSensitive) const {
    if (keyword == nullptr || strlen(keyword) == 0) {
        std::cout << "Invalid keyword.\n";
        return;
    }
    
    std::cout << "\n=== Logs containing \"" << keyword << "\" ===\n";
    
    LogEntry* current = logList.getHead();
    int foundCount = 0;
    int index = 1;
    
    while (current != nullptr) {
        int count;
        if (caseSensitive) {
            count = kmpMatcher.search(current->message, keyword);
        } else {
            count = kmpMatcher.searchCaseInsensitive(current->message, keyword);
        }
        
        if (count > 0) {
            std::cout << "[" << index << "] " << current->timestamp 
                      << " [" << current->log_level << "] " 
                      << current->message << "\n";
            foundCount++;
        }
        current = current->next;
        index++;
    }
    
    if (foundCount == 0) {
        std::cout << "No logs found containing the keyword.\n";
    } else {
        std::cout << "\nTotal matching logs: " << foundCount << "\n";
    }
}

// Get statistics
int LogAnalyzer::getTotalLogs() const {
    return logList.getSize();
}

int LogAnalyzer::getErrorCount() const {
    return errorTable.getTotalEntries();
}

// Clear all data
void LogAnalyzer::clearAll() {
    logList.clear();
    errorTable.clear();
}

// Load sample data for testing
void LogAnalyzer::loadSampleData() {
    // Clear existing data first
    clearAll();
    
    // Add sample log entries with variety of levels and messages
    addLog("2024-01-15 08:00:00", "INFO", "Server started successfully");
    addLog("2024-01-15 08:05:23", "WARNING", "High memory usage detected");
    addLog("2024-01-15 08:10:45", "ERROR", "Failed to connect to database");
    addLog("2024-01-15 08:15:12", "INFO", "User authentication successful");
    addLog("2024-01-15 08:20:30", "ERROR", "Failed to connect to database");
    addLog("2024-01-15 08:25:00", "DEBUG", "Processing request ID 12345");
    addLog("2024-01-15 08:30:15", "ERROR", "File not found: config.xml");
    addLog("2024-01-15 08:35:40", "INFO", "Cache cleared successfully");
    addLog("2024-01-15 08:40:22", "WARNING", "Disk space below 10%");
    addLog("2024-01-15 08:45:55", "ERROR", "Failed to connect to database");
    addLog("2024-01-15 09:00:00", "INFO", "Database connection restored");
    addLog("2024-01-15 09:05:12", "ERROR", "File not found: config.xml");
    addLog("2024-01-15 09:10:30", "INFO", "User login successful");
    addLog("2024-01-15 09:15:45", "WARNING", "Slow query detected");
    addLog("2024-01-15 09:20:00", "ERROR", "Failed to connect to database");
    
    std::cout << "\n✓ Sample data loaded successfully!\n";
    std::cout << "   Total logs: " << getTotalLogs() << "\n";
    std::cout << "   Unique errors: " << getErrorCount() << "\n";
}
