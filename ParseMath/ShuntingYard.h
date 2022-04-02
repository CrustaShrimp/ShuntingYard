#pragma once
#include <atlstr.h>
#include <vector>
#include "CMathTokenOperator.h"

class CShuntingYard
{
public:
    double Compute(const CString& strEquation) noexcept(false);

private:
    void ProcessLastOperatorFromStack();
    void AddToStackAndProcessHigherPrecedenceOperators(CMathTokenOperator Operator);

    const CString                           m_strSeperator = _T(" ");
    std::vector<double>                     m_NumStack;
    std::vector<CMathTokenOperator>         m_OperatorStack;
    bool                                    m_bNextValueNegative = false;
    void PrintStacks() const;
};
