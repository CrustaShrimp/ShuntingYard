#pragma once
#include "IMathToken.h"
#include <assert.h>

class CMathTokenOperator : public IMathToken
{
public:
    enum class EMathOperatorType
    {
        MOT_EXPONENT,
        MOT_BRACE_OPEN,
        MOT_BRACE_CLOSE,
        MOT_MULTIPLY,
        MOT_DIVIDE,
        MOT_ADD,
        MOT_SUBTRACT
    };

    enum class EMathOperatorAssociativity
    {
        MOA_LEFT,
        MOA_RIGHT
    };

    CMathTokenOperator(EMathOperatorType eOperatorType)
        :m_eOperatorType(eOperatorType)
        ,m_eOperatorAssociativity(eOperatorType == EMathOperatorType::MOT_EXPONENT ? EMathOperatorAssociativity::MOA_RIGHT : EMathOperatorAssociativity::MOA_LEFT)
        ,m_iOperatorPrecedence(-1)
    {
        switch (m_eOperatorType)
        {
        case EMathOperatorType::MOT_EXPONENT:
            m_iOperatorPrecedence = 4;
            break;
        case EMathOperatorType::MOT_BRACE_OPEN:
        case EMathOperatorType::MOT_BRACE_CLOSE:
            m_iOperatorPrecedence = 3;
            break;
        case EMathOperatorType::MOT_MULTIPLY:
        case EMathOperatorType::MOT_DIVIDE:
            m_iOperatorPrecedence = 2;
            break;
        case EMathOperatorType::MOT_ADD:
        case EMathOperatorType::MOT_SUBTRACT:
            m_iOperatorPrecedence = 1;
            break;
        default:
            assert(false);
            break;
        }

    };

    double ProcessOperator(const double dFirst, const double dSecond) const
    {
        switch (m_eOperatorType)
        {
        case EMathOperatorType::MOT_EXPONENT:
            return std::pow(dFirst, dSecond);
            break;
        case EMathOperatorType::MOT_BRACE_OPEN:
        case EMathOperatorType::MOT_BRACE_CLOSE:
            throw std::logic_error("The method or operation is not implemented.");
            break;
        case EMathOperatorType::MOT_MULTIPLY:
            return dFirst * dSecond;
            break;
        case EMathOperatorType::MOT_DIVIDE:
            assert(dSecond != 0.0);
            if (dSecond != 0.0)
            { 
                return dFirst / dSecond;
            }
            else
            {
                return 0.0;
            }
            break;
        case EMathOperatorType::MOT_ADD:
            return dFirst + dSecond;
            break;
        case EMathOperatorType::MOT_SUBTRACT:
            return dFirst - dSecond;
            break;
        default:
            assert(false);
            break;
        }
        return 0.0;
    }

    // From IMathToken:
    virtual bool IsOperator() const override { return true; }

private:
    const EMathOperatorType             m_eOperatorType;
    int                                 m_iOperatorPrecedence;
    const EMathOperatorAssociativity    m_eOperatorAssociativity;
};

