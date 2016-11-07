#include "Various_hashes.h"
#include <iostream>

const size_t bytes_to_hash=8;

int main()
{
    Sbox_hash<size_t> ht;

    std::string input(bytes_to_hash, 0);
    for (int    b             = 0; b < bytes_to_hash; b++) {
        std::cin >> input[b];
        std::cout << "; got " << input[b] << "\n";
    }
    size_t      original_hash = ht.hash(input);
    unsigned char uc, orig;

    for (int byte = 0; byte < input.size(); byte++) {
        for (int i = 0; i < CHAR_BIT; i++) {
            if (byte || i) {
                std::cout << " (";
            } else {
                std::cout << "((";
            }
            uc   = (unsigned char) input[byte];
            orig = uc;
            uc ^= (1 << i);
            input[byte] = uc;
            size_t this_hash = ht.hash(input);
            input[byte] = orig;
            size_t      differences = original_hash ^this_hash;
            for (size_t j           = 0; j < bytes_to_hash * CHAR_BIT; j++) {
                if (j != 0) std::cout << " ";
                size_t mask = (size_t) 1 << j;
                std::cout << ((differences & mask) != 0);
            }
            std::cout << ")\n";
        }
    }
    std::cout << ")";
    return 0;
}
