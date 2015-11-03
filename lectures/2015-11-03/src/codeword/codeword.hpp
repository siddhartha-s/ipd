#pragma once

#include <cctype>

namespace codeword
{

struct codeword {
    size_t length;
    unsigned long bits;
};

codeword
extend(codeword word, bool bit);

bool
is_full(codeword word);

}  // namespace codeword
