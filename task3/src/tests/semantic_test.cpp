#include "gtest/gtest.h"
#include <iostream>
#include <cmath>

#include "../Factory.hpp"

class FuncTestSuite : public testing::Test {
};

TEST(FuncTestSuite, FactoryTypesTest) {
    FunctionFactory factory;

    // Изменяем порядок и параметры функций
    auto f1 = factory.Create("polynomial", {3, 4, 5});
    Polynomial f2({2, 3});
    ASSERT_EQ(typeid(*f1), typeid(f2));

    auto f3 = factory.Create("exp", {});
    Exponential f5(1);
    ASSERT_EQ(typeid(*f3), typeid(f5));

    auto f4 = factory.Create("power", {4});
    Power f6(3);
    ASSERT_EQ(typeid(*f4), typeid(f6));

    auto f7 = factory.Create("const", {2});
    Constant f8(4);
    ASSERT_EQ(typeid(*f7), typeid(f8));

    auto f9 = factory.Create("ident", {});
    Identity f10;
    ASSERT_EQ(typeid(*f9), typeid(f10));
}

TEST(FuncTestSuite, String_rep_test) {
    FunctionFactory factory;

    // New examples with different types and parameters

    auto f1 = factory.Create("const", {3});
    auto f2 = factory.Create("polynomial", {3}); // A constant polynomial with value 3
    ASSERT_EQ(f1->ToString(), f2->ToString()); // Comparing string representation of constant 3 and polynomial 3

    auto f3 = factory.Create("polynomial", {0, 0, 1}); // x^2 + 1
    auto f4 = factory.Create("power", {2}); // x^2
    ASSERT_EQ(f3->ToString(), f4->ToString()); // Comparing string representation of x^2 + 1 and x^2
}
