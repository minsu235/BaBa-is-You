#pragma once
#include "CObject.h"

class CWinSpark :
    public CObject
{
public:
    float   m_fMoveTime;
    int     m_iCount;
    Vec2    m_vDir;

public:
    virtual void tick() override;

public:
    CLONE(CWinSpark);

public:
    CWinSpark(Vec2 _location);
    ~CWinSpark();
};

