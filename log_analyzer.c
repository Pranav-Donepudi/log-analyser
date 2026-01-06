/*
 * Smart Log Analyzer - Single File C Implementation
 * Combines all functionality into one simple C file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==================== STRUCTURES ==================== */

// Log Entry structure
typedef struct LogEntry {
    char* timestamp;
    char* log_level;
    char* message;
    struct LogEntry* next;
} LogEntry;

// Linked List structure
typedef struct {
    LogEntry* head;
    int size;
} LogList;

// Hash Node structure
typedef struct HashNode {
    char* key;
    int count;
    struct HashNode* next;
} HashNode;

// Hash Table structure
#define TABLE_SIZE 101
typedef struct {
    HashNode* buckets[TABLE_SIZE];
    int totalEntries;
} HashTable;

// KMP structure (no data needed, just for consistency)
typedef struct {
    // Empty - KMP is stateless
} KMP;

// Log Analyzer structure
typedef struct {
    LogList logList;
    HashTable errorTable;
    KMP kmpMatcher;
} LogAnalyzer;

/* ==================== LINKED LIST FUNCTIONS ==================== */

LogList* LogList_create() {
    LogList* list = (LogList*)malloc(sizeof(LogList));
    list->head = NULL;
    list->size = 0;
    return list;
}

void LogEntry_destroy(LogEntry* entry) {
    if (entry) {
        free(entry->timestamp);
        free(entry->log_level);
        free(entry->message);
        free(entry);
    }
}

void LogList_addEntry(LogList* list, const char* timestamp, const char* log_level, const char* message) {
    LogEntry* newEntry = (LogEntry*)malloc(sizeof(LogEntry));
    
    newEntry->timestamp = (char*)malloc(strlen(timestamp) + 1);
    strcpy(newEntry->timestamp, timestamp);
    
    newEntry->log_level = (char*)malloc(strlen(log_level) + 1);
    strcpy(newEntry->log_level, log_level);
    
    newEntry->message = (char*)malloc(strlen(message) + 1);
    strcpy(newEntry->message, message);
    
    newEntry->next = list->head;
    list->head = newEntry;
    list->size++;
}

void LogList_displayAll(LogList* list) {
    LogEntry* current = list->head;
    int index = 1;
    
    if (current == NULL) {
        printf("No log entries found.\n");
        return;
    }
    
    printf("\n=== All Log Entries ===\n");
    while (current != NULL) {
        printf("[%d] %s [%s] %s\n", index, current->timestamp, current->log_level, current->message);
        current = current->next;
        index++;
    }
    printf("\nTotal entries: %d\n", list->size);
}

void LogList_clear(LogList* list) {
    LogEntry* current = list->head;
    while (current != NULL) {
        LogEntry* next = current->next;
        LogEntry_destroy(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

void LogList_destroy(LogList* list) {
    LogList_clear(list);
    free(list);
}

/* ==================== HASH TABLE FUNCTIONS ==================== */

HashTable* HashTable_create() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->totalEntries = 0;
    return table;
}

int HashTable_hashFunction(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

void HashTable_insert(HashTable* table, const char* key) {
    int index = HashTable_hashFunction(key);
    HashNode* current = table->buckets[index];
    
    // Search for existing key
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }
    
    // Key not found, create new node
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = (char*)malloc(strlen(key) + 1);
    strcpy(newNode->key, key);
    newNode->count = 1;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    table->totalEntries++;
}

void HashTable_displayAll(HashTable* table) {
    int hasEntries = 0;
    
    printf("\n=== Hash Table Entries ===\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table->buckets[i];
        while (current != NULL) {
            printf("Key: \"%s\" -> Count: %d\n", current->key, current->count);
            current = current->next;
            hasEntries = 1;
        }
    }
    
    if (!hasEntries) {
        printf("No entries found.\n");
    } else {
        printf("\nTotal unique entries: %d\n", table->totalEntries);
    }
}

void HashTable_clear(HashTable* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table->buckets[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
        table->buckets[i] = NULL;
    }
    table->totalEntries = 0;
}

void HashTable_destroy(HashTable* table) {
    HashTable_clear(table);
    free(table);
}

int HashTable_getTotalEntries(HashTable* table) {
    return table->totalEntries;
}

/* ==================== KMP FUNCTIONS ==================== */

void KMP_buildFailureFunction(const char* pattern, int* failure, int patternLen) {
    failure[0] = 0;
    int j = 0;
    
    for (int i = 1; i < patternLen; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = failure[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            j++;
        }
        failure[i] = j;
    }
}

int KMP_search(const char* text, const char* pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    
    if (patternLen == 0 || patternLen > textLen) {
        return 0;
    }
    
    int* failure = (int*)malloc(patternLen * sizeof(int));
    KMP_buildFailureFunction(pattern, failure, patternLen);
    
    int count = 0;
    int j = 0;
    
    for (int i = 0; i < textLen; i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = failure[j - 1];
        }
        if (text[i] == pattern[j]) {
            j++;
        }
        if (j == patternLen) {
            count++;
            j = failure[j - 1];
        }
    }
    
    free(failure);
    return count;
}

int KMP_searchCaseInsensitive(const char* text, const char* pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    
    if (patternLen == 0 || patternLen > textLen) {
        return 0;
    }
    
    char* lowerText = (char*)malloc(textLen + 1);
    char* lowerPattern = (char*)malloc(patternLen + 1);
    
    for (int i = 0; i < textLen; i++) {
        lowerText[i] = tolower(text[i]);
    }
    lowerText[textLen] = '\0';
    
    for (int i = 0; i < patternLen; i++) {
        lowerPattern[i] = tolower(pattern[i]);
    }
    lowerPattern[patternLen] = '\0';
    
    int count = KMP_search(lowerText, lowerPattern);
    
    free(lowerText);
    free(lowerPattern);
    
    return count;
}

/* ==================== LOG ANALYZER FUNCTIONS ==================== */

LogAnalyzer* LogAnalyzer_create() {
    LogAnalyzer* analyzer = (LogAnalyzer*)malloc(sizeof(LogAnalyzer));
    analyzer->logList.head = NULL;
    analyzer->logList.size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        analyzer->errorTable.buckets[i] = NULL;
    }
    analyzer->errorTable.totalEntries = 0;
    return analyzer;
}

