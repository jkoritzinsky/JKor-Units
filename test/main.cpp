#include "../include/value.hpp"
#include "../include/si_units.hpp"
#include "../include/unit_to_string.hpp"

using basic_multiply = unit_multiply<meter_unit, second_unit>::type;
using basic_divide = unit_divide<unit<>, second_unit>::type;
using basic_compatible = units_compatible<second_unit, second_unit>;
using incompatible = units_compatible<second_unit, unit_multiply_t<meter_unit, kilogram_unit>>;
using complex_compatible = units_compatible<gram_unit, kilogram_unit>;
using basic_cancel = unit_divide<second_unit, second_unit>::type;
using complex_cancel = unit_divide<unit_multiply<meter_unit, second_unit>::type, second_unit>::type;
using remainingTest = unit_divide<gram_unit, kilogram_unit>;
using group_test = group_elements_t<second_component<std::ratio<1>>, second_component<std::ratio<1>>, gram_component<std::ratio<1>>, meter_component<std::ratio<1>>>;
#include <iostream>
#include <typeinfo>

int main()
{
    std::cout << std::boolalpha;
    std::cout << typeid(basic_multiply).name() << "\n\n";
    std::cout << typeid(basic_divide).name() << "\n\n";
    std::cout << typeid(basic_cancel).name() << "\n\n";
    std::cout << typeid(complex_cancel).name() << "\n\n";
    std::cout << typeid(remainingTest::remainingRatio).name() << "\n\n";
    std::cout << typeid(group_test).name() << "\n\n";
    std::cout << unit_to_string<meter_unit>::value << "\n\n";
    std::cout << basic_compatible::value;
    std::cout << complex_compatible::value;
    std::cout << incompatible::value;
    char wait;
    std::cin >> wait;
}