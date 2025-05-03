#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // process 6 characters at a time
        const int SIZE = 6;
        // up to 28 chars → at most 5 chunks
        const int NUM = 5;
        // values for each chunk
        HASH_INDEX_T w[NUM] = {0ULL};

        int len = static_cast<int>(k.length());
        // build chunk values from the end of the string backwards
        for (int chunk = 0; chunk < NUM; chunk++) {
            HASH_INDEX_T value = 0ULL;
            int end = len - chunk * SIZE;
            // no more chars for this chunk
            if (end <= 0) {
                break;  
            }
            int start = end - SIZE;
            if (start < 0) {
                start = 0;
            }
            // convert substring [start, end) from base-36 to decimal
            for (int i = start; i < end; ++i) {
                value = value * 36ULL + letterDigitToNumber(k[i]);
            }
            // store chunk value in reverse order: last chunk to w[4]
            w[NUM - 1 - chunk] = value;
        }

        // print chunk values
        for (int i = 0; i < NUM; ++i) {
                std::cout << "w[" << i << "] = " << w[i] << std::endl;
        }

        // combine all chunk values with the r multipliers
        HASH_INDEX_T hashValue = 0ULL;
        for (int i = 0; i < NUM; ++i) {
            hashValue += rValues[i] * w[i];
        }
        return hashValue;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // change uppercase to lowercase
        if (letter >= 'A' && letter <= 'Z') {
            letter = static_cast<char>(letter - 'A' + 'a');
        }
        if (letter >= 'a' && letter <= 'z') {
            return static_cast<HASH_INDEX_T>(letter - 'a');
        }
        if (letter >= '0' && letter <= '9') {
            return static_cast<HASH_INDEX_T>(letter - '0' + 26);
        }
        // for any other character (shouldn't occur), return 0
        return 0ULL;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
