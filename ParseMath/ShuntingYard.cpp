#pragma once
#include "ShuntingYard.h"

#include <string>
#include <stdexcept>
#include <iostream>

int CharacterCount(const CString& strSearchString, const LPCTSTR SearchChar)
{
    if (strSearchString.IsEmpty() || L"" == SearchChar)
    {
        return 0;
    }
    int nFind = -1;
    int nCount = 0;
    while (-1 != (nFind = strSearchString.Find(SearchChar, nFind + 1)))
    {
        nCount++;
    }
    return nCount;
}


double CShuntingYard::Compute(const CString& strEquation) noexcept(false)
{
    /* Calculation examples:
    * 5+8*-7
    * 7 * (5 + 8)
    *
    ** First add spaces to simplify parsing
    * Since we separate on space, any extra spaces are not a problem and are automatically discarded
    *
    ** Result:
    * 5 + 8 * - 7
    * 7 * ( 5 + 8 )
    *
    *
    ** Tokenize:
    * Token = 5
    * Check whether the token is an operator or a numeric value
    * In case its a value, push the token onto the numstack, always
    * Numstack = {5}
    * In case its an operator: 
    * Check validity of the calculation, two operators cannot follow each other, but a - is a special case, since it may belong to the next value
    * e.g. -7 is split into - 7 so the - may not be an operator at all, just a value.
    * In this case, we have to be smart and either change the previous operator (in case it was + it can become - and vice versa), or otherwise we have to negate the next value
    * If all is correct, compare the operator with the .top of the operator stack
    * In case it has higher precedence, add it to the stack
    * In case it has lower precedence, process the .top of the operator stack first
    * Repeat until the newest .top has lower precedence than the newly read operator, then add the operator to the stack
    * In case of same precedence, check the associativity for left/right and act accordingly
    * 
    ** Processing operator:
    * Get the operator (probably already done anyway) by .top
    * .pop the operator from the operator stack
    * Read the .top and store it in a double
    * .pop the stack
    * read the .top and store it in a double
    * .pop the stack
    * Perform the calculation
    * .push the result onto the stack
    *
    ** Repeat until the token is "" or maybe until iPosition == -1
    *
    ** At the end, process the remaining operator stack using the method described earlier
    *
    ** After this, the numstack should only contain one number, the end result
    *
    ** Smiley, you are done! :)
    *
    */

    m_NumStack.clear();
    m_OperatorStack.clear();
    m_bNextValueNegative = false;
    const LPCTSTR pszOpenCloseLine = _T("****************************************************************************\n");

    // Step 0: validate braces
    {
        const int iNumberOfOpenBraces       = CharacterCount(strEquation, _T("("));
        const int iNumberOfClosingBraces    = CharacterCount(strEquation, _T(")"));
        if (iNumberOfOpenBraces != iNumberOfClosingBraces)
        {
            // Problem in equation, missing ( or )
            const CString strMissingOpen = iNumberOfOpenBraces < iNumberOfClosingBraces ? _T("open") : _T("close");
            std::wcout << _T("Missing ") << strMissingOpen.GetString() << _T(" brace in equation ") << strEquation.GetString() << _T(", not able to process this equation\n\n");
            return 0.0;
        }
    }

    // Make a local copy of the string, which we can edit
    CString strEquationWithSpacing(strEquation);

    // Step 1: Add spaces
    strEquationWithSpacing.Replace(_T("^"), _T(" ^ "));
    strEquationWithSpacing.Replace(_T("("), _T(" ( "));
    strEquationWithSpacing.Replace(_T(")"), _T(" ) "));
    strEquationWithSpacing.Replace(_T("*"), _T(" * "));
    strEquationWithSpacing.Replace(_T("/"), _T(" / "));
    strEquationWithSpacing.Replace(_T("+"), _T(" + "));
    strEquationWithSpacing.Replace(_T("-"), _T(" - "));

    {
        std::wcout << pszOpenCloseLine << _T("Equation to evaluate:") << strEquationWithSpacing.GetString() << _T("\n");
    }

    // Step 2: Tokenize
    int iPosition = 0;
    CString strToken;
    bool bPreviousTokenWasValue = false;

    strToken = strEquationWithSpacing.Tokenize(m_strSeperator, iPosition);
    while (!strToken.IsEmpty())
    {
        // Test whether the token is a value or operator
        try
        {
            const double dValue = std::stod(strToken.GetString());
            // If this has succeeded, we have a value which we can push to the stack, taking into account the optional previous value modifier (-1)
            m_NumStack.push_back(m_bNextValueNegative ? -dValue : dValue);
            // Reset the value modifier to +
            m_bNextValueNegative = false;
            bPreviousTokenWasValue = true;
        }
        catch (const std::invalid_argument&)
        {
            // the token was not a value, it must be an operator
            const CMathTokenOperator Operator(strToken);
            if (Operator.IsOpenBrace())
            {
                // Check previous token, if it is not an operator we are missing an operator, so assume it is a multiplication: 4(5+3) = 4 * (5 + 3)
                if (bPreviousTokenWasValue && !m_NumStack.empty())
                {
                    m_OperatorStack.push_back(CMathTokenOperator(_T("*")));
                }
                // Add to stack
                m_OperatorStack.push_back(Operator);
                bPreviousTokenWasValue = false;
            }
            else if (Operator.IsCloseBrace())
            {
                // Always process until the previous open brace
                while (!m_OperatorStack.empty() && !m_OperatorStack.back().IsOpenBrace())
                {
                    ProcessLastOperatorFromStack();
                }
                // Pop the remaining open brace
                m_OperatorStack.pop_back();
                bPreviousTokenWasValue = true; // A bit weird, but since the brace is computed, a value is set last
            }
            else if (Operator.IsMinus())
            {
                // Check previous token, if it is an operator we are not an operator but part of a negative number
                if (!bPreviousTokenWasValue)
                {
                    // Invert the bool, since two negatives will lead to a positive
                    m_bNextValueNegative = !m_bNextValueNegative;
                }
                else
                {
                    // Add to stack
                    AddToStackAndProcessHigherPrecedenceOperators(Operator);
                    bPreviousTokenWasValue = false;
                }
            }
            else
            {
                // Check whether the previous token was a value, if not we have an incorrect equation
                // Best guess: just ignore the token and hope for the best
                if (bPreviousTokenWasValue)
                {
                    AddToStackAndProcessHigherPrecedenceOperators(Operator);
                    bPreviousTokenWasValue = false;
                }
            }
        }
        // Print the current numstack and operator stack:
        PrintStacks();
        // Get next token.
        strToken = strEquationWithSpacing.Tokenize(m_strSeperator, iPosition);
    }
    // Done processing the tokens, now process the remaining operator stack
    while (!m_OperatorStack.empty())
    {
        ProcessLastOperatorFromStack();
    }
    assert(m_NumStack.size() == 1);
    std::wcout << _T("The result from the equation ") << strEquationWithSpacing.GetString() << _T(" is ") << m_NumStack[0] << _T("\n") << pszOpenCloseLine << _T("\n");

    return m_NumStack[0];
}

