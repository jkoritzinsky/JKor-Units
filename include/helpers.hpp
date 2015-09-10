#pragma once
#include <type_traits>

template<bool...>
struct bool_list {};

template<bool... Bools>
using all_true = std::is_same<bool_list<true, Bools...>, bool_list<Bools..., true>>;
