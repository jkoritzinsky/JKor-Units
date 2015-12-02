#pragma once
#include <type_traits>

// Implementations taken from en.cppreference.com
struct nonesuch {};
namespace detail {
    template<typename... Ts> struct make_void { typedef void type; };
    template<typename... Ts> using void_t = typename make_void<Ts...>::type;
    template <class Default, class AlwaysVoid,
        template<class...> class Op, class... Args>
    struct detector {
        using value_t = std::false_type;
        using type = Default;
    };

    template <class Default, template<class...> class Op, class... Args>
    struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
        using value_t = std::true_type;
        using type = Op<Args...>;
    };

} // namespace detail

template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;