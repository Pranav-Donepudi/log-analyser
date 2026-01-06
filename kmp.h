#ifndef KMP_H
#define KMP_H

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cctype>

// KMP Pattern Matching class
class KMP {
private:
    // Build the failure function (prefix table) for KMP algorithm
    inline void buildFailureFunction(const char* pattern, int* failure, int patternLen) const {
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
    
public:
    // Constructor
    inline KMP() {
        // No initialization needed
    }
    
    // Search for pattern in text using KMP algorithm
    // Returns the number of occurrences found
    inline int search(const char* text, const char* pattern) const {
        int textLen = strlen(text);
        int patternLen = strlen(pattern);
        
        if (patternLen == 0) {
            return 0;
        }
        
        if (patternLen > textLen) {
            return 0;
        }
        
        // Allocate failure function array
        int* failure = new int[patternLen];
        buildFailureFunction(pattern, failure, patternLen);
        
        int count = 0;
        int j = 0;  // Index for pattern
        
        // Search through the text
        for (int i = 0; i < textLen; i++) {
            while (j > 0 && text[i] != pattern[j]) {
                j = failure[j - 1];
            }
            if (text[i] == pattern[j]) {
                j++;
            }
            if (j == patternLen) {
                // Pattern found
                count++;
                j = failure[j - 1];  // Continue searching for more occurrences
            }
        }
        
        delete[] failure;
        return count;
    }
    
    // Search for pattern in text and return positions of all matches
    // positions array should be pre-allocated with sufficient size
    // Returns the number of occurrences found
    inline int searchAll(const char* text, const char* pattern, int* positions, int maxPositions) const {
        int textLen = strlen(text);
        int patternLen = strlen(pattern);
        
        if (patternLen == 0 || patternLen > textLen || maxPositions <= 0) {
            return 0;
        }
        
        // Allocate failure function array
        int* failure = new int[patternLen];
        buildFailureFunction(pattern, failure, patternLen);
        
        int count = 0;
        int j = 0;  // Index for pattern
        
        // Search through the text
        for (int i = 0; i < textLen && count < maxPositions; i++) {
            while (j > 0 && text[i] != pattern[j]) {
                j = failure[j - 1];
            }
            if (text[i] == pattern[j]) {
                j++;
            }
            if (j == patternLen) {
                // Pattern found at position (i - patternLen + 1)
                positions[count] = i - patternLen + 1;
                count++;
                j = failure[j - 1];  // Continue searching for more occurrences
            }
        }
        
        delete[] failure;
        return count;
    }
    
    // Case-insensitive search
    inline int searchCaseInsensitive(const char* text, const char* pattern) const {
        int textLen = strlen(text);
        int patternLen = strlen(pattern);
        
        if (patternLen == 0) {
            return 0;
        }
        
        if (patternLen > textLen) {
            return 0;
        }
        
        // Create lowercase copies for comparison
        char* lowerText = new char[textLen + 1];
        char* lowerPattern = new char[patternLen + 1];
        
        for (int i = 0; i < textLen; i++) {
            lowerText[i] = tolower(text[i]);
        }
        lowerText[textLen] = '\0';
        
        for (int i = 0; i < patternLen; i++) {
            lowerPattern[i] = tolower(pattern[i]);
        }
        lowerPattern[patternLen] = '\0';
        
        int count = search(lowerText, lowerPattern);
        
        delete[] lowerText;
        delete[] lowerPattern;
        
        return count;
    }
};

#endif // KMP_H