#include "bit_io.h"
#include "common.h"

#include <iostream>
#include <cstdio>

// These are the default files to read from and write to when no
// command-line arguments are given:
#define DEFAULT_INFILE  "hamlet.txt"
#define DEFAULT_OUTFILE "hamlet.txt.huff"

using namespace ipd;
using namespace std;

void encode(istream& in, bofstream& out);

int main(int argc, const char *argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    ifstream in(infile);
    assert_good(in, argv);

    bofstream out(outfile);
    assert_good(out, argv);

    encode(in, out);
}

void encode(istream& in, bofstream& out)
{
    char c;

    while (in.read(&c, 1)) {
        out.write_bits(c, 7);
    }
}
