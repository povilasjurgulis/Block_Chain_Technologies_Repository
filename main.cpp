#include "main.h"
#include "test_functions.h"
#include <fstream>
#include <sstream>

/* Povilo Jurgulio 1 gr. 1 pogr. VU ISI 1 laboratorinis Blokų Grandinių Technologijų nr. 1 darbas */
/* Darbas darytas grupeje su Eligijumi Alenčiku ir aš dariau labiau su AI/Copilot, o Elgijus labiau pats*/

int main(int argc, char* argv[])
{
    std::string key; // bet koks zodis, kuri hash'insime
    unordered_map<string, string> hash_table;
    
    bool is_file_input = false;
    string filename = ""; // Pridedu kintamaji failo pavadinimui
    
    // Tikriname, ar yra command line argumentas
    if (argc > 1) {
        is_file_input = true;
        // Naudojame command line argumenta kaip failo pavadinima
        filename = argv[1]; // Issaugome failo pavadinima
        ifstream file(filename);
        
        if (!file.is_open()) {
            cout << "Could not open file: " << filename << endl;
            return 1;
        }
        
        // Nuskaitome visa failo turini
        std::stringstream buffer;
        buffer << file.rdbuf();
        key = buffer.str();
        
        file.close();
        cout << "\nINPUT VIA COMMAND LINE ARGUMENT\n";
        cout << "File read: " << filename << endl;
        cout << "File size: " << key.length() << " characters" << endl;
    } else {
        // Jei nera argumentu, prasome ivesti teksta ranka
        cout<<"--------------------------\n";
        cout << "Input anything: ";
        getline(cin, key); // Naudojame getline, kad nuskaitytų visa eilute su tarpais
    }
    
    // Matuojame hash funkcijos vykdymo laika
    auto start_time = high_resolution_clock::now();
    string hash_result = hash_function(key);
    auto end_time = high_resolution_clock::now();
    
    // Skaiciuojame trukme mikrosekundemis ir milisekundemis
    auto duration_microseconds = duration_cast<microseconds>(end_time - start_time);
    auto duration_milliseconds = duration_cast<milliseconds>(end_time - start_time);
    
    hash_table[key] = hash_result;
    
    if (!is_file_input) {
        cout << "Inputted string: " << key << endl;
    } else {
        cout << "File content hashed (content not shown due to size)" << endl;
    }
    cout << "Hash result: " << hash_result << endl;
    cout << "Hash length: " << hash_result.length() << " characters (256 bits)" << endl;
    cout << "Hash calculation time: " << duration_microseconds.count() 
         << " microseconds (" << duration_milliseconds.count() << " ms)" << endl;
    cout << "\n";
    
    // Demonstracija avalanche effect tik rankiniam ivedimui
    if (!is_file_input) {
        cout << "\nDemonstrating avalanche effect:" << endl;
        if (key.length() > 0) {
            string modified_key = key;
            modified_key[0] = (modified_key[0] == 'a') ? 'b' : 'a'; // Pakeiciame pirma simboli
            cout << "Modified string: " << modified_key << endl;
            
            // Matuojame modifikuoto string hash laika
            auto start_modified = high_resolution_clock::now();
            string modified_hash = hash_function(modified_key);
            auto end_modified = high_resolution_clock::now();
            auto duration_modified = duration_cast<microseconds>(end_modified - start_modified);
            
            cout << "Modified hash: " << modified_hash << endl;
            cout << "Modified hash time: " << duration_modified.count() << " microseconds" << endl;
        }
    } else {
        cout << "\n(Avalanche effect demonstration skipped for file input)" << endl;
    } 
    
    // TESTS
    // Uncomment the tests you want to run:
    
    // Test 4: Efficiency measurement - now passes filename from command line
    // efficiency_test(filename);
    
    // Test 5: Collision search
    // collision_test();
    
    // Test 6: Avalanche effect analysis
    // avalanche_test();
    
    // Test 7: Comparison with standard hash functions
    // comparison_test(filename);

    return 0;
}