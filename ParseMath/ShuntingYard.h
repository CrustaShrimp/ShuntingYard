#pragma once
#include "IMathToken.h"
#include <deque>
#include <atlstr.h>

std::deque<IMathToken*> ShuntingYard(const CString& strEquation)
{
    // Top
    // Push
    // Pop
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
        
        // Process the token = value
        const double dValue = std::stod(strToken);
        NumStack.push(dValue);
        
        // Get next token.
        strToken = strEquation.Tokenize(strSeperator, iPosition);

        // Process token into IMathToken
    }

    // TODO
    std::deque<IMathToken*> dq;
    return dq;
}
