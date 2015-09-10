#pragma once
#include <type_traits>
#include <ratio>

template<class Lhs, class Rhs>
struct unit_less : std::integral_constant<bool, Lhs::name_hash < Rhs::name_hash &&
									std::ratio_less<typename Lhs::prefix, typename Rhs::prefix>::value>
{};

template<class Lhs, class Rhs>
struct unit_equiv_ignore_prefix : std::integral_constant<bool, Lhs::name_hash == Rhs::name_hash>
{};

template<class Unit, class List, template<class, class> class Comparer>
struct find_remove_unit;

template<class Unit, template<class...> class List, template<class, class> class Comparer>
struct find_remove_unit<Unit, List<>, Comparer>
{
	constexpr static bool found = false;
	using remainedRatio = std::ratio<1>;
	using unitList = List<>;
};

template<class Unit, template<class...> class List, class First, class... Elements, template<class, class> class EqualityComparer>
struct find_remove_unit<Unit, List<First, Elements...>, EqualityComparer>
{
	constexpr static bool found = EqualityComparer<Unit, First>::value || find_remove_unit<Unit, List<Elements...>, EqualityComparer>::found;
	using remainedRatio = typename std::conditional<EqualityComparer<Unit, First>::value, std::ratio_divide<typename First::prefix, typename Unit::prefix>,
		typename find_remove_unit<Unit, List<Elements...>, EqualityComparer>::remainedRatio>::type;
	using unitList = typename std::conditional<EqualityComparer<Unit, First>::value, List<Elements...>,
		typename find_remove_unit<Unit, List<Elements...>, EqualityComparer>::unitList::template add_front_t<First>> ::type;
};

template<class Unit, class List, template<class, class> class Comparer>
struct insert_in_list;

template<class Unit, template<class...> class List, template<class, class> class Comparer>
struct insert_in_list<Unit, List<>, Comparer>
{
	using type = List<Unit>;
};

template<class Unit, template<class...> class List, class First, class... Elements, template<class, class> class Comparer>
struct insert_in_list<Unit, List<First, Elements...>, Comparer>
{
	using type = typename std::conditional < Comparer<Unit, First>::value, List<Unit, First, Elements...>,
		typename insert_in_list<Unit, List<Elements...>, Comparer>::type::template add_front_t < First >> ::type;
};

template<class Unit, class List, template<class, class> class Comparer>
using insert_in_list_t = typename insert_in_list<Unit, List, Comparer>::type;
