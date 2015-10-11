#include "../include/si_units.hpp"

static_assert(units_compatible<second_unit, second_unit>::value, "A unit is compatible with itself.");
static_assert(units_compatible<kilogram_unit, gram_unit>::value, "A unit is compatible with a scaled version of itself");
static_assert(units_compatible<unit_divide_t<unit_multiply_t<meter_unit, kilogram_unit>, kilogram_unit>, meter_unit>::value,
				"A unit is compatible with itself even if generated through dimensional math.");
static_assert(units_compatible<unit_multiply_t<meter_unit, candela_unit>, unit_multiply_t<meter_unit, candela_unit>>::value,
	"Compound units that are the created same way are compatible with themselves.");
static_assert(units_compatible<unit_multiply_t<meter_unit, candela_unit>, unit_multiply_t<candela_unit, meter_unit>>::value,
	"Compound units that are the same but created differently are compatible with themselves.");