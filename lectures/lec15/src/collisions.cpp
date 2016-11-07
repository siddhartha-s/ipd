#include "Vec_hash.h"
#include "Various_hashes.h"

void hash_trial(std::string name, Vec_hash<size_t>& h)
{
    std::vector<std::string> hamlet = get_hamlet();
    size_t                   i      = 0;
    for (std::string         s : hamlet) {
        h.add(s, i);
        i++;
    }
    size_t                   coll   = h.collisions();

    std::cout << "size: "
              << h.table_size()
              << " collisions: "
              << coll
              << " "
              << name
              << "\n";
}


int main()
{
    {
        Vec_hash<size_t> ht(10000);
        hash_trial("Vec_hash", ht);
    }

    {
        One_byte<size_t> ht(10000);
        hash_trial("One_byte", ht);
    }

    {
        Eight_bytes<size_t> ht(10000);
        hash_trial("Eight_bytes", ht);
    }

    {
        Simple_mix<size_t> ht(10000);
        hash_trial("Simple_mix", ht);
    }

    {
        Sbox_hash<size_t> ht(10000);
        hash_trial("SBox_hash", ht);
    }

    return 0;
}
