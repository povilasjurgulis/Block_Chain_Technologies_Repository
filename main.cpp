#include <main.h>

/* Povilo Jurgulio 1 gr. 1 pogr. VU ISI 1 laboratorinis Block Chain darbas */
int main()
{
    std::string key; // bet koks zodis, kuri hash'insime
    char hash[16];
    unordered_map<string, string> hash_table;

    cout << "Input a random string: ";
    cin >> key;
    hash_table[key] = hash_function(key);
}