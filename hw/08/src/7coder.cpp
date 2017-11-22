#include "7coder.h"

using namespace std;
using namespace ipd;

void encode(istream& in, bostream& out)
{
    char c;

    while (in.read(&c, 1)) {
        out.write_bits(c, 7);
    }
}

void decode(bistream& in, ostream& out)
{
    char c;

    while (in.read_bits(c, 7)) {
        out << c;
    }
}
