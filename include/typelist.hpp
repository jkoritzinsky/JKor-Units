#pragma once

template<class... Elements>
struct typelist
{
    template<class E>
    using add_front_t = typelist<E, Elements...>;
    template<class E>
    using add_back_t = typelist<Elements..., E>;
    private:
    template<class List>
    struct concat;
    template<template<class...> class List, class... Elements2>
    struct concat<List<Elements2...>>
    {
        using type = typelist<Elements..., Elements2...>;
    };
    public:
    template<class List>
    using concat_t = typename concat<List>::type;
};