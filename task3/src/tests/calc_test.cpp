#include "gtest/gtest.h"
#include <iostream>
#include <cmath>

#include "../Factory.hpp"
#include "../algorithm.hpp"

TEST(FuncTestSuite, Arithmetic) {
    TFunctionFactory factory;

    // Создаем степенную функцию и константу
    auto f1 = factory.Create("power", 2); // x^2
    auto f2 = factory.Create("const", 3); // 3

    // Проверяем сумму и разность f1 и f2
    auto f3 = *f1 + *f2;
    ASSERT_EQ(f3(2), 7); // 2^2 + 3 = 7
    ASSERT_EQ(f3(3), 12); // 3^2 + 3 = 12

    ASSERT_EQ((*f1 - *f2)(2), 1); // 2^2 - 3 = 1
    ASSERT_EQ((*f1 - *f2)(3), 6); // 3^2 - 3 = 6

    // Проверяем произведение f1 и f2
    ASSERT_EQ((*f1 * *f2)(2), 12); // 2^2 * 3 = 12
    ASSERT_EQ((*f1 * *f2)(3), 27); // 3^2 * 3 = 27

    // Создаем экспоненциальную функцию и тестируем деление
    auto f4 = factory.Create("exp", 1); // e^x
    ASSERT_EQ((f3 / *f4)(1), 4 / std::exp(1)); // (x^2 + 3) / e^x при x=1
    ASSERT_EQ((f3 / *f4)(2), 7 / std::exp(2)); // (x^2 + 3) / e^x при x=2

    // Полиномиальная функция для дальнейших проверок
    auto f5 = factory.Create("polynomial", {1, -1, 1}); // x^2 - x + 1
    ASSERT_EQ((*f5)(2), 3); // 2^2 - 2 + 1 = 3
    ASSERT_EQ((*f5)(3), 7); // 3^2 - 3 + 1 = 7
}

TEST(FuncTestSuite, Derivation) {
    TFunctionFactory factory;

    // Первый случай: используем три разных функции
    auto f1 = factory.Create("const", 5);  // Константа 5
    auto f2 = factory.Create("exp", 1);    // e^x
    auto f3 = factory.Create("power", 1);  // x
    auto f4 = *f1 + *f2 + *f3;

    // Проверяем производные для комбинации функций
    ASSERT_EQ(f4.Derivative(1), 1 + std::exp(1)); // Производная 5 + e^x + x при x = 1
    ASSERT_EQ(f4.Derivative(2), 1 + std::exp(2)); // Производная 5 + e^x + x при x = 2

    // Второй случай: используем две функции
    auto f5 = factory.Create("polynomial", {2, 0, -1}); // -x^2 + 2
    auto f6 = factory.Create("power", 3);               // x^3
    auto q = *f5 + *f6;                            // Сумма -x^2 + 2 и x^3

    // Проверяем производные для комбинации других функций
    ASSERT_EQ(q.Derivative(1), 1); // Производная -x^2 + 2 + x^3 при x = 1 -> -2x + 3x**2
    ASSERT_EQ(q.Derivative(3), -6 + 3 * 9); // Производная -x^2 + 2 + x^3 при x = 3
}


TEST(FuncTestSuite, LogicError) {
    TFunctionFactory factory;
    auto poly = factory.Create("polynomial", {0, 1, 2, 3});
    ASSERT_THROW(*poly + "something", std::logic_error);
    ASSERT_THROW(*poly + poly, std::logic_error);
}


TEST(FuncTestSuite, RootAlgo) {
    TFunctionFactory factory;

    auto g = factory.Create("polynomial", {4, 1});
    ASSERT_EQ(std::round(findRootUsingGradientDescent(*g, -10)), -4);
    auto p = factory.Create("polynomial", {1, 0, -1});
    ASSERT_EQ(std::round(findRootUsingGradientDescent(*p, -100)), -1);
    ASSERT_EQ(std::round(findRootUsingGradientDescent(*p, 20)), 1);
    auto h = factory.Create("polynomial", {8, 0, 0, -1});
    ASSERT_EQ(std::round(findRootUsingGradientDescent(*h, 1000)), 2);

}