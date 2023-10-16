#pragma once
#include "CObject.h"

class CTile :
    public CObject
{
private:
    Vec2                    m_vIdx;      // ≈∏¿œ ¿Œµ¶Ω∫

public:
    void setIdx(Vec2 _iIdx) { m_vIdx = _iIdx; }

public:
    virtual void render(HDC _dc) override;

    CLONE(CTile);
public:
    CTile();
    ~CTile();
};

