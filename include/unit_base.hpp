#pragma once
#include "typelist.hpp"
#include "unit_compare.hpp"
#include <type_traits>
#include <ratio>
#include "helpers.hpp"

namespace jkor {
namespace units {
        template<class Numerator = details::typelist<>, class Denominator = details::typelist<>>
        struct unit
        {
            using numerator = Numerator;
            using denominator = Denominator;
        };

        template<class AddList, class CancelList, class... Elements>
        struct reduce;

        template<class... AddElements, class... CancelElements>
        struct reduce<details::typelist<AddElements...>, details::typelist<CancelElements...>>
        {
            using afterAdd = details::typelist<AddElements...>;
            using afterCancel = details::typelist<CancelElements...>;
            using remainingRatio = std::ratio<1>;
        };

        template<class... AddElements, class... CancelElements, class First, class... Elements>
        struct reduce<details::typelist<AddElements...>, details::typelist<CancelElements...>, First, Elements...>
        {
        private:
            using tryCancel = find_remove_unit<First, details::typelist<CancelElements...>, unit_equiv_ignore_prefix>;
        public:
            using afterAdd = typename std::conditional<!tryCancel::found,
                typename reduce<insert_in_list_t<First, details::typelist<AddElements...>, unit_less>, typename tryCancel::unitList, Elements...>::afterAdd,
                typename reduce<details::typelist<AddElements...>, typename tryCancel::unitList, Elements...>::afterAdd>::type;
            using afterCancel = typename reduce<details::typelist<AddElements...>, typename tryCancel::unitList, Elements...>::afterCancel;
            using remainingRatio = std::ratio_multiply<typename tryCancel::remainedRatio,
                typename reduce<details::typelist<AddElements...>, typename tryCancel::unitList, Elements...>::remainingRatio>;
        };

        template<class Unit1, class Unit2>
        struct unit_multiply;

        template<class... Unit1Num, class... Unit1Denom, class... Unit2Num, class... Unit2Denom>
        struct unit_multiply<unit<details::typelist<Unit1Num...>, details::typelist<Unit1Denom...>>, unit<details::typelist<Unit2Num...>, details::typelist<Unit2Denom...>>>
        {
            using reduceDenom = reduce<details::typelist<Unit1Num...>, details::typelist<Unit1Denom...>, Unit2Num...>;
            using reduceNum = reduce<typename reduceDenom::afterCancel, typename reduceDenom::afterAdd, Unit2Denom...>;
            using type = unit<typename reduceNum::afterCancel, typename reduceNum::afterAdd>;
            using remainingRatio = std::ratio_divide<typename reduceNum::remainingRatio, typename reduceDenom::remainingRatio>;
        };


        template<class Unit1, class Unit2>
        using unit_multiply_t = typename unit_multiply<Unit1, Unit2>::type;

        template<class Unit1, class Ratio>
        struct unit_scale;

        template<template<class> class Component, class CurrentRatio, class... Elements, class Ratio>
        struct unit_scale<unit<details::typelist<>, details::typelist<Component<CurrentRatio>, Elements...>>, Ratio>
        {
            using type = unit<details::typelist<>, insert_in_list_t<Component<std::ratio_multiply<CurrentRatio, Ratio>>, details::typelist<Elements...>, unit_less>>;
        };

        template<template<class> class Component, class CurrentRatio, class... Elements, class Denominator, class Ratio>
        struct unit_scale<unit<details::typelist<Component<CurrentRatio>, Elements...>, Denominator>, Ratio>
        {
            using type = unit<insert_in_list_t<Component<std::ratio_multiply<CurrentRatio, Ratio>>, details::typelist<Elements...>, unit_less>, Denominator>;
        };

        template<class Ratio>
        struct unit_scale<unit<>, Ratio>
        {
            using type = unit<>;
        };

        template<class Unit1, class Ratio>
        using unit_scale_t = typename unit_scale<Unit1, Ratio>::type;

        template<class Unit1, class Unit2>
        struct unit_divide;

        template<class Unit1Num, class Unit1Denom, class Unit2Num, class Unit2Denom>
        struct unit_divide<unit<Unit1Num, Unit1Denom>, unit<Unit2Num, Unit2Denom>>
            : unit_multiply<unit<Unit1Num, Unit1Denom>, unit<Unit2Denom, Unit2Num>> {};

        template<class Unit1, class Unit2>
        using unit_divide_t = typename unit_divide<Unit1, Unit2>::type;

        template<class Unit1, class Unit2, class = void>
        struct units_compatible : std::false_type {};

        template<class... Unit1Num, class... Unit1Denom, class... Unit2Num, class... Unit2Denom>
        struct units_compatible<unit<details::typelist<Unit1Num...>, details::typelist<Unit1Denom...>>, unit<details::typelist<Unit2Num...>, details::typelist<Unit2Denom...>>,
            typename std::enable_if<sizeof...(Unit1Num) == sizeof...(Unit2Num) && sizeof...(Unit1Denom) == sizeof...(Unit2Denom)>::type>
            : std::integral_constant<bool, details::all_true<unit_equiv_ignore_prefix<Unit1Num, Unit2Num>::value...,
            unit_equiv_ignore_prefix<Unit1Denom, Unit2Denom>::value...>::value> {};
    }
}