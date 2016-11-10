#include "Vec_hash_done.h"
#include "hash_trial_done.h"
#include "Various_hashes_done.h"

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
        Identity_hash<size_t> ht(10000);
        hash_trial("Identity_hash", ht);
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
