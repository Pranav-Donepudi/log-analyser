// Terminal-based UI - fallback when ncurses is not available
// This is the default UI that always works

#include "ui_terminal.h"
#include "core.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

// Terminal UI functions
void displayTerminalMenu() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "    Smart Log Analyzer\n";
    std::cout << "========================================\n";
    std::cout << "0. Load Sample Data (for testing)\n";
    std::cout << "1. Add Log Entry\n";
    std::cout << "2. Display All Logs\n";
    std::cout << "3. Analyze ERROR Frequency\n";
    std::cout << "4. Search Keyword in Logs\n";
    std::cout << "5. Display Logs with Keyword\n";
    std::cout << "6. Show Statistics\n";
    std::cout << "7. Clear All Data\n";
    std::cout << "8. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

// Run terminal UI
int runTerminalUI(LogAnalyzer& analyzer) {
    int choice;
    char timestamp[64];
    char logLevel[32];
    char message[256];
    char keyword[128];
    
    while (true) {
        displayTerminalMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from buffer
        
        switch (choice) {
            case 0: {
                // Load Sample Data
                analyzer.loadSampleData();
                break;
            }
            
            case 1: {
                // Add Log Entry
                std::cout << "\nEnter timestamp (YYYY-MM-DD HH:MM:SS): ";
                std::cin.getline(timestamp, 64);
                
                std::cout << "Enter log level (INFO/WARNING/ERROR/DEBUG): ";
                std::cin.getline(logLevel, 32);
                
                std::cout << "Enter message: ";
                std::cin.getline(message, 256);
                
                analyzer.addLog(timestamp, logLevel, message);
                std::cout << "\n✓ Log entry added successfully!\n";
                break;
            }
            
            case 2: {
                // Display All Logs
                analyzer.displayAllLogs();
                break;
            }
            
            case 3: {
                // Analyze ERROR Frequency
                analyzer.analyzeErrorFrequency();
                break;
            }
            
            case 4: {
                // Search Keyword
                std::cout << "\nEnter keyword to search: ";
                std::cin.getline(keyword, 128);
                
                int count = analyzer.searchKeyword(keyword, true);
                std::cout << "\nKeyword '" << keyword << "' found " << count 
                          << " time(s) in log messages.\n";
                break;
            }
            
            case 5: {
                // Display Logs with Keyword
                std::cout << "\nEnter keyword: ";
                std::cin.getline(keyword, 128);
                
                analyzer.displayLogsWithKeyword(keyword, true);
                break;
            }
            
            case 6: {
                // Show Statistics
                std::cout << "\n=== Statistics ===\n";
                std::cout << "Total Logs: " << analyzer.getTotalLogs() << "\n";
                std::cout << "Unique Errors: " << analyzer.getErrorCount() << "\n";
                break;
            }
            
            case 7: {
                // Clear All Data
                analyzer.clearAll();
                std::cout << "\n✓ All data cleared successfully!\n";
                break;
            }
            
            case 8: {
                // Exit
                std::cout << "\nExiting... Thank you!\n";
                return 0;
            }
            
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    return 0;
}