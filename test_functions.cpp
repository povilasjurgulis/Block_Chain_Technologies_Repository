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
    cout << "\n--- COLLISION SEARCH ---" << endl;
    cout << "Generating 100,000 random strings of different lengths..." << endl;
    
    int total_strings = 100000;
    int collisions_found = 0;
    std::set<string> unique_hashes; // Renkuosi seta unikaliems hashams, nes set automatiškai tvarko unikalumą
    std::map<string, string> hash_to_original;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> char_dist(0, 61);
    std::uniform_int_distribution<> length_dist(0, 3);
    
    string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int lengths[] = {10, 100, 500, 1000};
    
    auto start_time = high_resolution_clock::now();
    
    for (int i = 0; i < total_strings; i++) {
        int length_idx = length_dist(gen); // Atsitiktinis ilgis nuo 0 iki 3
        int str_length = lengths[length_idx]; // Pasirenkame ilgį iš predefined masyvo
        
        string random_str = "";
        random_str.reserve(str_length);
        
        for (int j = 0; j < str_length; j++) {
            random_str += charset[char_dist(gen)]; // Atsitiktinis simbolis is charset
        }
        
        string hash_result = hash_function(random_str);
        // Patikriname ar hash jau egzistuoja sete: unique_hashes.find(hash_result) yra lygus .end() jei nerasta tokio elemento sete 
        if (unique_hashes.find(hash_result) != unique_hashes.end()) { // Jei hash jau egzistuoja sete, radome koliziją
            collisions_found++;
            cout << "\n!!! COLLISION FOUND !!!" << endl;
            cout << "Hash: " << hash_result << endl;
            cout << "String 1: " << hash_to_original[hash_result] << endl;
            cout << "String 2: " << random_str << endl;
        } else {
            unique_hashes.insert(hash_result);
            hash_to_original[hash_result] = random_str;
        }
        
    }
    
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    
    cout << "\n--- COLLISION RESULTS ---" << endl;
    cout << "Total generated: " << total_strings << endl;
    cout << "Unique hashes: " << unique_hashes.size() << endl;
    cout << "Collisions found: " << collisions_found << endl;
    cout << "Test duration: " << duration.count() << " ms" << endl;
    
    if (collisions_found == 0) {
        cout << "EXCELLENT RESULT: No collisions found!" << endl;
    }
}

// Function for avalanche effect testing
void avalanche_test() {
    cout << "\n--- AVALANCHE EFFECT TEST ---" << endl;
    cout << "Generating 100,000 pairs differing by one character..." << endl;
    
    int total_pairs = 100000;
    std::vector<double> bit_differences;
    std::vector<double> hex_differences;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> char_dist(0, 61);
    std::uniform_int_distribution<> length_dist(10, 50);
    std::uniform_int_distribution<> pos_dist(0, 49);
    
    string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    auto start_time = high_resolution_clock::now();
    
    for (int i = 0; i < total_pairs; i++) {
        // Generate original string
        int str_length = length_dist(gen);
        string original = "";
        for (int j = 0; j < str_length; j++) {
            original += charset[char_dist(gen)];
        }
        
        // Create modified string (change one character)
        string modified = original;
        int change_pos = pos_dist(gen) % str_length;
        char new_char;
        do {
            new_char = charset[char_dist(gen)];
        } while (new_char == modified[change_pos]);
        modified[change_pos] = new_char;
        
        // Calculate hashes
        string hash1 = hash_function(original);
        string hash2 = hash_function(modified);
        
        // Analyze bit differences
        int bit_diff_count = 0;
        for (size_t j = 0; j < hash1.length(); j++) {
            int val1 = (hash1[j] >= '0' && hash1[j] <= '9') ? hash1[j] - '0' : hash1[j] - 'a' + 10;
            int val2 = (hash2[j] >= '0' && hash2[j] <= '9') ? hash2[j] - '0' : hash2[j] - 'a' + 10;
            int xor_result = val1 ^ val2;
            
            // Count different bits
            for (int bit = 0; bit < 4; bit++) {
                if (xor_result & (1 << bit)) {
                    bit_diff_count++;
                }
            }
        }
        
        // Count hex character differences
        int hex_diff_count = 0;
        for (size_t j = 0; j < hash1.length(); j++) {
            if (hash1[j] != hash2[j]) {
                hex_diff_count++;
            }
        }
        
        double bit_diff_percent = (double)bit_diff_count / (hash1.length() * 4) * 100;
        double hex_diff_percent = (double)hex_diff_count / hash1.length() * 100;
        
        bit_differences.push_back(bit_diff_percent);
        hex_differences.push_back(hex_diff_percent);
        
    }
    
    // Calculate statistics
    double bit_min = *std::min_element(bit_differences.begin(), bit_differences.end());
    double bit_max = *std::max_element(bit_differences.begin(), bit_differences.end());
    double bit_avg = 0;
    for (double diff : bit_differences) bit_avg += diff;
    bit_avg /= bit_differences.size();
    
    double hex_min = *std::min_element(hex_differences.begin(), hex_differences.end());
    double hex_max = *std::max_element(hex_differences.begin(), hex_differences.end());
    double hex_avg = 0;
    for (double diff : hex_differences) hex_avg += diff;
    hex_avg /= hex_differences.size();
    
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    
    cout << "\n--- AVALANCHE EFFECT RESULTS ---" << endl;
    cout << "Bit level:" << endl;
    cout << "  Minimum: " << std::fixed << std::setprecision(2) << bit_min << "%" << endl;
    cout << "  Maximum: " << bit_max << "%" << endl;
    cout << "  Average: " << bit_avg << "%" << endl;
    
    cout << "\nHex level:" << endl;
    cout << "  Minimum: " << hex_min << "%" << endl;
    cout << "  Maximum: " << hex_max << "%" << endl;
    cout << "  Average: " << hex_avg << "%" << endl;
    
    cout << "\nTest duration: " << duration.count() << " ms" << endl;
    
    if (bit_avg >= 45.0 && bit_avg <= 55.0) {
        cout << "EXCELLENT avalanche effect! (~50% bits change)" << endl;
    }

}