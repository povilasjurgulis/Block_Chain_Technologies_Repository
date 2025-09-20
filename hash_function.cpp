#include "main.h"

string hash_function(string &key)
{
    // Pagrindiniai konstantos algoritmui (pirmieji skaičiai)
    const uint64_t PRIME1 = 0x9E3779B185EBCA87ULL;
    const uint64_t PRIME2 = 0xC2B2AE3D27D4EB4FULL;
    const uint64_t PRIME3 = 0x165667B19E3779F9ULL;
    const uint64_t PRIME4 = 0x85EBCA77C2B2AE63ULL;
    
    // Inicializuojame hash reikšmę
    uint64_t hash = PRIME1;
    
    // Pirmasis etapas: apdorojame kiekvieną baitą
    for (size_t i = 0; i < key.length(); i++) {
        uint64_t byte_val = static_cast<uint64_t>(key[i]);
        
        // Avalanche effect: kiekvienas baitas paveiks visus bitus
        hash ^= byte_val;
        hash *= PRIME2;
        hash ^= hash >> 33;
        hash *= PRIME3;
        hash ^= hash >> 29;
        hash *= PRIME4;
        hash ^= hash >> 32;
        
        // Papildomas maišymas su pozicija (deterministinis)
        hash ^= (i + 1) * PRIME1;
        hash = ((hash << 13) | (hash >> 51)); // Bit rotation
    }
    
    // Antrasis etapas: finalizavimas su papildomu maišymu
    hash ^= key.length() * PRIME2;
    hash *= PRIME1;
    hash ^= hash >> 33;
    hash *= PRIME3;
    hash ^= hash >> 29;
    hash *= PRIME4;
    hash ^= hash >> 32;
    
    // Trečiasis etapas: konvertuojame į hexadecimalų stringą
    char hex_chars[] = "0123456789abcdef";
    string hash_value;
    hash_value.reserve(16); // 64 bit = 16 hex simboliai
    
    for (int i = 15; i >= 0; i--) {
        hash_value += hex_chars[(hash >> (i * 4)) & 0xF];
    }
    
    return hash_value;
}