#include "Cons_list.h"

#include <stdexcept>

Int_cons::Int_cons(int elt, const Int_list& link)
    : first{elt}, rest{link}
{ }

Int_list cons(int elt, const Int_list& link)
{
    return std::make_shared<Int_cons>(elt, link);
}

int first(const Int_list& link)
{
    if (link == nullptr) throw std::logic_error{"first: empty"};
    return link->first;
}

const Int_list& rest(const Int_list& link)
{
    if (link == nullptr) throw std::logic_error{"rest: empty"};
    return link->rest;
}
