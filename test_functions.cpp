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