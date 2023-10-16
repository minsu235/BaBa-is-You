#pragma once
#include "CObject.h"
class CMenu_anim :
    public CObject
{
public:
    CLONE(CMenu_anim);

public:
    CMenu_anim(wstring _name);
    ~CMenu_anim();
};

