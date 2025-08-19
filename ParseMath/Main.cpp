#include "ShuntingYard.h"

#include <cassert>
#include <iomanip>
#include <cstdlib>

int main(int /*argc*/, char** /*/argv[] */ )
{
    CShuntingYard Algorithm;
    //Basic: just a number
    std::string strEquation("5");
    auto Result = Algorithm.Compute(strEquation);
    assert(Result == 5);

    // 0) Simplest equation
    strEquation = ("5 + 8 + 10");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 23);

    // 1) Simple equation
    strEquation = ("5 + 8 * 10");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 85);

    // 2) Simple equation with and without spaces
    strEquation = ("5+8 *10");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 85);

    // 3) Complexer equation
    strEquation = ("456*9+1385-12.5");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 5476.5);

    // 4) Equation with power
    strEquation = ("5^3+8*7");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 181);

    // 4.1) Equation with double power
    strEquation = ("5^3^2");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 1953125);

    // 5) Equation with braces
    strEquation = ("(12+5)*8^2");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 1088);

    // 5).2 Equation with braces
    strEquation = ("(12+5)+8^2");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 81);

    // 5.3) Equation with double braces
    strEquation = ("(12+5*(3+3))*8^2");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 2688);

    // 6) Equation with braces and missing operator
    strEquation = ("2(12+5)*8^2");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 2176);

    // 7) Equation with negative numbers
    strEquation = ("-8*5+6 *-3");
    Result = Algorithm.Compute(strEquation);
    assert(Result == -58);

    // 8) Equation with negative numbers pt 2
    strEquation = ("-8*5--6*-3");
    Result = Algorithm.Compute(strEquation);
    assert(Result == -58);

    // 8) Invalid equations and operators...
    strEquation = ("456&9");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 456);
    // Operator not supported

    strEquation = ("456|9");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 456);
    // Operator not supported

    strEquation = ("456**9");
    Result = Algorithm.Compute(strEquation);
    assert(Result == 4104);
    // Double operator, ignored

    strEquation = ("456^(9+5");
    Result = Algorithm.Compute(strEquation);
    // Missing closing brace

    return EXIT_SUCCESS; // optional return value
}