#pragma once

#include <memory>

struct Int_cons
{
    using link_t = std::shared_ptr<Int_cons>;

    Int_cons(int, const link_t&);

    const int first;
    const link_t rest;
};

using Int_list = Int_cons::link_t;

Int_list cons(int, const Int_list&);
int first(const Int_list&);
const Int_list& rest(const Int_list&);
