#include "codeword.hpp"

#include <climits>

#include "gsl/gsl.h"

namespace codeword
{

codeword
extend(codeword old, bool bit)
{
    Expects(!is_full(old));

    return codeword{old.length + 1, (old.bits << 1) + bit};
}

bool is_full(codeword word)
{
    return word.length == CHAR_BIT * sizeof word.bits;
}

} // namespace codeword
