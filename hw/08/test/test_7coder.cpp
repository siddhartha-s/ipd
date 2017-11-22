#include "7coder.h"
#include <UnitTest++/UnitTest++.h>

#include <sstream>

using namespace std;
using namespace ipd;

void round_trip(string const& msg)
{
    istringstream is(msg);
    bostringstream boss;
    encode(is, boss);

    bistringstream biss(boss.data());
    ostringstream os;
    decode(biss, os);

    CHECK_EQUAL(msg, os.str());
}

TEST(Round_trip_empty_string) {
    round_trip("");
}

TEST(Round_trip_A) {
    round_trip("A");
}

TEST(Round_trip_hello) {
    round_trip("hello");
}

TEST(Round_trip_Hello_world) {
    round_trip("Hello, world!");
}
