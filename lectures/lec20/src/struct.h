#pragma once

#include "symbol.h"

#include <memory>
#include <string>
#include <vector>

namespace islpp
{

struct Struct_id
{
    Symbol name;
    std::vector<Symbol> fields;
};

using struct_id_ptr = std::shared_ptr<Struct_id>;

}
