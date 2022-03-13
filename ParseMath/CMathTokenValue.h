#pragma once
#include "IMathToken.h"
class CMathTokenValue : public IMathToken
{
public:
    CMathTokenValue(const double dValue)
        :m_dValue(dValue)
    {

    }

    // From IMathToken:
    virtual bool IsOperator() const override { return false; }


    const double GetValue() const { return m_dValue;  }

    void SetValue(const double dNewValue) { m_dValue = dNewValue; }

private:
    double m_dValue;
};

