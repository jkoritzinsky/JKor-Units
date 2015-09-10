#pragma once
#include <type_traits>
#include "unit_base.hpp"
#include "unit_macros_base.hpp"
#include "HashedString.hpp"
#include <ratio>
//Split the definition of the second unit into two parts so I can add integration with <chrono> into the value class.
#include <chrono>
DEFINE_BASE_UNIT_COMPONENTS(second)

template<class Rep, class Unit>
class value;

template<class To, class Rep, class Unit, class = typename std::enable_if<units_compatible<typename To::unit, Unit>::value>::type>
To unit_cast(value<Rep, Unit> val);

template <class Rep = long long, class Unit = unit<>>
class value {
public:
	using unit = Unit;
	using value_type = Rep;
private:
	value_type val;
public:
	constexpr value() = default;
	constexpr value(const value&) = default;
	template <class Rep2, class = typename std::enable_if<std::is_convertible<Rep2, value_type>::value
		&& (std::chrono::treat_as_floating_point<value_type>::value
			|| !std::chrono::treat_as_floating_point<Rep2>::value)>::type>
		constexpr explicit value(const Rep2& val) : val(static_cast<value_type>(val)) {}

	template <class Rep2, class Unit2, class = typename std::enable_if<units_compatible<unit, Unit2>::value>::type,
	class = typename std::enable_if<std::chrono::treat_as_floating_point<value_type>::value ||
		(unit_divide<unit, Unit2>::remainingRatio::den == 1
			&& !std::chrono::treat_as_floating_point<Rep2>::value)>::type>
		constexpr value(const value<Rep2, Unit2>& other) : val(unit_cast<value>(other).count()) {}

	template<class Rep2, class Period>
	constexpr value(const std::chrono::duration<Rep2, Period>& duration) :
		value(value<Rep2, unit_scale_t<second_unit, Period>>{duration.count()}) {}

	Rep count()
	{
		return val;
	}

	template<class Rep2, class Unit2>
	value<typename std::common_type<Rep, Rep2>::type, unit_multiply_t<Unit, Unit2>> operator*(value<Rep2, Unit2> rhs)
	{
		return value<typename std::common_type<Rep, Rep2>::type, unit_multiply_t<Unit, Unit2>>{count() * rhs.count()};
	}

	template<class Rep2, class Unit2>
	value<typename std::common_type<Rep, Rep2>::type, unit_multiply_t<Unit, Unit2>> operator/(value<Rep2, Unit2> rhs)
	{
		return value<typename std::common_type<Rep, Rep2>::type, unit_multiply_t<Unit, Unit2>>{count() / rhs.count()};
	}
};

DEFINE_UNIT_UDLS(second, s)
DEFINE_SCALED_UNIT(minute, min, second, std::ratio<60>)
DEFINE_SCALED_UNIT(hour, hr, minute, std::ratio<60>)
