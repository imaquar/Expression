#include "expression.hpp"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include <iostream>

template<typename T>
Expression<T>::Expression(T value) : root(std::make_unique<ConstantNode>(value)) {}

template<typename T>
Expression<T>::Expression(const std::string& variable) : root(std::make_unique<VariableNode>(variable)) {}

template<typename T>
Expression<T>::Expression(const Expression& other) : root(other.root->clone()) {}

template<typename T>
Expression<T>::Expression(Expression&& other) noexcept : root(std::move(other.root)) {}

template<typename T>
Expression<T>::~Expression() = default;

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        root = other.root->clone();
    }
    return *this;
}

template<typename T>
Expression<T>& Expression<T>::operator=(Expression&& other) noexcept {
    if (this != &other) {
        root = std::move(other.root);
    }
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    return Expression(std::make_unique<BinaryOperationNode>('+', root->clone(), other.root->clone()));
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression& other) const {
    return Expression(std::make_unique<BinaryOperationNode>('-', root->clone(), other.root->clone()));
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression& other) const {
    return Expression(std::make_unique<BinaryOperationNode>('*', root->clone(), other.root->clone()));
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression& other) const {
    return Expression(std::make_unique<BinaryOperationNode>('/', root->clone(), other.root->clone()));
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression& other) const {
    return Expression(std::make_unique<BinaryOperationNode>('^', root->clone(), other.root->clone()));
}

template<typename T>
Expression<T> Expression<T>::sin() const {
    return Expression(std::make_unique<UnaryOperationNode>("sin", root->clone()));
}

template<typename T>
Expression<T> Expression<T>::cos() const {
    return Expression(std::make_unique<UnaryOperationNode>("cos", root->clone()));
}

template<typename T>
Expression<T> Expression<T>::ln() const {
    return Expression(std::make_unique<UnaryOperationNode>("ln", root->clone()));
}

template<typename T>
Expression<T> Expression<T>::exp() const {
    return Expression(std::make_unique<UnaryOperationNode>("exp", root->clone()));
}

template<typename T>
Expression<T> Expression<T>::substitute(const std::string& variable, T value) const {
    auto newRoot = root->substitute(variable, value);
    return Expression(std::move(newRoot));
}

template<typename T>
std::optional<T> Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    return root->evaluate(variables);
}

template<typename T>
std::string Expression<T>::toString() const {
    return root->toString();
}

template<typename T>
std::string Expression<T>::toStringWithSubstitution(const std::map<std::string, T>& variables) const {
    return root->toStringWithSubstitution(variables);
}

template<typename T>
Expression<T> Expression<T>::fromString(const std::string& expr) {
    size_t pos = 0;
    auto node = parseExpression(expr, pos);
    return Expression(std::move(node));
}

template<typename T>
Expression<T> Expression<T>::differentiate(const std::string& variable) const {
    auto diffRoot = root->differentiate(variable);
    return Expression(std::move(diffRoot));
}

template<typename T>
std::optional<T> Expression<T>::BinaryOperationNode::evaluate(const std::map<std::string, T>& variables) const {
    auto leftVal = left->evaluate(variables);
    auto rightVal = right->evaluate(variables);

    if (!leftVal || !rightVal) {
        return std::nullopt;
    }

    switch (op) {
        case '+': return *leftVal + *rightVal;
        case '-': return *leftVal - *rightVal;
        case '*': return *leftVal * *rightVal;
        case '/': return *leftVal / *rightVal;
        case '^': return std::pow(*leftVal, *rightVal);
        default: throw std::invalid_argument("неизвестный оператор");
    }
}

template<typename T>
std::string Expression<T>::BinaryOperationNode::toString() const {
    std::string leftStr = left->toString();
    std::string rightStr = right->toString();

    if (left->precedence() < this->precedence()) {
        leftStr = "(" + leftStr + ")";
    }
    if (right->precedence() < this->precedence()) {
        rightStr = "(" + rightStr + ")";
    }

    return leftStr + " " + op + " " + rightStr;
}

template<typename T>
std::string Expression<T>::BinaryOperationNode::toStringWithSubstitution(const std::map<std::string, T>& variables) const {
    std::string leftStr = left->toStringWithSubstitution(variables);
    std::string rightStr = right->toStringWithSubstitution(variables);

    if (left->precedence() < this->precedence()) {
        leftStr = "(" + leftStr + ")";
    }
    if (right->precedence() < this->precedence()) {
        rightStr = "(" + rightStr + ")";
    }

    return leftStr + " " + op + " " + rightStr;
}

