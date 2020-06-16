#ifndef SORT_H_
#define SORT_H_

#include <nlohmann/json.hpp>
#include "arg.h"

bool sort_chapters(nlohmann::json& j, struct arg_struct& as);

bool print_chapters(nlohmann::json& j, struct arg_struct& as);

#endif // SORT_H_
