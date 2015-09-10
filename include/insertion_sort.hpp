#pragma once
#include <type_traits>

template<class Element, class Container, template<class, class> class Comparator>
struct insert;

template<class Element, class Container, template<class, class> class Comparator>
using insert_t = typename insert<Element, Container, Comparator>::type;

template<class Element, template<class...> class List, template<class, class> class Comparator>
struct insert<Element, List<>, Comparator>
{
    using type = List<>;
};

template<class Element, template<class...> class List, class First, template<class, class> class Comparator, class... Elements>
struct insert<Element, List<First, Elements...>, Comparator>
{
    using type = typename std::conditional<Comparator<Element, First>::value,
                                        List<Element, First, Elements...>,
                                        insert_t<Element, List<Elements...>>::template add_front_t<First> >::type;
};

template<class Container, template<class, class> class Comparator>
struct insertion_sort;

template<template<class...> class List, template<class, class> class Comparator>
struct insertion_sort<List<>, Comparator>
{
    using type = List<>;
};

template<template<class...> class List, template<class, class> class Comparator, class First, class... Elements>
struct insertion_sort<List<First, Elements...>, Comparator>
{
    using type = insert_t<First, sort_t<List<Elements...>, Comparator>, Comparator>;
};