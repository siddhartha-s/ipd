#pragma once

#include <cstddef>

namespace codeword
{

struct codeword {
    size_t length      = 0;
    unsigned long bits = 0;
};

codeword
extend(codeword word, bool bit);

bool
is_full(codeword word);

}  // namespace codeword
