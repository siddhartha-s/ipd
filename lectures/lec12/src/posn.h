#pragma once

struct posn
{
    double x;
    double y;
};

// Computes the distance between two `posn`s.
double distance(posn, posn);
