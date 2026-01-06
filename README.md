
## Abstract

The Smart Log Analyzer is a C++ application designed to efficiently process and analyze server log entries using fundamental data structures and algorithms. The system implements a linked list for log storage, a hash table with chaining for error frequency analysis, and the Knuth-Morris-Pratt (KMP) algorithm for pattern matching. The application features a modular architecture with a fail-safe design that allows core functionality to operate independently of the user interface, supporting a terminal-based menu-driven interface.

## Problem Statement

Modern server systems generate vast amounts of log data that require efficient processing and analysis. The challenge is to:
1. Store log entries efficiently for quick access
2. Analyze error patterns and frequencies
3. Search for specific keywords or patterns in log messages
4. Provide an intuitive interface for log analysis

This project addresses these challenges by implementing custom data structures (without STL containers) and efficient algorithms to provide a robust log analysis tool suitable for educational and practical use.

## System Design

### Architecture Overview

The system follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────┐
│         UI Layer                     │
│  ┌─────────────┐                     │
│  │  Terminal   │                     │
│  │     UI      │                     │
│  └─────────────┘                     │
└─────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────┐
│         Core Logic Layer            │
│         (LogAnalyzer)               │
└─────────────────────────────────────┘
              │
    ┌─────────┼─────────┐
    ▼         ▼         ▼
┌────────┐ ┌────────┐ ┌──────┐
│  Log   │ │  Hash  │ │ KMP  │
│  List  │ │  Table │ │      │
└────────┘ └────────┘ └──────┘
```

### Module Description

#### 1. Linked List Module (`log_list.h/cpp`)
- **Purpose**: Store log entries in a dynamic linked list structure
- **Data Structure**: Singly linked list with head pointer
- **Operations**:
  - `addEntry()`: Insert new log entry at the beginning (O(1))
  - `displayAll()`: Traverse and display all entries (O(n))
  - `getHead()`: Access the head pointer
  - `clear()`: Remove all entries (O(n))

#### 2. Hash Table Module (`hash_table.h/cpp`)
- **Purpose**: Count frequency of ERROR messages using hashing with chaining
- **Data Structure**: Array of linked lists (buckets) with size 101 (prime number)
- **Hash Function**: djb2 algorithm for string hashing
- **Operations**:
  - `insert()`: Insert or update key count (O(1) average, O(n) worst case)
  - `getCount()`: Retrieve count for a key (O(1) average, O(n) worst case)
  - `displayAll()`: Display all entries (O(n))
  - `clear()`: Remove all entries (O(n))

#### 3. KMP Pattern Matching Module (`kmp.h/cpp`)
- **Purpose**: Efficient pattern matching in log messages
- **Algorithm**: Knuth-Morris-Pratt algorithm
- **Operations**:
  - `search()`: Find pattern occurrences (O(n + m))
  - `searchAll()`: Find all pattern positions (O(n + m))
  - `searchCaseInsensitive()`: Case-insensitive search (O(n + m))

#### 4. Core Logic Module (`core.h/cpp`)
- **Purpose**: Orchestrate all data structures and provide high-level operations
- **Class**: `LogAnalyzer`
- **Operations**:
  - `addLog()`: Add log entry to system
  - `displayAllLogs()`: Display all stored logs
  - `analyzeErrorFrequency()`: Show ERROR frequency analysis
  - `searchKeyword()`: Search for keywords using KMP
  - `displayLogsWithKeyword()`: Display logs containing keyword
  - `getTotalLogs()`: Get statistics
  - `clearAll()`: Reset all data structures

#### 5. UI Module
- **Terminal UI** (`ui_terminal.h/cpp`): Menu-driven interface

#### 6. Main Module (`main.cpp`)
- **Purpose**: Entry point
- **Features**: Command-line argument parsing

## Compilation Instructions

### Prerequisites
- C++ compiler (g++ recommended)
- Make utility (optional, for Makefile)

### Compiling

```bash
g++ -Wall -std=c++11 -o analyzer main.cpp log_list.cpp hash_table.cpp kmp.cpp core.cpp ui_terminal.cpp
```

### Using Makefile (Optional)

The project includes a `Makefile`:

```bash
make              # Build the application
make clean        # Remove executable
make help         # Show help
```

## Running the Application

```bash
./analyzer
```

### Help
```bash
./analyzer --help
# or
./analyzer -h
```

## Sample Input/Output

### Sample Session (Terminal Mode)

```
========================================
    Smart Log Analyzer