void LogAnalyzer_destroy(LogAnalyzer* analyzer) {
    if (analyzer) {
        LogList_clear(&analyzer->logList);
        HashTable_clear(&analyzer->errorTable);
        free(analyzer);
    }
}

int LogAnalyzer_isErrorLevel(const char* level) {
    return (strcmp(level, "ERROR") == 0 || strcmp(level, "error") == 0);
}

void LogAnalyzer_addLog(LogAnalyzer* analyzer, const char* timestamp, const char* log_level, const char* message) {
    LogList_addEntry(&analyzer->logList, timestamp, log_level, message);
    
    if (LogAnalyzer_isErrorLevel(log_level)) {
        HashTable_insert(&analyzer->errorTable, message);
    }
}

void LogAnalyzer_displayAllLogs(LogAnalyzer* analyzer) {
    LogList_displayAll(&analyzer->logList);
}

void LogAnalyzer_analyzeErrorFrequency(LogAnalyzer* analyzer) {
    printf("\n=== ERROR Frequency Analysis ===\n");
    HashTable_displayAll(&analyzer->errorTable);
}

int LogAnalyzer_searchKeyword(LogAnalyzer* analyzer, const char* keyword, int caseSensitive) {
    if (keyword == NULL || strlen(keyword) == 0) {
        return 0;
    }
    
    int matchCount = 0;
    LogEntry* current = analyzer->logList.head;
    
    while (current != NULL) {
        int count;
        if (caseSensitive) {
            count = KMP_search(current->message, keyword);
        } else {
            count = KMP_searchCaseInsensitive(current->message, keyword);
        }
        matchCount += count;
        current = current->next;
    }
    
    return matchCount;
}

void LogAnalyzer_displayLogsWithKeyword(LogAnalyzer* analyzer, const char* keyword, int caseSensitive) {
    if (keyword == NULL || strlen(keyword) == 0) {
        printf("Invalid keyword.\n");
        return;
    }
    
    printf("\n=== Logs containing \"%s\" ===\n", keyword);
    
    LogEntry* current = analyzer->logList.head;
    int foundCount = 0;
    int index = 1;
    
    while (current != NULL) {
        int count;
        if (caseSensitive) {
            count = KMP_search(current->message, keyword);
        } else {
            count = KMP_searchCaseInsensitive(current->message, keyword);
        }
        
        if (count > 0) {
            printf("[%d] %s [%s] %s\n", index, current->timestamp, current->log_level, current->message);
            foundCount++;
        }
        current = current->next;
        index++;
    }
    
    if (foundCount == 0) {
        printf("No logs found containing the keyword.\n");
    } else {
        printf("\nTotal matching logs: %d\n", foundCount);
    }
}

int LogAnalyzer_getTotalLogs(LogAnalyzer* analyzer) {
    return analyzer->logList.size;
}

int LogAnalyzer_getErrorCount(LogAnalyzer* analyzer) {
    return analyzer->errorTable.totalEntries;
}

void LogAnalyzer_clearAll(LogAnalyzer* analyzer) {
    LogList_clear(&analyzer->logList);
    HashTable_clear(&analyzer->errorTable);
}

