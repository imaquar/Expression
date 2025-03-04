#include "expression.hpp"
#include <iostream>

template <typename T>
Expression<T>::Expression(T value) : value(value), is_variable(false) {}

template <typename T>
Expression<T>::Expression(const std::string &variable) : variable(variable), is_variable(true) {}

template <typename T>
Expression<T>::Expression(const Expression<T>& other) : value(other.value), variable(other.variable), is_variable(other.is_variable) {}

template <typename T>
Expression<T>::Expression(Expression<T>&& other) noexcept : value(std::move(other.value)), variable(std::move(other.variable)), is_variable(other.is_variable) {}

template <typename T>
Expression<T>::~Expression() {}

template <typename T>
Expression<T>& Expression<T>::operator=(const Expression<T>& other) {
    value = other.value;
    variable = other.variable;
    is_variable = other.is_variable;
    return *this;
}

template <typename T>
Expression<T>& Expression<T>::operator=(Expression<T>&& other) noexcept {
    value = std::move(other.value);
    variable = std::move(other.variable);
    is_variable = other.is_variable;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression<T>& other) const {
    if (is_variable || other.is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(value + other.value);
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression<T>& other) const {
    if (is_variable || other.is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(value - other.value);
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression<T>& other) const {
    if (is_variable || other.is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(value * other.value);
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression<T>& other) const {
    if (is_variable || other.is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(value / other.value);
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression<T>& other) const {
    if (is_variable || other.is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(std::pow(value, other.value));
}

template <typename T>
Expression<T> Expression<T>::sin() const {
    if (is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(std::sin(value));
}

template <typename T>
Expression<T> Expression<T>::cos() const {
    if (is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(std::cos(value));
}

template <typename T>
Expression<T> Expression<T>::ln() const {
    if (is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(std::log(value));
}

template <typename T>
Expression<T> Expression<T>::exp() const {
    if (is_variable) {
        throw std::runtime_error("ошибка");
    }
    return Expression<T>(std::exp(value));
}

template <typename T>
std::string Expression<T>::to_string() const {
    if (is_variable) {
        return variable;
    } else {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
}

template <typename T>
void Expression<T>::substitute(const std::string &variable, T value) {
    if (is_variable && this->variable == variable) {
        this->value = value;
        is_variable = false;
    }
}

template <typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    if (is_variable) {
        auto it = variables.find(variable);
        if (it != variables.end()) {
            return it->second;
        } else {
            throw std::runtime_error("ошибка");
        }
    } else {
        return value;
    }
}

int main(void) {

    Expression<double> ch1(5.0);
    Expression<double> ch2("x");
    Expression<double> ch3(2.0);

    auto sum = ch1 + ch3;
    auto diff = ch1 - ch3;
    auto prod = ch1 * ch3;
    auto div = ch1 / ch3;
    auto pow = ch1 ^ ch3;
    
    std::cout << "sum: " << sum.to_string() << std::endl;
    std::cout << "diff " << diff.to_string() << std::endl;
    std::cout << "prod: " << prod.to_string() << std::endl;
    std::cout << "div: " << div.to_string() << std::endl;
    std::cout << "pow: " << pow.to_string() << std::endl;
 
    auto sin = ch1.sin();
    auto cos = ch1.cos();
    auto ln = ch1.ln();
    auto exp = ch1.exp();
    
    std::cout << "sin: " << sin.to_string() << std::endl;
    std::cout << "cos: " << cos.to_string() << std::endl;
    std::cout << "ln: " << ln.to_string() << std::endl;
    std::cout << "exp: " << exp.to_string() << std::endl;

    std::map<std::string, double> variables = {{"x", 4.0}};
    auto result = ch2.evaluate(variables);
    std::cout << "evaluation: " << result << std::endl;

    std::complex<double> complex_ch1(2.0, 3.0);
    std::complex<double> complex_ch2(1.0, 4.0);
    
    Expression<std::complex<double>> complex_expr1(complex_ch1);
    Expression<std::complex<double>> complex_expr2(complex_ch2);
    
    auto complex_sum = complex_expr1 + complex_expr2;
    auto complex_diff = complex_expr1 - complex_expr2;
    auto complex_prod = complex_expr1 * complex_expr2;
    auto complex_div = complex_expr1 / complex_expr2;
    
    std::cout << "complex sum: " << complex_sum.to_string() << std::endl;
    std::cout << "complex diff: " << complex_diff.to_string() << std::endl;
    std::cout << "complex prod: " << complex_prod.to_string() << std::endl;
    std::cout << "complex div: " << complex_div.to_string() << std::endl;
    
    return 0;
}
