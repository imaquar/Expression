#include <string>
#include <map>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <sstream>

template <typename T>
class Expression {
private:
    T value;
    std::string variable;
    bool is_variable;

public:
    Expression(T value);
    Expression(const std::string &variable);
    Expression(const Expression<T>& other);
    Expression(Expression<T>&& other) noexcept;
    ~Expression();

    Expression<T>& operator=(const Expression<T>& other);
    Expression<T>& operator=(Expression<T>&& other) noexcept;

    Expression<T> operator+(const Expression<T>& other) const;
    Expression<T> operator-(const Expression<T>& other) const;
    Expression<T> operator*(const Expression<T>& other) const;
    Expression<T> operator/(const Expression<T>& other) const;
    Expression<T> operator^(const Expression<T>& other) const;

    Expression<T> sin() const;
    Expression<T> cos() const;
    Expression<T> ln() const;
    Expression<T> exp() const;

    std::string to_string() const;

    void substitute(const std::string &variable, T value);

    T evaluate(const std::map<std::string, T>& variables) const;
};

template class Expression<double>;
template class Expression<std::complex<double>>;
