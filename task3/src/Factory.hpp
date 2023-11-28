#ifndef FACTORY
#define FACTORY

#include <map>

#include "Types.hpp"


class TFunctionFactory {
public:
    TFunctionFactory(int precision_ = 3): precision(precision_) {
        RegisterAdapters();
    }

    // general scheme: https://mk.cs.msu.ru/images/5/57/Lecture_pbscpp_8.pdf
    class AbstractProducer {
    public:
        virtual ~AbstractProducer() {};

        virtual auto Produce() -> std::shared_ptr<TFunc> const = 0;

        virtual auto Produce(double num) -> std::shared_ptr<TFunc> const = 0;

        virtual auto Produce(std::vector<double> parameters) -> std::shared_ptr<TFunc> const = 0;
    };

    template<class TFunction>
    class FunctionProducer : public AbstractProducer {
    public:
        auto Produce() -> std::shared_ptr<TFunc> const override {
            return std::make_shared<TFunction>(TFunction());
        }

        auto Produce(double num) -> std::shared_ptr<TFunc> const override {
            return std::make_shared<TFunction>(TFunction(num));
        }

        auto Produce(std::vector<double> parameters) -> std::shared_ptr<TFunc> const override {
            return std::make_shared<TFunction>(TFunction(parameters));
        }
    };


    std::shared_ptr<TFunc> Create(const std::string &type) {
        auto t = RegisteredCreators[type]->Produce();
        t->setPrecision(precision);
        return t;
    }

    std::shared_ptr<TFunc> Create(const std::string &type, double num) {
        auto t = RegisteredCreators[type]->Produce(num);
        t->setPrecision(precision);
        return t;
    }

    std::shared_ptr<TFunc> Create(const std::string &type, std::vector<double> parameters) {
        auto t = RegisteredCreators[type]->Produce(parameters);
        t->setPrecision(precision);
        return t;
    }

private:
    using TCreatorPtr = std::shared_ptr<AbstractProducer>;
    using TRegisteredCreators = std::map<std::string, TCreatorPtr>;

    TRegisteredCreators RegisteredCreators;

    int precision;

    void RegisterAdapters() {
        RegisteredCreators["ident"] = std::make_shared<FunctionProducer<Identity>>();
        RegisteredCreators["const"] = std::make_shared<FunctionProducer<Constant>>();
        RegisteredCreators["power"] = std::make_shared<FunctionProducer<Power>>();
        RegisteredCreators["exp"] = std::make_shared<FunctionProducer<Exponential>>();
        RegisteredCreators["polynomial"] = std::make_shared<FunctionProducer<Polynomial>>();
    }

};

#endif