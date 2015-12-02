
#include <type_traits>
#include "catch.h"
#include "detection_idiom_backport.hpp"
#include "../include/value.hpp"
#include "../include/si_units.hpp"

using namespace jkor::units;

template<class Value1, class Value2>
using addable = decltype(std::declval<Value1>() + std::declval<Value2>());

static_assert(is_detected<addable, seconds, seconds>{}, "Can add a unit to itself.");
//static_assert(!is_detected<addable, seconds, meters>{}, "Cannot add two incompatible units");
//Requires Expression SFINAE to run the commented out test
static_assert(is_detected<addable, grams, kilograms>{}, "Can add a unit with a scaled version of the same unit");