#pragma once
#include "CObject.h"
class CWinTitle :
    public CObject
{
public:
    float   m_fMoveTime;
    int     m_iCount;

public:
    virtual void tick() override;

public:
    CLONE(CWinTitle);

public:
    CWinTitle();
    ~CWinTitle();
};

