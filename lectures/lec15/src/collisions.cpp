#include "Vec_hash.h"
#include "Various_hashes.h"
#include "hamlet.h"

void try_it(Vec_hash<size_t>& h)
{
    for (std::string line : get_hamlet()) {
        h.add(line, 0);
    }
    h.how_are_we_doing();
}

int main()
{
    Vec_hash<size_t> h;
    try_it(h);
    Identity_hash<size_t> id;
    try_it(id);
    Simple_mix<size_t> sm;
    try_it(sm);
    Sbox_hash<size_t> sh;
    try_it(sh);
    return 0;
}
