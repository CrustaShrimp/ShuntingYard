#pragma once
#include "ShuntingYard.h"
#include "CMathTokenOperator.h"
#include <stack>
#include <string>
#include <stdexcept>
double ShuntingYard(CString strEquation)
{
    // Top
    // Push
    // Pop
    
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
    
    std::stack<double>                      NumStack;
    std::stack<CMathTokenOperator>          OperatorStack;
    double                                  dValueModifier = 1.0;
    
    // Step 1: Add spaces
    strEquation.Replace(_T("^"), _T(" ^ "));
    strEquation.Replace(_T("("), _T(" ( "));
    strEquation.Replace(_T(")"), _T(" ) "));
    strEquation.Replace(_T("*"), _T(" * "));
    strEquation.Replace(_T("/"), _T(" / "));
    strEquation.Replace(_T("+"), _T(" + "));
    strEquation.Replace(_T("-"), _T(" - "));

    // Step 2: Tokenize
    const CString strSeperator = _T(" ");
    int iPosition = 0;
    CString strToken;

    strToken = strEquation.Tokenize(strSeperator, iPosition);
    while (!strToken.IsEmpty())
    {
        // Test whether the token is a value or operator
        try
        {
            const double dValue = std::stod(strToken.GetString());
            NumStack.push(dValue * dValueModifier);
        }
        catch (const std::invalid_argument&)
        {
            // the token was not a value, it must be an operator
            const CMathTokenOperator Operator(strToken);
            if (Operator.IsOpenBrace() || OperatorStack.empty())
            {
                // Add to stack
                OperatorStack.push(Operator);
            }
            else if (Operator.IsCloseBrace())
            {
                // Always process until the previous open brace
                while (!OperatorStack.empty() && !OperatorStack.top().IsOpenBrace())
                {
                    assert(NumStack.size() >= 2);
                    const double dSecondOperand = NumStack.top();
                    NumStack.pop();
                    const double dFirstOperand = NumStack.top();
                    NumStack.pop();
                    CMathTokenOperator OperatorFromStack = OperatorStack.top();
                    OperatorStack.pop();
                    const double dResult = OperatorFromStack.ProcessOperator(dFirstOperand, dSecondOperand);
                    NumStack.push(dResult);
                }
                // Pop the remaining open brace
                OperatorStack.pop();
            }
            else
            {
                while (!OperatorStack.empty() && Operator < OperatorStack.top())
                {
                    assert(NumStack.size() >= 2);
                    // Process the stack operator immediately
                    const double dSecondOperand = NumStack.top();
                    NumStack.pop();
                    const double dFirstOperand = NumStack.top();
                    NumStack.pop();
                    CMathTokenOperator OperatorFromStack = OperatorStack.top();
                    OperatorStack.pop();
                    const double dResult = OperatorFromStack.ProcessOperator(dFirstOperand, dSecondOperand);
                    NumStack.push(dResult);
                }
                OperatorStack.push(Operator);
            }
            
        }
        // Get next token.
        strToken = strEquation.Tokenize(strSeperator, iPosition);
    }
    // Done processing the tokens, now process the remaining operator stack
    while (!OperatorStack.empty())
    {
        const CMathTokenOperator Operator = OperatorStack.top();
        OperatorStack.pop();

        const double dSecondOperand = NumStack.top();
        NumStack.pop();
        const double dFirstOperand = NumStack.top();
        NumStack.pop();
        const double dResult = Operator.ProcessOperator(dFirstOperand, dSecondOperand);
        NumStack.push(dResult);
    }
    assert(NumStack.size() == 1);
    return NumStack.top();
}
