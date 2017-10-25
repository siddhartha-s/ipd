#pragma once

// Represents a 2-D position.
struct posn
{
    double x;
    double y;
};

// Computes the distance between two `posn`s.
double distance(posn, posn);
