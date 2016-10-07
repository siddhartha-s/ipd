#pragma once

#include <memory>
#include <string>
#include <vector>

namespace islpp
{

struct Struct_id
{
    std::vector<std::string> fields;
};

using struct_id_ptr = std::shared_ptr<Struct_id>;

}
