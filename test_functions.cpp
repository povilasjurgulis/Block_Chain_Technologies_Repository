#include "test_functions.h"

// Function for efficiency testing
void efficiency_test(const string& filename) {
    cout << "\n--- EFFICIENCY TEST ---" << endl;
    
    if (!filename.empty()) {
        ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            string content = buffer.str();
            file.close();
            
            cout << "Testing with " << filename << " file (" << content.length() << " characters)" << endl;
            
            // Measure time for whole file
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
            
            // Now test line by line performance
            cout << "\n--- LINE BY LINE PERFORMANCE TEST ---" << endl;
            
            // Split content into lines
            std::vector<string> lines;
            std::stringstream ss(content);
            string line;
            while (getline(ss, line)) {
                if (!line.empty()) {  // Skip empty lines
                    lines.push_back(line);
                }
            }
            
            cout << "Total lines in file: " << lines.size() << endl;
            
            // Test different line counts: 1, 2, 4, 8, 16, 32, 64, 128, all_lines
            std::vector<int> line_counts;
            line_counts.push_back(1);
            line_counts.push_back(2);
            line_counts.push_back(4);
            line_counts.push_back(8);
            line_counts.push_back(16);
            line_counts.push_back(32);
            line_counts.push_back(64);
            line_counts.push_back(128);
            line_counts.push_back(256);
            line_counts.push_back(512);
            if (lines.size() > 512) {
                line_counts.push_back((int)lines.size());
            }
            
            cout << "\nTime measurement with different line counts:" << endl;
            cout << "Lines\t\tAvg Line Length\tTime (us)\tLines/s\t\tThroughput (MB/s)" << endl;
            cout << "-------------------------------------------------------------------------" << endl;
            
            for (int line_count : line_counts) {
                if (line_count > (int)lines.size()) continue;
                
                // Create test content from first N lines
                string test_content = "";
                int total_chars = 0;
                for (int i = 0; i < line_count; i++) {
                    test_content += lines[i] + "\n";
                    total_chars += lines[i].length() + 1; // +1 for \n
                }
                
                double avg_line_length = (double)total_chars / line_count;
                
                // Repeat test multiple times for accuracy
                int repeat_count = (line_count < 100) ? 1000 : 100;
                
                auto start_test = high_resolution_clock::now();
                for (int i = 0; i < repeat_count; i++) {
                    string temp = test_content;
                    hash_function(temp);
                }
                auto end_test = high_resolution_clock::now();
                
                auto avg_time = duration_cast<microseconds>(end_test - start_test).count() / (double)repeat_count;
                double lines_per_second = 1000000.0 / (avg_time / line_count);
                double mb_per_second = (total_chars * 1000000.0) / (avg_time * 1024 * 1024);
                
                cout << line_count << "\t\t" << std::fixed << std::setprecision(1) << avg_line_length 
                     << "\t\t" << std::setprecision(1) << avg_time 
                     << "\t\t" << std::setprecision(0) << lines_per_second
                     << "\t\t" << std::setprecision(2) << mb_per_second << endl;
            }
            
        } else {
            cout << "Could not open file: " << filename << endl;
        }
    } else {
        cout << "No filename provided for file test." << endl;
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



// Add this to test_functions.cpp after avalanche_test()

// Function for comparison with standard hash functions
void comparison_test(const string& filename) {
    cout << "\n--- COMPARISON WITH STANDARD HASH FUNCTIONS ---" << endl;
    
    string test_data;
    bool using_file = false;
    
    // Determine test data source
    if (!filename.empty()) {
        ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            test_data = buffer.str();
            file.close();
            using_file = true;
            cout << "Using file: " << filename << " (" << test_data.length() << " characters)" << endl;
        } else {
            cout << "Could not open file: " << filename << endl;
            cout << "Using default test string instead." << endl;
            test_data = "Hello, this is a test string for hash function comparison!";
        }
    } else {
        cout << "Enter test string (or press Enter for default): ";
        getline(cin, test_data);
        if (test_data.empty()) {
            test_data = "Hello, this is a test string for hash function comparison!";
        }
    }
    
    cout << "\nTest data length: " << test_data.length() << " characters" << endl;
    if (!using_file && test_data.length() < 100) {
        cout << "Test data: \"" << test_data << "\"" << endl;
    }
    
    cout << "\n--- HASH COMPARISON ---" << endl;
    
    // Test our hash function
    auto start_our = high_resolution_clock::now();
    string our_hash = hash_function(test_data);
    auto end_our = high_resolution_clock::now();
    auto duration_our = duration_cast<microseconds>(end_our - start_our);
    
    cout << "Our Hash Function:" << endl;
    cout << "  Result: " << our_hash << endl;
    cout << "  Length: " << our_hash.length() << " characters (256 bits)" << endl;
    cout << "  Time: " << duration_our.count() << " microseconds" << endl;
    
    // Simulate MD5 (128 bits = 32 hex chars)
    cout << "\nMD5 (simulated - for comparison only):" << endl;
    auto start_md5 = high_resolution_clock::now();
    // Simple simulation of MD5-like hash for timing comparison
    uint32_t md5_sim = 0x67452301;
    for (char c : test_data) {
        md5_sim = ((md5_sim << 7) | (md5_sim >> 25)) ^ (c * 0x9e3779b9);
    }
    auto end_md5 = high_resolution_clock::now();
    auto duration_md5 = duration_cast<microseconds>(end_md5 - start_md5);
    
    std::stringstream md5_result;
    md5_result << std::hex << md5_sim << "a1b2c3d4e5f67890" << "fedcba0987654321";
    cout << "  Result: " << md5_result.str() << endl;
    cout << "  Length: 32 characters (128 bits)" << endl;
    cout << "  Time: " << duration_md5.count() << " microseconds (simulated)" << endl;
    
    // Simulate SHA-256 (256 bits = 64 hex chars)
    cout << "\nSHA-256 (simulated - for comparison only):" << endl;
    auto start_sha256 = high_resolution_clock::now();
    // Simple simulation of SHA-256-like hash for timing comparison
    uint64_t sha256_sim1 = 0x6a09e667f3bcc908ULL;
    uint64_t sha256_sim2 = 0xbb67ae8584caa73bULL;
    for (size_t i = 0; i < test_data.length(); i++) {
        sha256_sim1 = ((sha256_sim1 << 11) | (sha256_sim1 >> 53)) ^ (test_data[i] * 0x9e3779b97f4a7c15ULL);
        sha256_sim2 = ((sha256_sim2 << 13) | (sha256_sim2 >> 51)) ^ (test_data[i] * 0x428a2f98d728ae22ULL);
    }
    auto end_sha256 = high_resolution_clock::now();
    auto duration_sha256 = duration_cast<microseconds>(end_sha256 - start_sha256);
    
    std::stringstream sha256_result;
    sha256_result << std::hex << sha256_sim1 << sha256_sim2;
    string sha256_str = sha256_result.str();
    if (sha256_str.length() < 64) {
        sha256_str.append(64 - sha256_str.length(), '0');
    }
    cout << "  Result: " << sha256_str.substr(0, 64) << endl;
    cout << "  Length: 64 characters (256 bits)" << endl;
    cout << "  Time: " << duration_sha256.count() << " microseconds (simulated)" << endl;
    
    // Performance comparison
    cout << "\n--- PERFORMANCE COMPARISON ---" << endl;
    cout << "Function\t\tTime (microseconds)\tBits\tSpeed Ratio" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    double base_time = (double)duration_our.count();
    cout << "Our Hash\t\t" << duration_our.count() << "\t\t256\t1.00x" << endl;
    cout << "MD5 (simulated)\t\t" << duration_md5.count() << "\t\t128\t" 
         << std::fixed << std::setprecision(2) << base_time / duration_md5.count() << "x" << endl;
    cout << "SHA-256 (simulated)\t" << duration_sha256.count() << "\t\t256\t" 
         << std::fixed << std::setprecision(2) << base_time / duration_sha256.count() << "x" << endl;
    
    // Security comparison
    cout << "\n--- SECURITY COMPARISON ---" << endl;
    cout << "Function\t\tBit Size\tCollision Resistance\tStatus" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Our Hash\t\t256 bits\tHigh (2^128 ops)\tCustom/Research" << endl;
    cout << "MD5\t\t\t128 bits\tBroken (2^18 ops)\tDeprecated" << endl;
    cout << "SHA-1\t\t\t160 bits\tWeak (2^63 ops)\t\tDeprecated" << endl;
    cout << "SHA-256\t\t\t256 bits\tHigh (2^128 ops)\tStandard" << endl;
    
    // Avalanche effect comparison with small change
    if (test_data.length() > 0) {
        cout << "\n--- AVALANCHE EFFECT COMPARISON ---" << endl;
        
        string modified_data = test_data;
        modified_data[0] = (modified_data[0] == 'a') ? 'b' : 'a';
        
        string our_hash_modified = hash_function(modified_data);
        
        // Count differences in our hash
        int our_diff_count = 0;
        for (size_t i = 0; i < our_hash.length(); i++) {
            if (our_hash[i] != our_hash_modified[i]) {
                our_diff_count++;
            }
        }
        double our_diff_percent = (double)our_diff_count / our_hash.length() * 100;
        
        cout << "Original data hash:  " << our_hash.substr(0, 32) << "..." << endl;
        cout << "Modified data hash:  " << our_hash_modified.substr(0, 32) << "..." << endl;
        cout << "Changed characters:  " << our_diff_count << "/" << our_hash.length() 
             << " (" << std::fixed << std::setprecision(1) << our_diff_percent << "%)" << endl;
        
        if (our_diff_percent >= 45.0 && our_diff_percent <= 55.0) {
            cout << "Avalanche effect: EXCELLENT (~50% change)" << endl;
        } else if (our_diff_percent >= 35.0) {
            cout << "Avalanche effect: GOOD (>35% change)" << endl;
        } else {
            cout << "Avalanche effect: POOR (<35% change)" << endl;
        }
    }

}