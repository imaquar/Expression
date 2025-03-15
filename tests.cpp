#include "expression.hpp"
#include <iostream>

void tests() {

    Expression<double> a1(7.6);
    Expression<double> a2(0.4);

    Expression<double> sum1 = a1 + a2;
    std::map<std::string, double> variables_sum1;
    auto result_sum1 = sum1.evaluate(variables_sum1);
    if (*result_sum1 == 8) {
        std::cout << "Test 1: OK" << std::endl;
    }
    else {
        std::cout << "Test 1: FAIL" << std::endl;
    }

    std::complex<double> complex_a3(2.0, 3.0);
    std::complex<double> complex_a4(4.0, -1.0);
    std::complex<double> check_sum2(6.0, 2.0);

    Expression<std::complex<double>> a3(complex_a3);
    Expression<std::complex<double>> a4(complex_a4);

    Expression<std::complex<double>> sum2 = a3 + a4;
    std::map<std::string, std::complex<double>> variables_sum2;
    auto result_sum2 = sum2.evaluate(variables_sum2);
    if (result_sum2->real() == check_sum2.real() && result_sum2->imag() == check_sum2.imag()) {
        std::cout << "Test 2: OK" << std::endl;
    }
    else {
        std::cout << "Test 2: FAIL" << std::endl;
    }

    Expression<double> b1(10.0);
    Expression<double> b2(5.5);

    Expression<double> diff1 = b1 - b2;
    std::map<std::string, double> variables_diff1;
    auto result_diff1 = diff1.evaluate(variables_diff1);
    if (*result_diff1 == 4.5) {
        std::cout << "Test 3: OK" << std::endl;
    }
    else {
        std::cout << "Test 3: FAIL" << std::endl;
    }

    std::complex<double> complex_b3(7.0, 4.0);
    std::complex<double> complex_b4(6.0, -1.0);
    std::complex<double> check_diff2(1.0, 5.0);

    Expression<std::complex<double>> b3(complex_b3);
    Expression<std::complex<double>> b4(complex_b4);

    Expression<std::complex<double>> diff2 = b3 - b4;
    std::map<std::string, std::complex<double>> variables_diff2;
    auto result_diff2 = diff2.evaluate(variables_diff2);
    if (result_diff2->real() == check_diff2.real() && result_diff2->imag() == check_diff2.imag()) {
        std::cout << "Test 4: OK" << std::endl;
    }
    else {
        std::cout << "Test 4: FAIL" << std::endl;
    }

    Expression<double> c1(10.0);
    Expression<double> c2(2.0);

    Expression<double> prod1 = c1 * c2;
    std::map<std::string, double> variables_prod1;
    auto result_prod1 = prod1.evaluate(variables_prod1);
    if (*result_prod1 == 20.0) {
        std::cout << "Test 5: OK" << std::endl;
    }
    else {
        std::cout << "Test 5: FAIL" << std::endl;   
    }

    std::complex<double> complex_c3(2.0, 4.0);
    std::complex<double> complex_c4(6.0, 1.0);
    std::complex<double> check_prod2(8.0, 26.0);

    Expression<std::complex<double>> c3(complex_c3);
    Expression<std::complex<double>> c4(complex_c4);

    Expression<std::complex<double>> prod2 = c3 * c4;
    std::map<std::string, std::complex<double>> variables_prod2;
    auto result_prod2 = prod2.evaluate(variables_prod2);
    if (result_prod2->real() == check_prod2.real() && result_prod2->imag() == check_prod2.imag()) {
        std::cout << "Test 6: OK" << std::endl;
    }
    else {
        std::cout << "Test 6: FAIL" << std::endl;
    }

    Expression<double> d1(10.0);
    Expression<double> d2(8.0);

    Expression<double> div1 = d1 / d2;
    std::map<std::string, double> variables_div1;
    auto result_div1 = div1.evaluate(variables_div1);
    if (*result_div1 == 1.25) {
        std::cout << "Test 7: OK" << std::endl;
    }
    else {
        std::cout << "Test 7: FAIL" << std::endl;   
    }

    std::complex<double> complex_d3(10.0, 10.0);
    std::complex<double> complex_d4(2.0, 4.0);
    std::complex<double> check_div2(3.0, -1.0);

    Expression<std::complex<double>> d3(complex_d3);
    Expression<std::complex<double>> d4(complex_d4);

    Expression<std::complex<double>> div2 = d3 / d4;
    std::map<std::string, std::complex<double>> variables_div2;
    auto result_div2 = div2.evaluate(variables_div2);
    if (result_div2->real() == check_div2.real() && result_div2->imag() == check_div2.imag()) {
        std::cout << "Test 8: OK" << std::endl;
    }
    else {
        std::cout << "Test 8: FAIL" << std::endl;
    }

    Expression<double> e1(5.0);
    Expression<double> e2(5.0);

    Expression<double> pow1 = e1 ^ e2;
    std::map<std::string, double> variables_pow1;
    auto result_pow1 = pow1.evaluate(variables_pow1);
    if (*result_pow1 == 3125.0) {
        std::cout << "Test 9: OK" << std::endl;
    }
    else {
        std::cout << "Test 9: FAIL" << std::endl;   
    }

    Expression<double> a("a");
    Expression<double> b("b");

    Expression<double> sin1 = a * b.sin();
    std::map<std::string, double> variables_sin1 = {{"a", 2.0}, {"b", 1.0}};
    auto result_sin1 = sin1.evaluate(variables_sin1);
    if (result_sin1 && std::fabs(*result_sin1 - 1.68294) < 1e-5) {
        std::cout << "Test 10: OK" << std::endl;
    }
    else {
        std::cout << "Test 10: FAIL" << std::endl;
    }

    Expression<double> c("c");
    Expression<double> d("d");

    Expression<double> cos1 = c + d.cos();
    std::map<std::string, double> variables_cos1 = {{"c", 4.0}, {"d", 1.0}};
    auto result_cos1 = cos1.evaluate(variables_cos1);
    if (result_cos1 && std::fabs(*result_cos1 - 4.54030) < 1e-5) {
        std::cout << "Test 11: OK" << std::endl;
    }
    else {
        std::cout << "Test 11: FAIL" << std::endl;
    }

    Expression<double> e("e");
    Expression<double> f("f");

    Expression<double> ln1 = e - f.ln();
    std::map<std::string, double> variables_ln1 = {{"e", 4.0}, {"f", 5.0}};
    auto result_ln1 = ln1.evaluate(variables_ln1);
    if (result_ln1 && std::fabs(*result_ln1 - 2.39056) < 1e-5) {
        std::cout << "Test 12: OK" << std::endl;
    }
    else {
        std::cout << "Test 12: FAIL" << std::endl;
    }

    Expression<double> g("g");
    Expression<double> h("h");

    Expression<double> exp1 = g / h.exp();
    std::map<std::string, double> variables_exp1 = {{"g", 100.0}, {"h", 1.0}};
    auto result_exp1 = exp1.evaluate(variables_exp1);
    if (result_exp1 && std::fabs(*result_exp1 - 36.78794) < 1e-5) {
        std::cout << "Test 13: OK" << std::endl;
    }
    else {
        std::cout << "Test 13: FAIL" << std::endl;
    }

    Expression<double> expr = Expression<double>::fromString("sin(2x) - 10(x^3)");
    std::cout << "Выражение: " << expr.toString() << std::endl;

    Expression<double> diffExpr = expr.differentiate("x").simplify();
    std::cout << "Производная: " << diffExpr.toString() << std::endl;

}

int main() {

    tests();

    return 0;

}




// int main() {

//     Expression<double> z("z");
//     Expression<double> w("w");
//     Expression<double> x("x");

//     Expression<double> expr1 = z * w.ln() - (x ^ 2.0);

//     std::cout << "выражение: " << expr1.toString() << std::endl;

//     std::map<std::string, double> variables = {{"z", 4.5}, {"w", 8.0}, {"x", 2.0}};
//     auto result = expr1.evaluate(variables);

//     if (result) {
//         std::cout << "вычисление: " << *result << std::endl;
//     } else {
//         std::cout << "вычисление без переменной: " << expr1.toStringWithSubstitution(variables) << std::endl;
//     }

//     Expression<double> expr = Expression<double>::fromString("-sin(2x) - 10(x^3)");
//     std::cout << "выражение: " << expr.toString() << std::endl;

//     Expression<double> diffExpr = expr.differentiate("x").simplify();
//     std::cout << "производная: " << diffExpr.toString() << std::endl;

//     return 0;
// }