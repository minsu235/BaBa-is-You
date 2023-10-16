#pragma once

// State Pattern

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene*		m_arrScene[SCENE_TYPE::END];
	CScene*		m_pCurScene;
	bool		m_bGrid;
	Vec2		m_vCursor;

public:
	CScene* GetCurScene() { return m_pCurScene; }
	
private:
	void ChangeScene(SCENE_TYPE _eNext);
	void ChangeScene(wstring _str);
	void Restart();

public:
	void GridChange() { if (m_bGrid) m_bGrid = false; else m_bGrid = true; }
	bool getGrid() { return m_bGrid; }
	void setCursor(Vec2 _vec2) { m_vCursor = _vec2; }
	Vec2 getCursor() { return m_vCursor; }

public:
	void init();

	void tick();
	void render(HDC _dc);


	friend class CEventMgr;
};

