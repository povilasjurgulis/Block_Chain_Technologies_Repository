#include "main.h"

/* Povilo Jurgulio 1 gr. 1 pogr. VU ISI 1 laboratorinis Block Chain darbas */
/* Darbas darytas grupėje su Eligijumi ir aš dariau labiau su AI/Copilot, o Elgijus labiau pats*/
int main()
{
    std::string key; // bet koks zodis, kuri hash'insime
    unordered_map<string, string> hash_table;

    cout << "Input a random string: ";
    cin >> key;
    
    string hash_result = hash_function(key);
    hash_table[key] = hash_result;
    
    cout << "Original string: " << key << endl;
    cout << "Hash result: " << hash_result << endl;
    
    // Demonstracija avalanche effect
    cout << "\nDemonstrating avalanche effect:" << endl;
    if (key.length() > 0) {
        string modified_key = key;
        modified_key[0] = (modified_key[0] == 'a') ? 'b' : 'a'; // Pakeičiame pirmą simbolį
        cout << "Modified string: " << modified_key << endl;
        cout << "Modified hash: " << hash_function(modified_key) << endl;
    }
    
    return 0;
}