========================================
1. Add Log Entry
2. Display All Logs
3. Analyze ERROR Frequency
4. Search Keyword in Logs
5. Display Logs with Keyword
6. Show Statistics
7. Clear All Data
8. Exit
========================================
Enter your choice: 1

Enter timestamp (YYYY-MM-DD HH:MM:SS): 2024-01-15 10:30:45
Enter log level (INFO/WARNING/ERROR/DEBUG): ERROR
Enter message: Database connection failed

✓ Log entry added successfully!

Press Enter to continue...

Enter your choice: 1

Enter timestamp (YYYY-MM-DD HH:MM:SS): 2024-01-15 10:31:12
Enter log level (INFO/WARNING/ERROR/DEBUG): INFO
Enter message: User login successful

✓ Log entry added successfully!

Press Enter to continue...

Enter your choice: 1

Enter timestamp (YYYY-MM-DD HH:MM:SS): 2024-01-15 10:32:00
Enter log level (INFO/WARNING/ERROR/DEBUG): ERROR
Enter message: Database connection failed

✓ Log entry added successfully!

Press Enter to continue...

Enter your choice: 2

=== All Log Entries ===
[1] 2024-01-15 10:32:00 [ERROR] Database connection failed
[2] 2024-01-15 10:31:12 [INFO] User login successful
[3] 2024-01-15 10:30:45 [ERROR] Database connection failed

Total entries: 3

Press Enter to continue...

Enter your choice: 3

=== ERROR Frequency Analysis ===
Key: "Database connection failed" -> Count: 2

Total unique entries: 1

Press Enter to continue...

Enter your choice: 4

Enter keyword to search: Database

Keyword 'Database' found 2 time(s) in log messages.

Press Enter to continue...

Enter your choice: 5

Enter keyword: connection

=== Logs containing "connection" ===
[1] 2024-01-15 10:32:00 [ERROR] Database connection failed
[2] 2024-01-15 10:30:45 [ERROR] Database connection failed

Total matching logs: 2

Press Enter to continue...

Enter your choice: 6

=== Statistics ===
Total Logs: 3
Unique Errors: 1

Press Enter to continue...

Enter your choice: 8

