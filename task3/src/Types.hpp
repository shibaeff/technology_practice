#ifndef TYPES
#define TYPES

#include <string>
#include <memory>
#include <vector>
#include <map>

#include <stdio.h>


class TFunc {
public:
    virtual double operator()(double num) = 0;

    virtual std::string ToString() const = 0;

    virtual double Derivative(double num) = 0;

    virtual std::shared_ptr<TFunc> Copy() = 0;

protected:
    int precision;
public:
    int getPrecision() const;

    void setPrecision(int precision);
};

class Composition : public TFunc {
public:
    Composition(TFunc &other, TFunc &another_, std::string type_);

    double operator()(double num);

    std::string ToString() const;

    double Derivative(double num);

    std::shared_ptr<TFunc> Copy();

private:
    std::shared_ptr<TFunc> other, another;
    std::string type;
};

template<typename GenericFunction>
Composition operator+(TFunc &other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "+");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator+(Composition other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "+");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator-(TFunc &other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "-");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator-(Composition other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "-");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator*(TFunc &other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "*");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator*(Composition other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "*");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator/(TFunc &other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "/");
    } else {
        throw std::logic_error("Wrong types");
    }
}

template<typename GenericFunction>
Composition operator/(Composition other, GenericFunction &another) {
    if constexpr (std::is_base_of_v<TFunc, GenericFunction>) {
        return Composition(other, another, "/");
    } else {
        throw std::logic_error("Wrong types");
    }
}


class Polynomial : public TFunc {
public:
    Polynomial();

    explicit Polynomial(double factor_);

    explicit Polynomial(std::vector<double> factor_);

    double operator()(double num);

    std::string ToString() const;

    double Derivative(double num);

    std::shared_ptr<TFunc> Copy();

protected:
    std::map<unsigned int, double> Coefficients;
};

class Identity : public Polynomial {
public:
    Identity();

    explicit Identity(double factor_);

    explicit Identity(std::vector<double> factor_);
};

class Constant : public Polynomial {
public:
    Constant();

    explicit Constant(double factor_);

    explicit Constant(std::vector<double> factor_);
};

class Power : public Polynomial {
public:
    Power();

    explicit Power(double factor_);

    explicit Power(std::vector<double> factor_);
};

class Exponential : public TFunc {
public:
    Exponential() : factor(0) {};

    explicit Exponential(double _factor) : factor(_factor) {};

    explicit Exponential(std::vector<double> factor_);

    double operator()(double num);

    std::string ToString() const;

    double Derivative(double num);

    std::shared_ptr<TFunc> Copy();

protected:
    double factor;
};

#endif