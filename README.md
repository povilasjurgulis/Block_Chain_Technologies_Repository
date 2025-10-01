# Hash Function Laboratorinis Darbas su didele AI pagalba

**Padarė:** Povilas Jurgulis  
**Grupė:** 1 grupė, 1 pogrupis  
**Universitetas:** VU ISI  

## Aprašymas

Šis projektas efektyviai įgyvendina hash funkciją, kuri atitinka visus reikalavimus:
- Deterministiškumas
- Avalanche effect (lavinos efektas)
- Negrįžtamumas
- Fiksuoto dydžio išvestis (256 bitai / 64 hex simboliai)

## Algoritmo savybės

Hash funkcija naudoja keturias nepriklausomas 64-bit hash sekas, kurios apjungiamos į vieną 256-bit rezultatą. Algoritmas:

1. **Inicializavimas:** Keturios hash reikšmės su pirminiais skaičiais
2. **Apdorojimas:** Kiekvienas įvesties baitas apdorojamas visose hash sekose
3. **Maišymas:** XOR, daugyba su pirminiais skaičiais, bit rotacija
4. **Užbaiginėjimas:** Papildomas maišymas su įvesties ilgiu
5. **Konvertavimas:** Konvertuojame į 64 hex simbolių eilutę

## Algoritmo idėja (pseudokodas)

```
FUNKCIJA hash_function(input_string)
    // 1. INICIALIZACIJA - keturi pirminiai skaičiai
    PRIME1 = 0x9E3779B185EBCA87
    PRIME2 = 0xC2B2AE3D27D4EB4F  
    PRIME3 = 0x165667B19E3779F9
    PRIME4 = 0x85EBCA77C2B2AE63
    
    // Keturios nepriklausomos 64-bit hash reikšmės
    hash1 = PRIME1
    hash2 = PRIME2
    hash3 = PRIME3
    hash4 = PRIME4
    
    // 2. APDOROJIMAS - kiekvienas baitas paveiks visas hash sekas
    CIKLUI i = 0 IKI input_string.length - 1
        byte_value = input_string[i]
        position = i + 1
        
        // Hash1 seka
        hash1 = hash1 XOR byte_value
        hash1 = hash1 * PRIME2
        hash1 = hash1 XOR (hash1 >> 33)
        hash1 = hash1 * PRIME3
        hash1 = hash1 XOR (hash1 >> 29)
        hash1 = hash1 XOR (position * PRIME1)
        hash1 = rotate_left(hash1, 13)
        
        // Hash2 seka (skirtingi parametrai)
        hash2 = hash2 XOR (byte_value * PRIME1)
        hash2 = hash2 * PRIME3
        hash2 = hash2 XOR (hash2 >> 31)
        hash2 = hash2 * PRIME4
        hash2 = hash2 XOR (hash2 >> 27)
        hash2 = hash2 XOR (position * PRIME2)
        hash2 = rotate_left(hash2, 17)
        
        // Hash3 seka (skirtingi parametrai)
        hash3 = hash3 XOR (byte_value * PRIME2)
        hash3 = hash3 * PRIME4
        hash3 = hash3 XOR (hash3 >> 35)
        hash3 = hash3 * PRIME1
        hash3 = hash3 XOR (hash3 >> 23)
        hash3 = hash3 XOR (position * PRIME3)
        hash3 = rotate_left(hash3, 19)
        
        // Hash4 seka (skirtingi parametrai)
        hash4 = hash4 XOR (byte_value * PRIME3)
        hash4 = hash4 * PRIME1
        hash4 = hash4 XOR (hash4 >> 37)
        hash4 = hash4 * PRIME2
        hash4 = hash4 XOR (hash4 >> 25)
        hash4 = hash4 XOR (position * PRIME4)
        hash4 = rotate_left(hash4, 23)
    CIKLO_PABAIGA
    
    // 3. UŽBAIGINĖJIMAS - papildomas maišymas su input ilgiu
    input_length = input_string.length
    
    hash1 = hash1 XOR (input_length * PRIME2)
    hash1 = hash1 * PRIME1
    hash1 = hash1 XOR (hash1 >> 33)
    hash1 = hash1 * PRIME3
    hash1 = hash1 XOR (hash1 >> 29)
    
    hash2 = hash2 XOR (input_length * PRIME3)
    hash2 = hash2 * PRIME2
    hash2 = hash2 XOR (hash2 >> 31)
    hash2 = hash2 * PRIME4
    hash2 = hash2 XOR (hash2 >> 27)
    
    hash3 = hash3 XOR (input_length * PRIME4)
    hash3 = hash3 * PRIME3
    hash3 = hash3 XOR (hash3 >> 35)
    hash3 = hash3 * PRIME1
    hash3 = hash3 XOR (hash3 >> 25)
    
    hash4 = hash4 XOR (input_length * PRIME1)
    hash4 = hash4 * PRIME4
    hash4 = hash4 XOR (hash4 >> 37)
    hash4 = hash4 * PRIME2
    hash4 = hash4 XOR (hash4 >> 23)
    
    // 4. KONVERTAVIMAS Į HEX STRING
    result = ""
    hash_array = [hash1, hash2, hash3, hash4]
    
    CIKLUI j = 0 IKI 3  // Keturios hash reikšmės
        current_hash = hash_array[j]
        CIKLUI k = 15 IKI 0  // 16 hex simbolių kiekvienai hash reikšmei
            hex_digit = (current_hash >> (k * 4)) AND 0xF
            result = result + hex_chars[hex_digit]
        CIKLO_PABAIGA
    CIKLO_PABAIGA
    
    GRĄŽINTI result  // 64 hex simbolių string (256 bitai)
FUNKCIJOS_PABAIGA

// Pagalbinė funkcija bit rotacijai
FUNKCIJA rotate_left(value, positions)
    GRĄŽINTI (value << positions) OR (value >> (64 - positions))
FUNKCIJOS_PABAIGA
```

### Algoritmo esmė žingsnis po žingsnio:

1. **Keturios nepriklausomos hash sekos** - kiekviena naudoja skirtingus parametrus
2. **Pozicijos įtaka** - kiekvieno simbolio pozicija paveiks hash rezultatą  
3. **Avalanche effect** - XOR, daugyba, bit shifting sukuria lavinos efektą
4. **Negrįžtamumas** - kelių etapų maišymas su informacijos praradimais
5. **Fiksuotas dydis** - visada 256 bitai nepriklausomai nuo įvesties
