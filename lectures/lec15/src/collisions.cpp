#include "Vec_hash.h"
#include "Various_hashes.h"
#include "hamlet.h"

void try_it(Vec_hash<size_t>& ht)
{
    for (std::string line : get_hamlet()) {
        ht.add(line, 0);
    }
    ht.how_are_we_doing();
}

int main()
{
    Vec_hash<size_t> h;
    std::cout << "Vec_hash\n";
    try_it(h);
    std::cout << "\n";

    Identity_hash<size_t> id;
    std::cout << "Identity_hash\n";
    try_it(id);
    std::cout << "\n";

    Simple_mix<size_t> sm;
    std::cout << "Simple_mix\n";
    try_it(sm);
    std::cout << "\n";

    Sbox_hash<size_t> sh;
    std::cout << "Sbox_hash\n";
    try_it(sh);

    return 0;
}
