#pragma once
#include "CScene.h"

class CMenu_anim;
class CToolPanel;

enum class BGMtype
{
	BABA,
	BURN,
	CAVE,
	CRYSTAL,
	FACTORY,
	FLOAT,
	FOREST,
	GARDEN,
	MOUNTAIN,
	RAIN,
	RUIN,
	SPACE,
	STARS,
	END
};

const wstring BGM_arr[(UINT)BGMtype::END]
=
{
	L"baba",
	L"burn",
	L"cave",
	L"crystal",
	L"factory",
	L"float",
	L"forest",
	L"garden",
	L"mountain",
	L"rain",
	L"ruin",
	L"space",
	L"stars",
};

class CScene_Tool :
    public CScene
{
private:
	bool			m_bFade;
	float			m_fFadetime;
	CToolPanel*		m_panel;

	CMenu_anim*		m_Bababutton;
	int				m_iPoint;
	Scenestate		m_eScenestate;

	Vec2			m_vSize;
	BGMtype			m_BGM;

public:
	void setSize(Vec2 _vec) { m_vSize = _vec; }
	Vec2 getSize() { return m_vSize; }
	CToolPanel* getToolpanel() { return m_panel; }
	void MapSave();
	void MapLoad();

public:
	virtual void enter() override;
	virtual void tick() override;
	virtual void exit() override;
	virtual void render(HDC _dc) override;

public:
	CScene_Tool();
	~CScene_Tool();

};

