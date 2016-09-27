#pragma once

#include <memory>

struct Int_cons;

using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    const int first;
    const Int_list rest;

    Int_cons(int, const Int_list&);
};

Int_list cons(int, const Int_list&);
int first(const Int_list&);
const Int_list& rest(const Int_list&);
