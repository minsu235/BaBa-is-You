#pragma once
#include "CScene.h"

class CMenu_anim;
class CMenu;

class CScene_Logo :
    public CScene
{
private:
    CMenu_anim* m_pBababutton;
    CMenu*      m_pBack;
    int         m_iPoint;
    bool        m_bChange;
    float       m_fAccTime;
    bool	    m_bFade;
    float		m_fFadetime;

public:
    virtual void tick() override;

    virtual void enter() override;
    virtual void exit() override;

public:
    CScene_Logo();
    ~CScene_Logo();
};

