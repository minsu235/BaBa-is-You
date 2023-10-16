#pragma once
#include "CObject.h"
class CWinTwinkle :
    public CObject
{
private:
    float   m_fMoveTime;
    Vec2    m_vDir;

public:
    virtual void tick() override;

public:
    CLONE(CWinTwinkle);

public:
    CWinTwinkle(Vec2 _location);
    ~CWinTwinkle();
};

