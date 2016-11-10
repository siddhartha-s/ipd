#include "Vec_hash_done.h"
#include "hash_trial_done.h"
#include "Various_hashes_done.h"

#include <sstream>

int main()
{
    for (size_t mixer = 1; mixer <= 128; ++mixer) {
        std::ostringstream name;
        name << "Simple_mix(" << mixer << ")";
        Simple_mix<size_t> ht(10000, mixer);
        hash_trial(name.str(), ht);
    }
}
