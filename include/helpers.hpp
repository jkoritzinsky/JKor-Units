#pragma once
#include <type_traits>
#ifdef __cpp_fold_expressions
template<bool... Bools>
using all_true = std::integral_constant<bool, (Bools & ...)>;
#else
template<bool...>
struct bool_list {};

template<bool... Bools>
using all_true = std::is_same<bool_list<true, Bools...>, bool_list<Bools..., true>>;
#endif