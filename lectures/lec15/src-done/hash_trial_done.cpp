#include "hash_trial_done.h"
#include "hamlet.h"

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


