#pragma once
#include <type_traits>

template<class Pivot, template<class, class> class Comparator, class Container>
struct partition;

template<class Pivot, template<class, class> class Comparator, class Container>
using partition_t = partition<Pivot, Comparator, Container>;

template<class Pivot, template<class, class> class Comparator, template<class...> class List, class First, class... Elements>
struct partition<Pivot, Comparator, List<>>
{
    using less = List<>;
    using equal = List<Pivot>;
    using greater = List<>;
};

template<class Pivot, template<class, class> class Comparator, template<class...> class List, class First, class... Elements>
struct partition<Pivot, Comparator, List<First, Elements...>>
{
    using part = partition_t<Pivot, Comparator, List<Elements...>>;
    using less = typename std::contitional<Comparator<First, Pivot>::value, 
                                            part::less::template add_front_t<First>,
                                            part::less>::type;
    using greater = typename std::contitional<Comparator<Pivot, First>::value, 
                                            part::greater::template add_front_t<First>,
                                            part::greater>::type;
    using equal = typename std::contitional<!Comparator<Pivot, First>::value && !Comparator<First, Pivot>::value, 
                                            part::equal::template add_front_t<First>,
                                            part::equal>::type;
};

template<class Container, template<class, class> class Comparator>
struct quicksort;

template<template<class...> class List, template<class, class> Comparator>
struct quicksort<List<>, Comparator>
{
    using type = List<>;
};

template<template<class...> class List, template<class, class> Comparator, class Pivot, class... Elements>
struct quicksort<List<Pivot, Elements...>, Comparator>
{
    using parts = partition_t<Pivot, Comparator, Elements...>;
    using type = sort_t<parts::less>::template concat_t<parts::equal>::template concat_t<sort_t<parts::greater>>;
};