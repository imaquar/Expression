#include <string>
#include <memory>
#include <map>
#include <complex>
#include <stdexcept>
#include <sstream>
#include <optional>
#include <vector>
#include <cctype>

template<typename T>
void printResult(const T& value);

template<>
void printResult<std::complex<double>>(const std::complex<double>& value);

template<typename T>
class Expression {
public:

    Expression(T value);
    Expression(const std::string& variable);
    Expression(const Expression& other);
    Expression(Expression&& other) noexcept;

    ~Expression();

    Expression& operator=(const Expression& other);
    Expression& operator=(Expression&& other) noexcept;

    Expression operator+(const Expression& other) const;
    Expression operator-(const Expression& other) const;
    Expression operator*(const Expression& other) const;
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;

    Expression sin() const;
    Expression cos() const;
    Expression ln() const;
    Expression exp() const;

    Expression simplify() const;

    Expression substitute(const std::string& variable, T value) const;

    std::optional<T> evaluate(const std::map<std::string, T>& variables) const;

    std::string toString() const;

    std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const;

    static Expression fromString(const std::string& expr);

    Expression differentiate(const std::string& variable) const;

private:
    struct Node {
        virtual ~Node() = default;
        virtual std::optional<T> evaluate(const std::map<std::string, T>& variables) const = 0;
        virtual std::string toString() const = 0;
        virtual std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const = 0;
        virtual std::unique_ptr<Node> clone() const = 0;
        virtual std::unique_ptr<Node> substitute(const std::string& variable, T value) = 0;
        virtual int precedence() const = 0;
        virtual std::unique_ptr<Node> differentiate(const std::string& variable) const = 0;
    };

    struct ConstantNode : Node {
        T value;
        ConstantNode(T value) : value(value) {}
        std::optional<T> evaluate(const std::map<std::string, T>& variables) const override { return value; }
        std::string toString() const override;
        std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const override {
            return toString();
        }
        std::unique_ptr<Node> clone() const override { return std::make_unique<ConstantNode>(value); }
        std::unique_ptr<Node> substitute(const std::string& variable, T value) override {
            return clone();
        }
        int precedence() const override { return 0; }
        std::unique_ptr<Node> differentiate(const std::string& variable) const override {
            return std::make_unique<ConstantNode>(0);
        }
    };

    struct VariableNode : Node {
        std::string name;
        VariableNode(const std::string& name) : name(name) {}
        std::optional<T> evaluate(const std::map<std::string, T>& variables) const override;
        std::string toString() const override { return name; }
        std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const override {
            auto it = variables.find(name);
            if (it != variables.end()) {
                std::ostringstream oss;
                oss << it->second;
                return oss.str();
            }
            return name;
        }
        std::unique_ptr<Node> clone() const override { return std::make_unique<VariableNode>(name); }
        std::unique_ptr<Node> substitute(const std::string& variable, T value) override;
        int precedence() const override { return 0; }
        std::unique_ptr<Node> differentiate(const std::string& variable) const override {
            if (name == variable) {
                return std::make_unique<ConstantNode>(1);
            }
            return std::make_unique<ConstantNode>(0);
        }
    };

    struct BinaryOperationNode : Node {
        char op;
        std::unique_ptr<Node> left, right;
        BinaryOperationNode(char op, std::unique_ptr<Node> left, std::unique_ptr<Node> right) : op(op), left(std::move(left)), right(std::move(right)) {}
        std::optional<T> evaluate(const std::map<std::string, T>& variables) const override;
        std::string toString() const override;
        std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const override;
        std::unique_ptr<Node> clone() const override {
            return std::make_unique<BinaryOperationNode>(op, left->clone(), right->clone());
        }
        std::unique_ptr<Node> substitute(const std::string& variable, T value) override {
            auto newLeft = left->substitute(variable, value);
            auto newRight = right->substitute(variable, value);
            return std::make_unique<BinaryOperationNode>(op, std::move(newLeft), std::move(newRight));
        }
        int precedence() const override {
            switch (op) {
                case '^': return 4;
                case '*': case '/': return 3;
                case '+': case '-': return 2;
                default: return 1;
            }
        }
        std::unique_ptr<Node> differentiate(const std::string& variable) const override {
            auto leftDiff = left->differentiate(variable);
            auto rightDiff = right->differentiate(variable);

            switch (op) {
                case '+': return std::make_unique<BinaryOperationNode>('+', std::move(leftDiff), std::move(rightDiff));
                case '-': return std::make_unique<BinaryOperationNode>('-', std::move(leftDiff), std::move(rightDiff));
                case '*': {
                    auto leftRight = std::make_unique<BinaryOperationNode>('*', left->clone(), std::move(rightDiff));
                    auto rightLeft = std::make_unique<BinaryOperationNode>('*', right->clone(), std::move(leftDiff));
                    return std::make_unique<BinaryOperationNode>('+', std::move(leftRight), std::move(rightLeft));
                }
                case '/': {
                    auto numerator1 = std::make_unique<BinaryOperationNode>('*', left->clone(), std::move(rightDiff));
                    auto numerator2 = std::make_unique<BinaryOperationNode>('*', right->clone(), std::move(leftDiff));
                    auto numerator = std::make_unique<BinaryOperationNode>('-', std::move(numerator1), std::move(numerator2));
                    auto denominator = std::make_unique<BinaryOperationNode>('^', right->clone(), std::make_unique<ConstantNode>(2));
                    return std::make_unique<BinaryOperationNode>('/', std::move(numerator), std::move(denominator));
                }
                case '^': {
                    if (right->toString() == "2") {
                        return std::make_unique<BinaryOperationNode>('*', std::make_unique<ConstantNode>(2), left->clone());
                    }
                    return std::make_unique<BinaryOperationNode>('*', right->clone(), std::make_unique<BinaryOperationNode>('^', left->clone(), std::make_unique<ConstantNode>(std::stod(right->toString()) - 1)));
                }
                default: throw std::invalid_argument("неизвестный оператор");
            }
        }
    };

