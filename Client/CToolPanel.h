#pragma once
#include "CUI.h"
#include "CThing.h"

class CImage;

struct cursorInfo
{
    Thingtype _thingtype;
    Text      _text;

    cursorInfo(Thingtype _t, Text _text = Text::END)
        : _thingtype(_t)
        , _text(_text)
    {}
};

class CToolPanel :
    public CUI
{
private:
    Vec2        m_vCapturePos;
    CImage*     m_pImage;
    CImage*     m_pImage2;
    Vec2        m_cursor;
    cursorInfo  m_Info;
    DIR         m_dir;

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

    cursorInfo getInfo() { return m_Info; }
    DIR getDIR() { return m_dir; }

public:
    CLONE(CToolPanel);

    CToolPanel();
    ~CToolPanel();
    
};

