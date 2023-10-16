#pragma once
#include "CObject.h"
class CSpark :
    public CObject
{
public:
    float   m_fMoveTime;

public:
    virtual void tick() override;

public:
    CLONE(CSpark);

public:
    CSpark(wstring _string ,Vec2 _location);
    ~CSpark();
};

