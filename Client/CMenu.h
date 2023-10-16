#pragma once
#include "CObject.h"

class CImage;

class CMenu :
    public CObject
{
private:
    CImage*     m_pImage;
    bool        m_bGrid;
    bool        m_Grid;
    int         m_iTransparency;

public:
    virtual void render(HDC _dc) override;
    virtual void tick() override;

public:
    CLONE(CMenu);

public:
    CMenu(wstring _name);
    ~CMenu();
};

