#pragma once

#include <array>
#include <string>

class Sbox_hash
{
public:
    Sbox_hash();
    size_t operator()(const std::string&) const;

private:
    std::array<size_t, 256> sbox_;
};