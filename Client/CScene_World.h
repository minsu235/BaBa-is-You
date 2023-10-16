#pragma once
#include "CScene.h"

class CMenu_anim;

class CScene_World :
    public CScene
{
private:
	float			m_fAccTime;
	bool			m_bFade;
	float			m_fFadetime;

	CMenu_anim*		m_Cursor;
	Vec2			m_CursorLocation;
	Vec2			m_CursorMatrix;
	DIR				m_eMoveDir;
	float			m_fMoveTime;

	Scenestate		m_eScenestate;
	CMenu_anim*		m_Bababutton;
	int				m_iPoint;

public:
	bool CursorMove(DIR _dir);
	
public:
	virtual void enter() override;
	virtual void tick() override;
	virtual void exit() override;

public:
	CScene_World();
	~CScene_World();
};