Exiting... Thank you!
```

### Sample Test Data

You can add the following sample logs for testing:

```
2024-01-15 08:00:00 | INFO | Server started successfully
2024-01-15 08:05:23 | WARNING | High memory usage detected
2024-01-15 08:10:45 | ERROR | Failed to connect to database
2024-01-15 08:15:12 | INFO | User authentication successful
2024-01-15 08:20:30 | ERROR | Failed to connect to database
2024-01-15 08:25:00 | DEBUG | Processing request ID 12345
2024-01-15 08:30:15 | ERROR | File not found: config.xml
2024-01-15 08:35:40 | INFO | Cache cleared successfully
2024-01-15 08:40:22 | WARNING | Disk space below 10%
2024-01-15 08:45:55 | ERROR | Failed to connect to database
```

## Time and Space Complexity Analysis

### Linked List Operations

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `addEntry()` | O(1) | O(1) |
| `displayAll()` | O(n) | O(1) |
| `getHead()` | O(1) | O(1) |
| `clear()` | O(n) | O(1) |

**Overall**: O(n) space for n log entries

### Hash Table Operations

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `hashFunction()` | O(k) where k = key length | O(1) |
| `insert()` | O(1) average, O(n) worst case | O(1) |
| `getCount()` | O(1) average, O(n) worst case | O(1) |
| `displayAll()` | O(n) | O(1) |
| `clear()` | O(n) | O(1) |

**Overall**: 
- Time: O(1) average case, O(n) worst case (all collisions)
- Space: O(n) for n unique keys

**Note**: With a good hash function and prime table size (101), collisions are rare, making average case O(1) realistic.

### KMP Pattern Matching

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `buildFailureFunction()` | O(m) where m = pattern length | O(m) |
| `search()` | O(n + m) where n = text length | O(m) |
| `searchAll()` | O(n + m) | O(m) |
| `searchCaseInsensitive()` | O(n + m) | O(n + m) for lowercase copies |

**Overall**: 
- Time: O(n + m) - linear time, optimal for pattern matching
- Space: O(m) for failure function array

### Core Operations

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `addLog()` | O(1) + O(k) where k = message length | O(1) |
| `displayAllLogs()` | O(n) | O(1) |
| `analyzeErrorFrequency()` | O(e) where e = unique errors | O(1) |
| `searchKeyword()` | O(n × (t + m)) where t = avg text length | O(m) |
| `displayLogsWithKeyword()` | O(n × (t + m)) | O(m) |

**Overall System**:
- **Time Complexity**: 
  - Adding logs: O(1) per log
  - Searching: O(n × (t + m)) where n = number of logs
  - Error analysis: O(e) where e = unique errors
- **Space Complexity**: O(n × L) where n = number of logs, L = average log entry size

### Comparison with Alternatives

| Data Structure | Insert | Search | Space |
|----------------|--------|--------|-------|
| **Linked List** | O(1) | O(n) | O(n) |
| **Hash Table** | O(1) avg | O(1) avg | O(n) |
| **KMP** | N/A | O(n+m) | O(m) |

**Why these choices?**
- **Linked List**: Simple, dynamic, O(1) insertion for logs
- **Hash Table**: Optimal for frequency counting (O(1) average lookup)
- **KMP**: Optimal pattern matching algorithm (O(n+m) guaranteed, better than naive O(n×m))

## Features

1. ✅ **Modular Design**: Clear separation between data structures, core logic, and UI
2. ✅ **Fail-Safe Architecture**: Core logic works independently of UI
3. ✅ **No STL Containers**: All data structures implemented from scratch
4. ✅ **Efficient Algorithms**: KMP for pattern matching, hash table for O(1) lookups
5. ✅ **Terminal UI**: Menu-driven interface
6. ✅ **Memory Management**: Proper allocation and deallocation
7. ✅ **Well-Commented Code**: Clear documentation throughout

## File Structure

```
smart_log_analyzer/
├── log_list.h              # Linked list header
├── log_list.cpp            # Linked list implementation
├── hash_table.h            # Hash table header
├── hash_table.cpp          # Hash table implementation
├── kmp.h                   # KMP algorithm header
├── kmp.cpp                 # KMP algorithm implementation
├── core.h                  # Core logic header
├── core.cpp                # Core logic implementation
├── ui_terminal.h           # Terminal UI header
├── ui_terminal.cpp         # Terminal UI implementation
├── main.cpp                # Main entry point
└── README.md               # This file
```

## Design Decisions

1. **Linked List for Logs**: Chosen for O(1) insertion and simplicity. Trade-off: O(n) search, but logs are typically displayed sequentially.

2. **Hash Table Size 101**: Prime number reduces collisions. Chaining handles collisions gracefully.

3. **KMP Algorithm**: Chosen over naive pattern matching for O(n+m) guaranteed performance, especially important for long log messages.

4. **UI Separation**: Core logic completely independent of UI ensures reliability and testability.

5. **No STL**: Educational requirement to understand underlying data structures.

## Limitations and Future Enhancements

### Current Limitations
- Fixed-size character arrays (could use dynamic allocation)
- No file I/O for loading logs from files
- No persistence (data lost on exit)
- Limited to single-threaded operation

### Possible Enhancements
- File-based log loading
- Data persistence (save/load state)
- Multi-threaded processing for large log files
- Advanced filtering options
- Log export functionality
- Regular expression support
- Time-based filtering

## Testing Recommendations

1. **Empty List**: Test with no logs added
2. **Single Entry**: Test with one log entry
3. **Multiple Errors**: Test error frequency with duplicate errors
4. **Pattern Matching**: Test KMP with various patterns (short, long, not found)
5. **Edge Cases**: Empty strings, very long messages, special characters
6. **Memory**: Verify no memory leaks (use valgrind if available)

## License

This project is created for educational purposes as part of an Advanced Data Structures laboratory assignment.

## Author

Created for Advanced Data Structures Lab - Smart Log Analyzer Project

---

**Note**: This implementation prioritizes educational value and clarity over production-ready features. The code demonstrates fundamental data structures and algorithms without relying on standard library containers.
