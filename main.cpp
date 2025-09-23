#include "main.h"
#include <fstream>
#include <sstream>

/* Povilo Jurgulio 1 gr. 1 pogr. VU ISI 1 laboratorinis Block Chain darbas */
/* Darbas darytas grupėje su Eligijumi ir aš dariau labiau su AI/Copilot, o Elgijus labiau pats*/
int main(int argc, char* argv[])
{
    std::string key; // bet koks zodis, kuri hash'insime
    unordered_map<string, string> hash_table;
    
    // Tikriname, ar yra command line argumentas
    if (argc > 1) {
        // Naudojame command line argumentą kaip failo pavadinimą
        string filename = argv[1];
        ifstream file(filename);
        
        if (!file.is_open()) {
            cout << "Nepavyko atidaryti failo: " << filename << endl;
            return 1;
        }
        
        // Nuskaitome visą failo turinį
        std::stringstream buffer;
        buffer << file.rdbuf();
        key = buffer.str();
        
        file.close();
        cout << "IVEDIMAS PER COMMAND LINE ARGUMENTA\n";
        cout << "Nuskaitytas failas: " << filename << endl;
    } else {
        // Jei nėra argumentų, prašome įvesti tekstą ranka
        cout << "IVEDIMAS RANKA\n";
        cout << "Input a random string: ";
        getline(cin, key); // Naudojame getline, kad nuskaitytų visą eilutę su tarpais
    }
    
    string hash_result = hash_function(key);
    hash_table[key] = hash_result;
    
    cout << "\n";
    cout << "Original string: " << key << endl;
    cout << "Hash result: " << hash_result << endl;
    cout << "Hash length: " << hash_result.length() << " simboliu (64 bitai)" << endl;
    
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