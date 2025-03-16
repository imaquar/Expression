#include "expression.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    if (strcmp(argv[1], "--eval") == 0) {
        if (argc < 3) {
            return 1;
        }

        std::string expression = argv[2];
        std::map<std::string, double> variables;

        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eqPos = arg.find('=');
            if (eqPos == std::string::npos) {
                std::cerr << "Ошибка: неправильный формат переменной " << arg << "\n";
                return 1;
            }

            std::string varName = arg.substr(0, eqPos);
            double varValue = std::stod(arg.substr(eqPos + 1));
            variables[varName] = varValue;
        }

        auto expr = Expression<double>::fromString(expression);
        auto result = expr.evaluate(variables);

        if (result) {
            std::cout << *result << "\n";
        } else {
            std::cerr << "Ошибка: не удалось вычислить выражение\n";
            return 1;
        }
    } else if (strcmp(argv[1], "--diff") == 0) {
        if (argc < 5 || strcmp(argv[3], "--by") != 0) {
            return 1;
        }

        std::string expression = argv[2];
        std::string variable = argv[4];

        auto expr = Expression<double>::fromString(expression);
        auto diffExpr = expr.differentiate(variable);

        std::cout << diffExpr.toString() << "\n";
    } else {
        return 1;
    }

    return 0;
}