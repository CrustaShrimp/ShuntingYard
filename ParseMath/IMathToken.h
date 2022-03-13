#pragma once
class IMathToken
{
public:
    virtual ~IMathToken()
    {

    }

    virtual bool IsOperator() const = 0;
    bool         IsValue()    const { return !IsOperator(); }

};