template<typename T>
std::optional<T> Expression<T>::UnaryOperationNode::evaluate(const std::map<std::string, T>& variables) const {
    auto val = operand->evaluate(variables);
    if (!val) {
        return std::nullopt;
    }

    if (func == "sin") return std::sin(*val);
    if (func == "cos") return std::cos(*val);
    if (func == "ln") return std::log(*val);
    if (func == "exp") return std::exp(*val);
    throw std::invalid_argument("неизвестная функция");
}

template<typename T>
std::string Expression<T>::UnaryOperationNode::toString() const {
    return func + "(" + operand->toString() + ")";
}

template<typename T>
std::string Expression<T>::UnaryOperationNode::toStringWithSubstitution(const std::map<std::string, T>& variables) const {
    return func + "(" + operand->toStringWithSubstitution(variables) + ")";
}

template<typename T>
std::optional<T> Expression<T>::VariableNode::evaluate(const std::map<std::string, T>& variables) const {
    auto it = variables.find(name);
    if (it != variables.end()) {
        return it->second;
    }
    return std::nullopt;
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::VariableNode::substitute(const std::string& variable, T value) {
    if (name == variable) {
        return std::make_unique<ConstantNode>(value);
    }
    return clone();
}

template<typename T>
Expression<T> Expression<T>::simplify() const {
    auto simplifiedRoot = simplifyNode(root->clone());
    return Expression(std::move(simplifiedRoot));
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::simplifyNode(std::unique_ptr<Node> node) {
    if (auto binaryNode = dynamic_cast<BinaryOperationNode*>(node.get())) {
        auto left = simplifyNode(std::move(binaryNode->left));
        auto right = simplifyNode(std::move(binaryNode->right));

        if (binaryNode->op == '*' && (left->toString() == "0.000000" || right->toString() == "0.000000")) {
            return std::make_unique<ConstantNode>(0);
        }

        if (binaryNode->op == '*' && left->toString() == "1.000000") {
            return right;
        }
        if (binaryNode->op == '*' && right->toString() == "1.000000") {
            return left;
        }

        if (binaryNode->op == '*' && dynamic_cast<ConstantNode*>(left.get()) && dynamic_cast<ConstantNode*>(right.get())) {
            T leftValue = dynamic_cast<ConstantNode*>(left.get())->value;
            T rightValue = dynamic_cast<ConstantNode*>(right.get())->value;
            return std::make_unique<ConstantNode>(leftValue * rightValue);
        }

        if (binaryNode->op == '+' && left->toString() == "0.000000") {
            return right;
        }
        if (binaryNode->op == '+' && right->toString() == "0.000000") {
            return left;
        }

        if (binaryNode->op == '*' && dynamic_cast<BinaryOperationNode*>(left.get()) && left->toString().find('*') != std::string::npos) {
            left = simplifyNode(std::move(left));
        }
        if (binaryNode->op == '*' && dynamic_cast<BinaryOperationNode*>(right.get()) && right->toString().find('*') != std::string::npos) {
            right = simplifyNode(std::move(right));
        }

        return std::make_unique<BinaryOperationNode>(binaryNode->op, std::move(left), std::move(right));
    }

    if (auto unaryNode = dynamic_cast<UnaryOperationNode*>(node.get())) {
        auto operand = simplifyNode(std::move(unaryNode->operand));
        return std::make_unique<UnaryOperationNode>(unaryNode->func, std::move(operand));
    }

    return node->clone();
}

template<typename T>
void Expression<T>::skipWhitespace(const std::string& expr, size_t& pos) {
    while (pos < expr.size() && std::isspace(expr[pos])) {
        pos++;
    }
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::parseExpression(const std::string& expr, size_t& pos) {
    auto left = parseTerm(expr, pos);
    skipWhitespace(expr, pos);

    while (pos < expr.size()) {
        char op = expr[pos];
        if (op != '+' && op != '-') break;
        pos++;
        skipWhitespace(expr, pos);
        auto right = parseTerm(expr, pos);
        left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        skipWhitespace(expr, pos);
    }
    return left;
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::parseTerm(const std::string& expr, size_t& pos) {
    auto left = parseFactor(expr, pos);
    skipWhitespace(expr, pos);

    while (pos < expr.size()) {
        char op = expr[pos];
        if (op != '*' && op != '/') break;
        pos++;
        skipWhitespace(expr, pos);
        auto right = parseFactor(expr, pos);
        left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
        skipWhitespace(expr, pos);
    }
    return left;
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::parseFactor(const std::string& expr, size_t& pos) {
    auto left = parseUnary(expr, pos);
    skipWhitespace(expr, pos);

    while (pos < expr.size() && expr[pos] == '^') {
        pos++;
        skipWhitespace(expr, pos);
        auto right = parseUnary(expr, pos);
        left = std::make_unique<BinaryOperationNode>('^', std::move(left), std::move(right));
        skipWhitespace(expr, pos);
    }
    return left;
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::parseUnary(const std::string& expr, size_t& pos) {
    skipWhitespace(expr, pos);

    if (expr[pos] == '-') {
        pos++;
        skipWhitespace(expr, pos);
        auto operand = parseUnary(expr, pos);
        return std::make_unique<UnaryOperationNode>("-", std::move(operand));
    }

    return parsePrimary(expr, pos);
}

template<typename T>
std::unique_ptr<typename Expression<T>::Node> Expression<T>::parsePrimary(const std::string& expr, size_t& pos) {
    skipWhitespace(expr, pos);

    if (expr[pos] == '-') {
        pos++;
        skipWhitespace(expr, pos);
        auto operand = parsePrimary(expr, pos);
        return std::make_unique<UnaryOperationNode>("-", std::move(operand));
    }

    if (expr[pos] == '(') {
        pos++;
        skipWhitespace(expr, pos);

        auto realPart = parseExpression(expr, pos);
        skipWhitespace(expr, pos);

        if (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-')) {
            char sign = expr[pos];
            pos++;
            skipWhitespace(expr, pos);
            auto imaginaryPart = parseExpression(expr, pos);
            skipWhitespace(expr, pos);
            if (pos >= expr.size() || expr[pos] != 'i') {
                throw std::invalid_argument("ожидается 'i' в мнимой части");
            }
            pos++;
            skipWhitespace(expr, pos);

            if (expr[pos] != ')') {
                throw std::invalid_argument("нужна вторая скобка");
            }
            pos++;
            skipWhitespace(expr, pos);
            if (sign == '+') {
                return std::make_unique<BinaryOperationNode>('+', std::move(realPart), std::move(imaginaryPart));
            } else {
                return std::make_unique<BinaryOperationNode>('-', std::move(realPart), std::move(imaginaryPart));
            }
        } else {
            if (expr[pos] != ')') {
                throw std::invalid_argument("нужна вторая скобка");
            }
            pos++;
            skipWhitespace(expr, pos);
            return realPart;
        }
    }

    if (std::isdigit(expr[pos]) || expr[pos] == '.') {
        std::string numStr;
        while (pos < expr.size() && (std::isdigit(expr[pos]) || expr[pos] == '.')) {
            numStr += expr[pos++];
        }
        skipWhitespace(expr, pos);

        T value;
        if constexpr (std::is_same_v<T, std::complex<double>>) {
            value = std::complex<double>(std::stod(numStr), 0);
        } else {
            value = static_cast<T>(std::stod(numStr));
        }

        if (pos < expr.size() && (std::isalpha(expr[pos]) || expr[pos] == '(')) {
            auto left = std::make_unique<ConstantNode>(value);
            auto right = parsePrimary(expr, pos);
            return std::make_unique<BinaryOperationNode>('*', std::move(left), std::move(right));
        }

        return std::make_unique<ConstantNode>(value);
    }

    if (std::isalpha(expr[pos])) {
        std::string token;
        while (pos < expr.size() && std::isalpha(expr[pos])) {
            token += expr[pos++];
        }
        skipWhitespace(expr, pos);

        if (token == "sin" || token == "cos" || token == "ln" || token == "exp") {
            if (expr[pos] != '(') {
                throw std::invalid_argument("нужна первая скобка после функции");
            }
            pos++;
            skipWhitespace(expr, pos);
            auto operand = parseExpression(expr, pos);
            skipWhitespace(expr, pos);
            if (expr[pos] != ')') {
                throw std::invalid_argument("нужна вторая скобка после аргумента функции");
            }
            pos++;
            skipWhitespace(expr, pos);
            return std::make_unique<UnaryOperationNode>(token, std::move(operand));
        }
        if (token == "i") {
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                return std::make_unique<ConstantNode>(std::complex<double>(0, 1));
            } else {
                throw std::invalid_argument("мнимая единица поддерживается только для std::complex<double>");
            }
        }

        return std::make_unique<VariableNode>(token);
    }

    throw std::invalid_argument("неизвестный символ");
}

template<typename T>
void printResult(const T& value) {
    std::cout << value << std::endl;
}

template<>
void printResult<std::complex<double>>(const std::complex<double>& value) {
    if (value.imag() == 0) {
        std::cout << value.real() << std::endl;
    } else {
        std::cout << "(" << value.real() << ", " << value.imag() << ")" << std::endl;
    }
}

template<>
std::string Expression<std::complex<double>>::ConstantNode::toString() const {
    std::ostringstream oss;
    if (value.imag() == 0) {
        oss << value.real();
    } else {
        oss << "(" << value.real() << ", " << value.imag() << ")";
    }
    return oss.str();
}

template class Expression<double>;
template class Expression<std::complex<double>>;