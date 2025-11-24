#include "expression.hpp"
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <algorithm>

template<typename T>
void evaluateAndPrint(const std::string& exprStr, const std::map<std::string, T>& variables) {
    try {
        auto expr = Expression<T>::fromString(exprStr);
        auto result = expr.evaluate(variables);

        if (result) {
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                if (result->imag() == 0) {
                    std::cout << "Вычисление: " << result->real() << std::endl;
                } else {
                    std::cout << "Вычисление: (" << result->real() << ", " << result->imag() << ")" << std::endl;
                }
            } else {
                std::cout << "Вычисление: " << *result << std::endl;
            }
        } else {
            std::cerr << "Ошибка" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

template<typename T>
void differentiateAndPrint(const std::string& exprStr, const std::string& variable) {
    try {
        auto expr = Expression<T>::fromString(exprStr);
        auto derivative = expr.differentiate(variable);
        std::cout << derivative.toString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

template<typename T>
std::map<std::string, T> parseVariables(int argc, char* argv[], int startIndex) {
    std::map<std::string, T> variables;
    for (int i = startIndex; i < argc; ++i) {
        std::string arg = argv[i];
        size_t equalsPos = arg.find('=');
        if (equalsPos != std::string::npos) {
            std::string name = arg.substr(0, equalsPos);
            std::string valueStr = arg.substr(equalsPos + 1);
            T value;
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                size_t plusPos = valueStr.find('+');
                if (plusPos != std::string::npos) {
                    double real = std::stod(valueStr.substr(0, plusPos));
                    double imag = std::stod(valueStr.substr(plusPos + 1));
                    value = std::complex<double>(real, imag);
                } else {
                    value = std::complex<double>(std::stod(valueStr), 0);
                }
            } else {
                value = std::stod(valueStr);
            }
            variables[name] = value;
        }
    }
    return variables;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " --eval <expression> [variables...]" << std::endl;
        std::cerr << "       " << argv[0] << " --diff <expression> --by <variable>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string exprStr = argv[2];
    bool isComplex = exprStr.find('i') != std::string::npos;

    if (mode == "--eval") {
        if (isComplex) {
            auto variables = parseVariables<std::complex<double>>(argc, argv, 3);
            evaluateAndPrint<std::complex<double>>(exprStr, variables);
        } else {
            auto variables = parseVariables<double>(argc, argv, 3);
            evaluateAndPrint<double>(exprStr, variables);
        }
    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            std::cerr << argv[0] << " --diff <expression> --by <variable>" << std::endl;
            return 1;
        }
        std::string variable = argv[4];
        if (isComplex) {
            differentiateAndPrint<std::complex<double>>(exprStr, variable);
        } else {
            differentiateAndPrint<double>(exprStr, variable);
        }
    } else {
        std::cerr << "Неизвестно: " << mode << std::endl;
        return 1;
    }

    return 0;
}