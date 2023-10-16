#pragma once
#include "CObject.h"
#include "CThing.h"

class CMoveEffect :
    public CObject
{
private:
    DIR m_eMoveDir;
    float m_fMoveTime;

public:
    virtual void tick() override;

public:
    virtual CMoveEffect* Clone() { return new CMoveEffect(*this); }

public:
    CMoveEffect(Thingtype _thingtype, DIR _dir, Vec2 _location);
    ~CMoveEffect();
};

