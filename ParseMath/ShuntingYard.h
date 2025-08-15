#pragma once
#include <vector>
#include "CMathTokenOperator.h"

class CShuntingYard
{
public:
    double Compute(const std::string& strEquation) noexcept(false);

private:
    void ProcessLastOperatorFromStack();
    void AddToStackAndProcessHigherPrecedenceOperators(CMathTokenOperator Operator);

    const std::string                       m_strSeperator = (" ");
    std::vector<double>                     m_NumStack;
    std::vector<CMathTokenOperator>         m_OperatorStack;
    bool                                    m_bNextValueNegative = false;
    void PrintStacks() const;
};
