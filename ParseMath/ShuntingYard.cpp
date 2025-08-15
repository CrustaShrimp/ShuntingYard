#include "ShuntingYard.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <stdexcept>
#include <iostream>

int CharacterCount(const std::string& strSearchString, const std::string::value_type SearchChar)
{
    if (strSearchString.empty() || SearchChar == '\0')
    {
        return 0;
    }
    int nFind = -1;
    int nCount = 0;
    while (std::string::npos != (nFind = static_cast<int>(strSearchString.find(SearchChar, nFind + 1))))
    {
        nCount++;
    }
    return nCount;
}


void CShuntingYard::ProcessNumber(bool &bPreviousTokenWasValue, const std::string& Number) {
    try
    {
        const double dValue = std::stod(Number);
        // If this has succeeded, we have a value which we can push to the stack, taking into account the optional previous value modifier (-1)
        m_NumStack.push_back(m_bNextValueNegative ? -dValue : dValue);
        // Reset the value modifier to +
        m_bNextValueNegative = false;
        bPreviousTokenWasValue = true;
    }
    catch (const std::invalid_argument&) {
        // TODO Handle exception
        assert(false);
    }
}

double CShuntingYard::Compute(const std::string& strEquation) noexcept(false)
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
    if (strEquation.empty())
    {
        return 0.0;
    }
    m_NumStack.clear();
    m_OperatorStack.clear();
    m_bNextValueNegative = false;
    const std::string OpenCloseLine = ("****************************************************************************\n");

    // Step 0: validate braces
    {
        const int iNumberOfOpenBraces       = CharacterCount(strEquation, '(');
        const int iNumberOfClosingBraces    = CharacterCount(strEquation, ')');
        if (iNumberOfOpenBraces != iNumberOfClosingBraces)
        {
            // Problem in equation, missing ( or )
            const std::string strMissingOpen = iNumberOfOpenBraces < iNumberOfClosingBraces ? ("open") : ("close");
            std::wcout << ("Missing ") << strMissingOpen.c_str() << (" brace in equation ") << strEquation.c_str() << (", not able to process this equation\n\n");
            return 0.0;
        }
    }
    {
        std::wcout << OpenCloseLine.c_str() << ("Equation to evaluate:") << strEquation.c_str() << ("\n");
    }

    // Step 2: Tokenize
    size_t StartPos = 0;
    bool bPreviousTokenWasValue = false;

    size_t TokenPos = strEquation.find_first_of(CMathTokenOperator::GetSupportedTokens(), StartPos);
    while (TokenPos != std::string::npos)
    {
        std::string Number = strEquation.substr(StartPos, TokenPos - StartPos);
        if (!Number.empty()) {
            ProcessNumber(bPreviousTokenWasValue, Number);
        }
        StartPos = std::min(TokenPos + 1, strEquation.size());
        auto OperatorToken = strEquation.at(TokenPos);
        // the token was not a value, it must be an operator
        const CMathTokenOperator Operator(OperatorToken);
        if (Operator.IsOpenBrace())
        {
            // Check previous token, if it is not an operator we are missing an operator, so assume it is a multiplication: 4(5+3) = 4 * (5 + 3)
            if (bPreviousTokenWasValue && !m_NumStack.empty())
            {
                m_OperatorStack.emplace_back('*');
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
        // Print the current numstack and operator stack:
        PrintStacks();
        // Get next token.
        TokenPos = strEquation.find_first_of(CMathTokenOperator::GetSupportedTokens(), StartPos);
    }
    if (StartPos < strEquation.size()) {
        // one more number to process
        std::string Number = strEquation.substr(StartPos, strEquation.size() - StartPos);
        ProcessNumber(bPreviousTokenWasValue, Number);
    }
    // Done processing the tokens, now process the remaining operator stack
    while (!m_OperatorStack.empty())
    {
        ProcessLastOperatorFromStack();
    }
    assert(m_NumStack.size() == 1);
    std::wcout << ("The result from the equation ") << strEquation.c_str() << (" is ") << m_NumStack[0] << ("\n") << OpenCloseLine.c_str() << ("\n");

    return m_NumStack[0];
}

void CShuntingYard::ProcessLastOperatorFromStack()
{
    std::size_t NumStackSize = m_NumStack.size();
    assert(NumStackSize >= 2);
    // Process the stack operator immediately
    const double dSecondOperand = m_NumStack.back();
    m_NumStack.pop_back();
    const double dFirstOperand = m_NumStack.back();
    m_NumStack.pop_back();
    const CMathTokenOperator OperatorFromStack = m_OperatorStack.back();
    m_OperatorStack.pop_back();
    const double dResult = OperatorFromStack.ProcessOperator(dFirstOperand, dSecondOperand);
    m_NumStack.push_back(dResult);

    std::wcout << ("Processed operation ") << dFirstOperand << OperatorFromStack.GetStr().c_str() << dSecondOperand << (", resulting in value ") << dResult << ("\n") << ("The current stacks are:\n");
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
    std::wcout << ("Current numstack:");
    for (const double dNum : m_NumStack)
    {
        std::wcout << dNum << (" ");
    }
    std::wcout << ("\n") << ("Current operator stack:");
    for (const CMathTokenOperator& Operator : m_OperatorStack)
    {
        std::wcout << Operator.GetStr().c_str() << (" ");
    }
    std::wcout << ("\n\n");
}
