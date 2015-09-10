#pragma once
#include "typelist.hpp"
#include "unit_compare.hpp"
#include <type_traits>
#include <ratio>
#include "helpers.hpp"

template<class Numerator = typelist<>, class Denominator = typelist<>>
struct unit
{
	using numerator = Numerator;
	using denominator = Denominator;
};

template<class AddList, class CancelList, class... Elements>
struct reduce;

template<class... AddElements, class... CancelElements>
struct reduce<typelist<AddElements...>, typelist<CancelElements...>>
{
	using afterAdd = typelist<AddElements...>;
	using afterCancel = typelist<CancelElements...>;
	using remainingRatio = std::ratio<1>;
};

template<class... AddElements, class... CancelElements, class First, class... Elements>
struct reduce<typelist<AddElements...>, typelist<CancelElements...>, First, Elements...>
{
private:
	using tryCancel = find_remove_unit<First, typelist<CancelElements...>, unit_equiv_ignore_prefix>;
public:
	using afterAdd = typename std::conditional<!tryCancel::found,
		typename reduce<insert_in_list_t<First, typelist<AddElements...>, unit_less>, typename tryCancel::unitList, Elements...>::afterAdd,
		typename reduce<typelist<AddElements...>, typename tryCancel::unitList, Elements...>::afterAdd>::type;
	using afterCancel = typename reduce<typelist<AddElements...>, typename tryCancel::unitList, Elements...>::afterCancel;
	using remainingRatio = std::ratio_multiply<typename tryCancel::remainedRatio,
		typename reduce<typelist<AddElements...>, typename tryCancel::unitList, Elements...>::remainingRatio>;
};

template<class Unit1, class Unit2>
struct unit_multiply;

template<class... Unit1Num, class... Unit1Denom, class... Unit2Num, class... Unit2Denom>
struct unit_multiply<unit<typelist<Unit1Num...>, typelist<Unit1Denom...>>, unit<typelist<Unit2Num...>, typelist<Unit2Denom...>>>
{
	using reduceDenom = reduce<typelist<Unit1Num...>, typelist<Unit1Denom...>, Unit2Num...>;
	using reduceNum = reduce<typename reduceDenom::afterCancel, typename reduceDenom::afterAdd, Unit2Denom...>;
	using type = unit<typename reduceNum::afterCancel, typename reduceNum::afterAdd>;
	using remainingRatio = std::ratio_divide<typename reduceNum::remainingRatio, typename reduceDenom::remainingRatio>;
};


template<class Unit1, class Unit2>
using unit_multiply_t = typename unit_multiply<Unit1, Unit2>::type;

template<class Unit1, class Ratio>
struct unit_scale;

template<template<class> class Component, class CurrentRatio, class... Elements, class Ratio>
struct unit_scale<unit<typelist<>, typelist<Component<CurrentRatio>, Elements...>>, Ratio>
{
	using type = unit<typelist<>, insert_in_list_t<Component<std::ratio_multiply<CurrentRatio, Ratio>>, typelist<Elements...>, unit_less>>;
};

template<template<class> class Component, class CurrentRatio, class... Elements, class Denominator, class Ratio>
struct unit_scale<unit<typelist<Component<CurrentRatio>, Elements...>, Denominator>, Ratio>
{
	using type = unit<insert_in_list_t<Component<std::ratio_multiply<CurrentRatio, Ratio>>, typelist<Elements...>, unit_less>, Denominator>;
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

template<class Unit1, class Unit2>
struct units_compatible : std::false_type {};

template<class... Unit1Num, class... Unit1Denom, class... Unit2Num, class... Unit2Denom>
struct units_compatible<unit<typelist<Unit1Num...>, typelist<Unit1Denom...>>, unit<typelist<Unit2Num...>, typelist<Unit2Denom...>>>
	: std::integral_constant<bool, all_true<unit_equiv_ignore_prefix<Unit1Num, Unit2Num>::value..., 
			unit_equiv_ignore_prefix<Unit1Denom, Unit2Denom>::value...>::value> {};
