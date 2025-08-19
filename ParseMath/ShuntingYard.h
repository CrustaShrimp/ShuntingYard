#pragma once

#include "CMathTokenOperator.h"

#include <vector>
#include <algorithm>
#include <string>

class CShuntingYard
{
public:
    std::expected<double, ErrorType> Compute(const std::string& strEquation) noexcept(false);

private:
    std::optional<ErrorType> ProcessNumber(bool &bPreviousTokenWasValue, const std::string& Number);

    std::optional<ErrorType> ProcessLastOperatorFromStack();
    std::optional<ErrorType> AddToStackAndProcessHigherPrecedenceOperators(CMathTokenOperator Operator);

    const std::string          m_strSeperator = (" ");
    std::vector<double>                     m_NumStack;
    std::vector<CMathTokenOperator>  m_OperatorStack;
    bool                                    m_bNextValueNegative = false;
    void PrintStacks() const;
};