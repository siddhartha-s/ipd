#include "Various_hashes.h"
#include <iostream>

const size_t bytes_to_hash = 8;

int main()
{
    Sbox_hash<size_t> ht;

    std::string input(bytes_to_hash, 0);
    for (int    b             = 0; b < bytes_to_hash; b++) {
        std::cin >> input[b];
    }
    size_t      original_hash = ht.hash(input);

    for (int i = 0; i < input.size() * CHAR_BIT; i++) {
        std::cout << (i == 0 ? "((" : ")\n (");
        int byte = i / CHAR_BIT;
        int bit  = i % CHAR_BIT;
        input[byte] ^= (1 << bit);
        size_t this_hash = ht.hash(input);
        input[byte] ^= (1 << bit);
        size_t      differences = original_hash ^ this_hash;
        for (size_t j           = 0; j < bytes_to_hash * CHAR_BIT; j++) {
            if (j != 0) std::cout << " ";
            size_t mask = (size_t) 1 << j;
            std::cout << ((differences & mask) != 0);
        }
    }
    std::cout << "))\n";
    return 0;
}
