#pragma once
#include "insertion_sort.hpp"
#include "quicksort.hpp"

template<class Container, template<class, class> class Comparator>
struct sort;

template<template<class...> class List, template<class, class> class Comparator, class... Elements>
struct sort<List<Elements...>, Comparator>
{
    using type = sort_impl_t<sizeof...(Elements) > 20, List<Elements...>, Comparator>;
};

template<class Container, template<class, class> class Comparator>
using sort_t = typename sort<Container, Comparator>::type;

template<bool SizeSwitch, class Container, template<class, class> class Comparator>
struct sort_impl;

// Use insertion sort for <= 20 elements
template<class Container, template<class, class> class Comparator>
struct sort_impl<false, Container, Comparator> : public insertion_sort<Container, Comparator>
{};

// Use quicksort for > 20 elements
template<class Container, template<class, class> class Comparator>
struct sort_impl<true, Container, Comparator> : public quicksort<Container, Comparator>
{};

template<bool SizeSwitch, class Container, template<class, class> class Comparator>
using sort_impl_t = typename sort_impl<SizeSwitch, Container, Comparator>::type;