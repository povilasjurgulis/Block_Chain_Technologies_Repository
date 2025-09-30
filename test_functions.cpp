#include "test_functions.h"

// Function for efficiency testing
void efficiency_test() {
    cout << "\n--- EFFICIENCY TEST ---" << endl;
    
    // Ask for filename
    string filename;
    cout << "Enter filename for efficiency test (or press Enter to skip file test): ";
    getline(cin, filename);
    
    if (!filename.empty()) {
        ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer; // Sukuriame stream objekta
            buffer << file.rdbuf(); // Nuskaitome visa faila i buferi
            string content = buffer.str(); // Konvertuojame i string (iš stream)
            file.close();
            
            cout << "Testing with " << filename << " file (" << content.length() << " characters)" << endl;
            
            // Measure time
            auto start = high_resolution_clock::now();
            string hash_result = hash_function(content);
            auto end = high_resolution_clock::now();
            
            auto duration_us = duration_cast<microseconds>(end - start);
            auto duration_ms = duration_cast<milliseconds>(end - start);
            
            cout << "Hash result: " << hash_result << endl;
            cout << "Time: " << duration_us.count() << " microseconds (" 
                 << duration_ms.count() << " ms)" << endl;
            
            // Calculate throughput
            double chars_per_second = (double)content.length() / ((double)duration_us.count() / 1000000.0);
            cout << "Throughput: " << std::fixed << std::setprecision(0) << chars_per_second << " characters/second" << endl;
        } else {
            cout << "Could not open file: " << filename << endl;
            cout << "Continuing with string length tests..." << endl;
        }
    } 
}

// Function for collision testing
void collision_test() {
    
}

// Function for avalanche effect testing
void avalanche_test() {
    
}