#pragma once
#include "IMathToken.h"
#include <deque>
#include <atlstr.h>

std::deque<IMathToken*> ShuntingYard(const CString& strEquation)
{
    // Top
    // Push
    // Pop
    
    /* Calculation examples:
    * 5+8*-7
    * 7 * (5 + 8)
    *
    ** First add spaces to simplify parsing: 
    * strEquation.Replace("^", " ^ ");
    * strEquation.Replace("(", " ( ");
    * strEquation.Replace(")", " ) ");
    * strEquation.Replace("*", " * ");
    * strEquation.Replace("/", " / ");
    * strEquation.Replace("+", " + ");
    * strEquation.Replace("-", " - ");
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
    
    std::stack<double>              NumStack;
    std::stack<CMathTokenOperator>  OperatorStack;
    
    // Step 1: Remove spaces
    strEquation.Remove(' ');
    // Step 2: Tokenize
    const CString strSeperators = _T("+-*/()^");
    int iPosition = 0;
    CString strToken;

    strToken = strEquation.Tokenize(strSeperator, iPosition);
    while (!strToken.IsEmpty())
    {
        // Process the token = value
        const double dValue = std::stod(strToken);
        NumStack.push(dValue);
        
        // Check the seperator = operator
        char cOperator = strSeperator[iPosition-1];
        switch(cOperator)
        {
            case '^':
                // Process power
                break;
            case ')':
                break;
            case '(':
                break;
            case '*':
                break;
            case '/':
                break;
            case '+':
                break;
            case '-':
                break;
            default:
                assert(false);
                break;
        }
        
        // Get next token.
        strToken = strEquation.Tokenize(strSeperator, iPosition);

        // Process token into IMathToken
    }

    // TODO
    std::deque<IMathToken*> dq;
    return dq;
}
