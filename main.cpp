// Main entry point
// Compile with: g++ -o analyzer main.cpp *.cpp

#include "core.h"
#include "ui_terminal.h"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    LogAnalyzer analyzer;
    
    // Check command line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n";
            std::cout << "Options:\n";
            std::cout << "  --help, -h       Show this help message\n";
            return 0;
        }
    }
    
    // Run terminal UI
    return runTerminalUI(analyzer);
}
