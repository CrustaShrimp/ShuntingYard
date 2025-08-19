#pragma once
#include <cmath>
#include <expected>
#include <iostream>
#include <ostream>
#include <string>

enum struct ErrorType {
    DivideByZero,
    InvalidOperation,
    InvalidNumeric,
};

class CMathTokenOperator
{
public:
    static std::string GetSupportedTokens() { return "()^*/+-";}

    explicit CMathTokenOperator(const char Token)
        :m_eOperatorType(EMathOperatorType::MOT_LAST)
        ,m_iOperatorPrecedence(-1)
        ,m_eOperatorAssociativity(EMathOperatorAssociativity::MOA_LEFT)
    {
        switch (Token)
        {
        case '(':
            m_eOperatorType = EMathOperatorType::MOT_BRACE_OPEN;
            m_iOperatorPrecedence = 0;
            break;
        case ')':
            m_eOperatorType = EMathOperatorType::MOT_BRACE_CLOSE;
            m_iOperatorPrecedence = 0;
            break;
        case '^':
            m_eOperatorType = EMathOperatorType::MOT_EXPONENT;
            m_iOperatorPrecedence = 3;
            m_eOperatorAssociativity = EMathOperatorAssociativity::MOA_RIGHT;
            break;
        case '*':
            m_eOperatorType = EMathOperatorType::MOT_MULTIPLY;
            m_iOperatorPrecedence = 2;
            break;
        case '/':
            m_eOperatorType = EMathOperatorType::MOT_DIVIDE;
            m_iOperatorPrecedence = 2;
            break;
        case '+':
            m_eOperatorType = EMathOperatorType::MOT_ADD;
            m_iOperatorPrecedence = 1;
            break;
        case '-':
            m_eOperatorType = EMathOperatorType::MOT_SUBTRACT;
            m_iOperatorPrecedence = 1;
            break;
        default:
            std::wcout << L"CMathTokenOperator: Invalid operator.\n";
            break;
        }
    };

    inline friend bool operator< (const CMathTokenOperator& lhs, const CMathTokenOperator& rhs)
    { 
        if (lhs.m_iOperatorPrecedence == rhs.m_iOperatorPrecedence)
        {
            return rhs.m_eOperatorAssociativity == EMathOperatorAssociativity::MOA_LEFT;
        }
        return lhs.m_iOperatorPrecedence < rhs.m_iOperatorPrecedence;
    }

    [[nodiscard]] std::expected<double, ErrorType> ProcessOperator(const double dFirst, const double dSecond) const
    {
        switch (m_eOperatorType)
        {
        case EMathOperatorType::MOT_EXPONENT:
            return std::pow(dFirst, dSecond);
        case EMathOperatorType::MOT_BRACE_OPEN:
        case EMathOperatorType::MOT_BRACE_CLOSE:
            break;
        case EMathOperatorType::MOT_MULTIPLY:
            return dFirst * dSecond;
        case EMathOperatorType::MOT_DIVIDE: {
            if (dSecond == 0.0)
            {
                return std::unexpected(ErrorType::DivideByZero);

            }
            return dFirst / dSecond;
        }
        case EMathOperatorType::MOT_ADD:
            return dFirst + dSecond;
        case EMathOperatorType::MOT_SUBTRACT:
            return dFirst - dSecond;
        default:
            return std::unexpected(ErrorType::InvalidOperation);
        }

        return std::unexpected(ErrorType::InvalidOperation);;
    }

    [[nodiscard]] bool IsOpenBrace() const
    {
        return m_eOperatorType == EMathOperatorType::MOT_BRACE_OPEN;
    }

    [[nodiscard]] bool IsCloseBrace() const
    {
        return m_eOperatorType == EMathOperatorType::MOT_BRACE_CLOSE;
    }

    [[nodiscard]] bool IsMinus() const
    {
        return m_eOperatorType == EMathOperatorType::MOT_SUBTRACT;
    }


    [[nodiscard]] std::string GetStr() const
    {
        switch (m_eOperatorType)
        {
        case CMathTokenOperator::EMathOperatorType::MOT_EXPONENT:
            return {'^'};
        case CMathTokenOperator::EMathOperatorType::MOT_BRACE_OPEN:
            return {'('};
        case CMathTokenOperator::EMathOperatorType::MOT_BRACE_CLOSE:
            return {')'};
        case CMathTokenOperator::EMathOperatorType::MOT_MULTIPLY:
            return {'*'};
        case CMathTokenOperator::EMathOperatorType::MOT_DIVIDE:
            return {'/'};
        case CMathTokenOperator::EMathOperatorType::MOT_ADD:
            return {'+'};
        case CMathTokenOperator::EMathOperatorType::MOT_SUBTRACT:
            return {'-'};
        default:
            return ("Undefined Operator");
        }
    }

private:
    enum class EMathOperatorType
    {
        MOT_EXPONENT,
        MOT_BRACE_OPEN,
        MOT_BRACE_CLOSE,
        MOT_MULTIPLY,
        MOT_DIVIDE,
        MOT_ADD,
        MOT_SUBTRACT,
        MOT_LAST
    };

    enum class EMathOperatorAssociativity
    {
        MOA_LEFT,
        MOA_RIGHT
    };

    EMathOperatorType           m_eOperatorType;
    int                         m_iOperatorPrecedence;
    EMathOperatorAssociativity  m_eOperatorAssociativity;
};

