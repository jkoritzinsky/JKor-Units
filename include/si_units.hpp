#pragma once
#include "unit_macros.hpp"
#include <ratio>

namespace jkor {
    namespace units {
        DEFINE_BASE_UNIT(meter, m)
        DEFINE_BASE_UNIT(ampere, A)
        DEFINE_BASE_UNIT(gram, g)
        DEFINE_SCALED_UNIT(kilogram, kg, gram, std::kilo)
        DEFINE_BASE_UNIT(kelvin, K)
        DEFINE_BASE_UNIT(mole, mol)
        DEFINE_BASE_UNIT(candela, cd)
        DEFINE_DERIVED_UNIT(coulomb, C, decltype(amperes{} *seconds{})::unit)
        DEFINE_DERIVED_UNIT(hertz, Hz, decltype(value<>{} / seconds{})::unit)
        DEFINE_DERIVED_UNIT(radian, rad, value<>::unit)
        DEFINE_DERIVED_UNIT(steradian, sr, value<>::unit)
        DEFINE_DERIVED_UNIT(newton, N, decltype(kilograms{} *meters{} / (seconds{} *seconds{}))::unit)
        DEFINE_DERIVED_UNIT(pascal, Pa, decltype(newtons{} / (meters{} *meters{}))::unit)
        DEFINE_DERIVED_UNIT(joule, J, decltype(newtons{} *meters{})::unit)
        DEFINE_DERIVED_UNIT(watt, W, decltype(joules{} / seconds{})::unit)
        DEFINE_DERIVED_UNIT(volt, V, decltype(joules{} / coulombs{})::unit)
        DEFINE_DERIVED_UNIT(farad, F, decltype(coulombs{} / volts{})::unit)
        DEFINE_DERIVED_UNIT(ohm, O, decltype(volts{} / amperes{})::unit)
        DEFINE_DERIVED_UNIT(siemens, S, decltype(value<>{} / ohms{})::unit)
        DEFINE_DERIVED_UNIT(weber, Wb, decltype(joules{} / amperes{})::unit)
        DEFINE_DERIVED_UNIT(tesla, T, decltype(webers{} / (meters{} *meters{}))::unit)
        DEFINE_DERIVED_UNIT(henry, H, decltype(ohms{} *seconds{})::unit)
        DEFINE_DERIVED_UNIT(lumen, lm, decltype(candelas{} *steradians{})::unit)
        DEFINE_DERIVED_UNIT(lux, lx, decltype(lumens{} / (meters{} *meters{}))::unit)
        DEFINE_DERIVED_UNIT(becquerel, Bq, hertzs)
        DEFINE_DERIVED_UNIT(katal, kat, decltype(moles{} / seconds{})::unit)
    }
}