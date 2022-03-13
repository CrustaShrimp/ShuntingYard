#pragma once
#include "IMathToken.h"
#include <deque>
#include <atlstr.h>

std::deque<IMathToken*> ShuntingYard(const CString& strEquation)
{
    // Step 1: Remove spaces
    strEquation.Remove(' ');
    // Step 2: Tokenize
    const CString Seperator = _T(" ");
    int Position = 0;
    CString Token;

    Token = strEquation.Tokenize(Seperator, Position);
    while (!Token.IsEmpty())
    {
        // Get next token.
        Token = strEquation.Tokenize(Seperator, Position);

        // Process token into IMathToken
    }

    // TODO
    std::deque<IMathToken*> dq;
    return dq;
}