#pragma once

#include "value.h"
#include "env.h"

namespace islpp {

namespace primop {

extern env_ptr<value_ptr> environment;

extern value_ptr cons;
extern value_ptr plus;
extern value_ptr num_eq;
extern value_ptr equal_huh;

}

}
