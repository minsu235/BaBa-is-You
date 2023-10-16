#pragma once
#include "CObject.h"

class CUI :
    public CObject
{
private:
    vector<CUI*>    m_vecChildUI;
    CUI*            m_pParentUI;

    Vec2            m_vFinalPos;

    bool            m_bMouseOn;
    bool            m_bLbtnPressed;

public:
    CUI* GetParentUI() { return m_pParentUI; }
    CUI* GetAncestorUI();
    void AddChildUI(CUI* _pChildUI);
    bool IsMouseOn() { return m_bMouseOn; }
    Vec2 GetFinalPos() { return m_vFinalPos; }
    bool IsLBtnDown() { return m_bLbtnPressed; }
    
public:
    virtual void tick();
    virtual void render(HDC _dc);

    virtual void OnMouse() {}
    virtual void LBtnDown() { m_bLbtnPressed = true; }
    virtual void LBtnClicked() {}

private:
    void CheckMouseOn();

public:
    virtual CUI* Clone() = 0;

public:
    CUI();
    CUI(const CUI& _ui);
    ~CUI();

    friend class CUIMgr;
};

