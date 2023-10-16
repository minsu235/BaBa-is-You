#pragma once
#include "CObject.h"
#include "CThing.h"

class CToolTile :
    public CObject
{
private:
    Vec2                    m_vIdx;      // ≈∏¿œ ¿Œµ¶Ω∫
    Thingtype               m_eThingtype;
    Text                    m_eText;
    DIR                     m_eDir;
    bool                    m_bMouseOn;

public:
    void setIdx(Vec2 _iIdx) { m_vIdx = _iIdx; }
    void setThingtype(Thingtype _type) { m_eThingtype = _type; }
    void setText(Text _text) { m_eText = _text; }
    void setDIR(DIR _dir) { m_eDir = _dir; }
    Vec2 getIdx() { return m_vIdx; }
    Thingtype getThingtype() { return m_eThingtype; }
    Text getText() { return m_eText; }
    DIR getDIR() { return m_eDir; }

private:
    void CheckMouseOn();

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

    CLONE(CToolTile);
public:
    CToolTile();
    ~CToolTile();
};

