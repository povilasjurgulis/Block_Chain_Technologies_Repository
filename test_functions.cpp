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
    } else {
        cout << "File test skipped." << endl;
    }

    // Test with different length strings
    std::vector<int> test_lengths = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    cout << "\nTime measurement with different length strings:" << endl;
    cout << "Length\t\tTime mikroseconds\tFiles/s" << endl;
    cout << "------------------------------------------------" << endl;

    for (int length : test_lengths) {
        string test_string(length, 'a');

        // Repeat 1000 times for more precise measurement
        auto start = high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            string temp = test_string; // Create copy to avoid reference issues
            hash_function(temp);
        }
        auto end = high_resolution_clock::now();

        auto avg_time = duration_cast<microseconds>(end - start).count() / 1000.0;
        double files_per_second = 1000000.0 / avg_time; // 1 second = 1,000,000 us

        cout << length << "\t\t" << std::fixed << std::setprecision(1) 
             << avg_time << "\t\t\t" << std::setprecision(0) << files_per_second << endl;
    } 
}

// Function for collision testing
void collision_test() {
    
}

// Function for avalanche effect testing
void avalanche_test() {
    
}