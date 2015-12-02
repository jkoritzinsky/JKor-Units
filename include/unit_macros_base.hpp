#pragma once
#define DEFINE_BASE_UNIT_COMPONENTS(name) \
template<class Prefix> \
struct name ## _component{ \
	using prefix = Prefix; \
	constexpr static const char* component_name = #name; \
	constexpr static unsigned long long name_hash = ::jkor::units::details::hashString(#name); \
}; \
using name ## _unit = ::jkor::units::unit<::jkor::units::details::typelist<name ## _component<std::ratio<1>>>>;

#define DEFINE_UNIT_UDLS(name, suffix) \
using name ## s = ::jkor::units::value<long long, name ## _unit>; \
constexpr name ## s operator""_v ## suffix(unsigned long long unit_value) \
{ \
	return name ## s{ unit_value }; \
} \
constexpr ::jkor::units::value<long double, name ## _unit> operator""_v ## suffix(long double unit_value) \
{ \
	return ::jkor::units::value<long double, name ## _unit>{ unit_value };\
}

#define DEFINE_BASE_UNIT(name, suffix) \
DEFINE_BASE_UNIT_COMPONENTS(name)\
DEFINE_UNIT_UDLS(name, suffix)

#define DEFINE_SCALED_UNIT(name, suffix, baseUnit, ...) \
using name ## _unit = ::jkor::units::unit_scale_t<baseUnit ## _unit, __VA_ARGS__>; \
DEFINE_UNIT_UDLS(name, suffix)

#define DEFINE_DERIVED_UNIT(name, suffix, compoundUnit) \
using name ## _unit = compoundUnit; \
DEFINE_UNIT_UDLS(name, suffix)