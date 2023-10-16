#pragma once

#include "CEntity.h"

class CObject;

class CScene
	: public CEntity
{
private:
	vector<CObject*>	m_arrLayer[MAX_LAYER];

	UINT				m_iTileCol;
	UINT				m_iTileRow;


public:
	void AddObject(CObject* _pObject, LAYER_TYPE _eLayer) { m_arrLayer[(UINT)_eLayer].push_back(_pObject); }
	const vector<CObject*>& GetObjects(LAYER_TYPE _eType) { return m_arrLayer[(UINT)_eType]; }

	UINT GetTileCol(){return m_iTileCol;}
	UINT GetTileRow(){return m_iTileRow;}

	void SetFocusedUI(CObject* _pUI);
 
public:
	virtual void tick();

	// Scene 진입 시 호출
	virtual void enter() = 0;

	// Scene 나올 때 호출
	virtual void exit() = 0;
	
	virtual void render(HDC _dc);


protected:
	void DeleteAllObject();
	void DeleteObject(LAYER_TYPE _eType);

public:
	CScene();
	virtual ~CScene();

	friend class CEventMgr;
	friend class CRuleEventMgr;
};

