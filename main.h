#ifndef MAIN_H
#define MAIN_H

#include <iostream> // cout ir t.t.
#include <unordered_map> // hash
#include <chrono> // laikui
#include <random> // rng
#include <string> // string
#include <cstdint> // uint64_t

using std::unordered_map; using std::cout; using std::ifstream; using std::endl;
using namespace std::chrono; using std::random_device; using std::mt19937; using std::cin;
using std::string;

string hash_function(string &key);

#endif
