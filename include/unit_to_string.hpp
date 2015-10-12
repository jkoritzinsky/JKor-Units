#pragma once
#include "typelist.hpp"
#include <ratio>
#include "unit_base.hpp"
#include <string>

template<class Ratio>
struct ratio_to_prefix
{
    static const char* value;
};

#define RatioToPrefix(RatioType, string) \
template<> \
const char * ratio_to_prefix<RatioType>::value = string

RatioToPrefix(std::ratio<1>, "");
RatioToPrefix(std::atto, "a");
RatioToPrefix(std::femto, "f");
RatioToPrefix(std::pico, "p");
RatioToPrefix(std::nano, "n");
RatioToPrefix(std::micro, "\u00b5");
RatioToPrefix(std::milli, "m");
RatioToPrefix(std::centi, "c");
RatioToPrefix(std::deci, "d");
RatioToPrefix(std::deca, "da");
RatioToPrefix(std::hecto, "h");
RatioToPrefix(std::kilo, "k");
RatioToPrefix(std::mega, "M");
RatioToPrefix(std::giga, "G");
RatioToPrefix(std::tera, "T");
RatioToPrefix(std::peta, "P");
RatioToPrefix(std::exa, "E");

template<class Component>
struct component_to_string;

template<template<class> class ComponentTemplate, class Ratio>
struct component_to_string<ComponentTemplate<Ratio>>
{
    static const std::string value;
};

template<template<class> class ComponentTemplate, class Ratio>
const std::string component_to_string<ComponentTemplate<Ratio>>::value = ratio_to_prefix<Ratio>::value + std::string{ ComponentTemplate<Ratio>::component_name };

template<class Key, int count>
struct group
{
    using type = Key;
    constexpr static int value = count;
};

template<class CurrentKey, class... Elements>
struct group_elements_impl;

template<class CurrentKey, class First, class... Elements>
struct group_elements_impl<CurrentKey, First, Elements...>
{
    constexpr static bool sameKey = std::is_same<CurrentKey, First>::value;
    constexpr static int count = 1 + (group_elements_impl<First, Elements...>::sameKey ? group_elements_impl<First, Elements...>::count : 0);
    using type = typename std::conditional < !sameKey,
        typename group_elements_impl<First, Elements...>::type::template add_front_t<group<First, count>>,
        typename group_elements_impl<First, Elements...>::type>::type;
};

template<class CurrentKey>
struct group_elements_impl<CurrentKey>
{
    constexpr static int count = 1;
    constexpr static bool sameKey = false;
    using type = typelist<>;
};

template<class... Elements>
struct group_elements
{
    using type = typename group_elements_impl<void, Elements...>::type;
};

template<class... Elements>
using group_elements_t = typename group_elements<Elements...>::type;

template<class List>
struct list_to_string;

template<class... Groups>
struct concatenate_groups_impl
{
    static const std::string value;
};

#ifdef __cpp_fold_expressions
template<class... Groups>
const std::string concatenate_groups_impl<Groups...>::value = ("" + ... + (component_to_string<typename Groups::type>::value  (Groups::value != 1 ? + "^" + std::to_string(Groups::value) : "")));
#else
template<>
const std::string concatenate_groups_impl<>::value = "";

template<class First, class... Rest>
struct concatenate_groups_impl<First, Rest...>
{
    static const std::string value;
};

template<class First, class... Rest>
const std::string concatenate_groups_impl<First, Rest...>::value = component_to_string<typename First::type>::value + (First::value != 1 ? "^" + std::to_string(First::value)  : "")
                                                                    + concatenate_groups_impl<Rest...>::value;
#endif
template<class List>
struct concatenate_groups;

template<class... Elements>
struct concatenate_groups<typelist<Elements...>>
{
    static const std::string value;
};

template<class... Elements>
const std::string concatenate_groups<typelist<Elements...>>::value = concatenate_groups_impl<Elements...>::value;

template<class... Elements>
struct list_to_string<typelist<Elements...>>
{
    using groups = group_elements_t<Elements...>;
    static const std::string value;
};

template<class... Elements>
const std::string list_to_string<typelist<Elements... >>::value = concatenate_groups<groups>::value;

template<class Unit>
struct unit_to_string
{
    static const std::string value;
};

template<class Unit>
const std::string unit_to_string<Unit>::value = list_to_string<typename Unit::numerator>::value
                                        + (Unit::denominator::count != 0 ? "/" + list_to_string<typename Unit::denominator>::value : "");