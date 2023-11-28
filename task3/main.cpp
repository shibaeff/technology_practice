#include <iostream>
#include <vector>
#include <memory>
#include "src/Factory.hpp"

int main() {
    FunctionFactory factory;
    std::vector<std::shared_ptr<TFunc>> cont;
    auto f = factory.Create("power", {1, 2, 10}); // Power: x^2
    cont.push_back(f);
    auto g = factory.Create("polynomial", {7, 0, 3, 15}); // Polynomial 7 + 3*x^2 + 15*x^3
    cont.push_back(g);
    for (const auto ptr: cont) {
        std::cout << ptr->ToString() << " for x = 10 is " << (*ptr)(10) << std::endl;
    }
    auto p = *f + *g;
    std::cout << p.Derivative(1) << "\n"; // 51
    // auto h = *f / "1"; // std::logic_error
    std::cout << f->Derivative(3) << "\n"; // 6

    auto ident = factory.Create("ident", {1, 2});
    std::cout << ident->ToString() << "\n";
    return 0;
}