    struct UnaryOperationNode : Node {
        std::string func;
        std::unique_ptr<Node> operand;
        UnaryOperationNode(const std::string& func, std::unique_ptr<Node> operand) : func(func), operand(std::move(operand)) {}
    
        std::optional<T> evaluate(const std::map<std::string, T>& variables) const override;
        std::string toString() const override;
        std::string toStringWithSubstitution(const std::map<std::string, T>& variables) const override;
    
        std::unique_ptr<Node> clone() const override {
            return std::make_unique<UnaryOperationNode>(func, operand->clone());
        }
    
        std::unique_ptr<Node> substitute(const std::string& variable, T value) override {
            auto newOperand = operand->substitute(variable, value);
            return std::make_unique<UnaryOperationNode>(func, std::move(newOperand));
        }
    
        int precedence() const override { return 5; }
    
        std::unique_ptr<Node> differentiate(const std::string& variable) const override {
            if (func == "-") {
                auto operandDiff = operand->differentiate(variable);
                return std::make_unique<UnaryOperationNode>("-", std::move(operandDiff));
            }
            if (func == "sin") {
                auto cosNode = std::make_unique<UnaryOperationNode>("cos", operand->clone());
                auto operandDiff = operand->differentiate(variable);
                return std::make_unique<BinaryOperationNode>('*', std::move(cosNode), std::move(operandDiff));
            }
            if (func == "cos") {
                auto sinNode = std::make_unique<UnaryOperationNode>("sin", operand->clone());
                auto operandDiff = operand->differentiate(variable);
                auto negSinNode = std::make_unique<UnaryOperationNode>("-", std::move(sinNode));
                return std::make_unique<BinaryOperationNode>('*', std::move(negSinNode), std::move(operandDiff));
            }
            if (func == "ln") {
                auto operandDiff = operand->differentiate(variable);
                return std::make_unique<BinaryOperationNode>('/', std::move(operandDiff), operand->clone());
            }
            if (func == "exp") {
                auto expNode = std::make_unique<UnaryOperationNode>("exp", operand->clone());
                auto operandDiff = operand->differentiate(variable);
                return std::make_unique<BinaryOperationNode>('*', std::move(expNode), std::move(operandDiff));
            }
            throw std::invalid_argument("неизвестная функция");
        }
    };

    std::unique_ptr<Node> root;

    Expression(std::unique_ptr<Node> root) : root(std::move(root)) {}
    
    static std::unique_ptr<Node> simplifyNode(std::unique_ptr<Node> node);
    static void skipWhitespace(const std::string& expr, size_t& pos);
    static std::unique_ptr<Node> parseUnary(const std::string& expr, size_t& pos);
    static std::unique_ptr<Node> parseExpression(const std::string& expr, size_t& pos);
    static std::unique_ptr<Node> parseTerm(const std::string& expr, size_t& pos);
    static std::unique_ptr<Node> parseFactor(const std::string& expr, size_t& pos);
    static std::unique_ptr<Node> parsePrimary(const std::string& expr, size_t& pos);
};

template<>
std::string Expression<std::complex<double>>::ConstantNode::toString() const;

template<typename T>
std::string Expression<T>::ConstantNode::toString() const {
    return std::to_string(value);
}