void CShuntingYard::ProcessLastOperatorFromStack()
{
    assert(m_NumStack.size() >= 2);
    // Process the stack operator immediately
    const double dSecondOperand = m_NumStack.back();
    m_NumStack.pop_back();
    const double dFirstOperand = m_NumStack.back();
    m_NumStack.pop_back();
    const CMathTokenOperator OperatorFromStack = m_OperatorStack.back();
    m_OperatorStack.pop_back();
    const double dResult = OperatorFromStack.ProcessOperator(dFirstOperand, dSecondOperand);
    m_NumStack.push_back(dResult);

    std::wcout << _T("Processed operation ") << dFirstOperand << OperatorFromStack.GetStr().GetString() << dSecondOperand << _T(", resulting in value ") << dResult << _T("\n") << _T("The current stacks are:\n");
    PrintStacks();
}

void CShuntingYard::AddToStackAndProcessHigherPrecedenceOperators(CMathTokenOperator Operator)
{
    while (!m_OperatorStack.empty() && Operator < m_OperatorStack.back())
    {
        ProcessLastOperatorFromStack();
    }
    m_OperatorStack.push_back(Operator);
}

void CShuntingYard::PrintStacks() const
{
    std::wcout << _T("Current numstack:");
    for (const double dNum : m_NumStack)
    {
        std::wcout << dNum << _T(" ");
    }
    std::wcout << _T("\n") << _T("Current operator stack:");
    for (const CMathTokenOperator& Operator : m_OperatorStack)
    {
        std::wcout << Operator.GetStr().GetString() << _T(" ");
    }
    std::wcout << _T("\n\n");
}
