#pragma once

#include "unit_macros.hpp"
#include "si_units.hpp"
#include <ratio>

namespace jkor {
    namespace units {
        DEFINE_SCALED_UNIT(inch, in, meter, std::ratio<254, 1000>);
        DEFINE_SCALED_UNIT(foot, ft, inch, std::ratio<12>);
        using feet = foots; // Need to correct the pluralization...
        DEFINE_SCALED_UNIT(yard, yd, foot, std::ratio<3>);
        DEFINE_SCALED_UNIT(mile, mi, foot, std::ratio<5280>);
        DEFINE_DERIVED_UNIT(square_mile, sq_mi, decltype(miles{} *miles{})::unit);
        DEFINE_SCALED_UNIT(acre, ac, square_mile, std::ratio<1, 640>);
        DEFINE_SCALED_UNIT(ounce, oz, gram, std::ratio_divide<std::ratio<28349523125>, std::giga>);
        DEFINE_SCALED_UNIT(pound, lb, ounce, std::ratio<12>);

    }
}