void LogAnalyzer_loadSampleData(LogAnalyzer* analyzer) {
    LogAnalyzer_clearAll(analyzer);
    
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:00:00", "INFO", "Server started successfully");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:05:23", "WARNING", "High memory usage detected");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:10:45", "ERROR", "Failed to connect to database");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:15:12", "INFO", "User authentication successful");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:20:30", "ERROR", "Failed to connect to database");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:25:00", "DEBUG", "Processing request ID 12345");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:30:15", "ERROR", "File not found: config.xml");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:35:40", "INFO", "Cache cleared successfully");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:40:22", "WARNING", "Disk space below 10%%");
    LogAnalyzer_addLog(analyzer, "2024-01-15 08:45:55", "ERROR", "Failed to connect to database");
    LogAnalyzer_addLog(analyzer, "2024-01-15 09:00:00", "INFO", "Database connection restored");
    LogAnalyzer_addLog(analyzer, "2024-01-15 09:05:12", "ERROR", "File not found: config.xml");
    LogAnalyzer_addLog(analyzer, "2024-01-15 09:10:30", "INFO", "User login successful");
    LogAnalyzer_addLog(analyzer, "2024-01-15 09:15:45", "WARNING", "Slow query detected");
    LogAnalyzer_addLog(analyzer, "2024-01-15 09:20:00", "ERROR", "Failed to connect to database");
    
    printf("\n✓ Sample data loaded successfully!\n");
    printf("   Total logs: %d\n", LogAnalyzer_getTotalLogs(analyzer));
    printf("   Unique errors: %d\n", LogAnalyzer_getErrorCount(analyzer));
}

/* ==================== UI FUNCTIONS ==================== */

void displayTerminalMenu() {
    printf("\n");
    printf("========================================\n");
    printf("    Smart Log Analyzer\n");
    printf("========================================\n");
    printf("0. Load Sample Data (for testing)\n");
    printf("1. Add Log Entry\n");
    printf("2. Display All Logs\n");
    printf("3. Analyze ERROR Frequency\n");
    printf("4. Search Keyword in Logs\n");
    printf("5. Display Logs with Keyword\n");
    printf("6. Show Statistics\n");
    printf("7. Clear All Data\n");
    printf("8. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

int runTerminalUI(LogAnalyzer* analyzer) {
    int choice;
    char timestamp[64];
    char logLevel[32];
    char message[256];
    char keyword[128];
    char input[10];
    
    while (1) {
        displayTerminalMenu();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        if (sscanf(input, "%d", &choice) != 1) {
            printf("\nInvalid input. Please enter a number.\n");
            printf("\nPress Enter to continue...");
            getchar();
            continue;
        }
        
        switch (choice) {
            case 0: {
                LogAnalyzer_loadSampleData(analyzer);
                break;
            }
            
            case 1: {
                printf("\nEnter timestamp (YYYY-MM-DD HH:MM:SS): ");
                fgets(timestamp, sizeof(timestamp), stdin);
                timestamp[strcspn(timestamp, "\n")] = 0; // Remove newline
                
                printf("Enter log level (INFO/WARNING/ERROR/DEBUG): ");
                fgets(logLevel, sizeof(logLevel), stdin);
                logLevel[strcspn(logLevel, "\n")] = 0;
                
                printf("Enter message: ");
                fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = 0;
                
                LogAnalyzer_addLog(analyzer, timestamp, logLevel, message);
                printf("\n✓ Log entry added successfully!\n");
                break;
            }
            
            case 2: {
                LogAnalyzer_displayAllLogs(analyzer);
                break;
            }
            
            case 3: {
                LogAnalyzer_analyzeErrorFrequency(analyzer);
                break;
            }
            
            case 4: {
                printf("\nEnter keyword to search: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = 0;
                
                int count = LogAnalyzer_searchKeyword(analyzer, keyword, 1);
                printf("\nKeyword '%s' found %d time(s) in log messages.\n", keyword, count);
                break;
            }
            
            case 5: {
                printf("\nEnter keyword: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = 0;
                
                LogAnalyzer_displayLogsWithKeyword(analyzer, keyword, 1);
                break;
            }
            
            case 6: {
                printf("\n=== Statistics ===\n");
                printf("Total Logs: %d\n", LogAnalyzer_getTotalLogs(analyzer));
                printf("Unique Errors: %d\n", LogAnalyzer_getErrorCount(analyzer));
                break;
            }
            
            case 7: {
                LogAnalyzer_clearAll(analyzer);
                printf("\n✓ All data cleared successfully!\n");
                break;
            }
            
            case 8: {
                printf("\nExiting... Thank you!\n");
                return 0;
            }
            
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
        
        printf("\nPress Enter to continue...");
        fgets(input, sizeof(input), stdin); // Wait for Enter key
    }
    
    return 0;
}

/* ==================== MAIN FUNCTION ==================== */

int main(int argc, char* argv[]) {
    LogAnalyzer* analyzer = LogAnalyzer_create();
    
    // Check command line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage: %s [OPTIONS]\n", argv[0]);
            printf("Options:\n");
            printf("  --help, -h       Show this help message\n");
            LogAnalyzer_destroy(analyzer);
            return 0;
        }
    }
    
    // Run terminal UI
    int result = runTerminalUI(analyzer);
    LogAnalyzer_destroy(analyzer);
    return result;
}

