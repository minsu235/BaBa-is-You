#include "pch.h"
#include "CSceneMgr.h"

#include "CScene_Stage.h"
#include "CScene_Logo.h"
#include "CScene_World.h"
#include "CScene_Tool.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
	, m_bGrid(false)
{	
}

CSceneMgr::~CSceneMgr()
{
	for (int i = 0; i < SCENE_TYPE::END; ++i)
	{
		if(nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneMgr::init()
{
	m_arrScene[TOOL] = new CScene_Tool;
	m_arrScene[STAGE] = new CScene_Stage;
	m_arrScene[LOGO] = new CScene_Logo;
	m_arrScene[WORLD] = new CScene_World;

	ChangeScene(SCENE_TYPE::LOGO);
}

void CSceneMgr::tick()
{
	m_pCurScene->tick();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	if (nullptr != m_pCurScene)
	{
		m_pCurScene->exit();
	}
	else if (m_pCurScene == m_arrScene[STAGE])
	{
		m_pCurScene->exit();
		delete m_arrScene[STAGE];
		m_arrScene[STAGE] = nullptr;
	}

	m_pCurScene = m_arrScene[_eNext];

	m_pCurScene->enter();
}

void CSceneMgr::ChangeScene(wstring _str)
{
	if (nullptr != m_pCurScene)
	{
		m_pCurScene->exit();
	}
	delete m_arrScene[STAGE];
	m_arrScene[STAGE] = new CScene_Stage;
	CScene_Stage* stage = (CScene_Stage*)m_arrScene[STAGE];
	m_pCurScene = m_arrScene[STAGE];

	stage->enter(_str);
}

void CSceneMgr::Restart()
{
	CScene_Stage* stage = (CScene_Stage*)m_arrScene[STAGE];
	wstring name = stage->getMapname();
	m_pCurScene->exit();
	delete m_arrScene[STAGE];
	m_arrScene[STAGE] = new CScene_Stage;
	m_pCurScene = m_arrScene[STAGE];
	stage = (CScene_Stage*)m_arrScene[STAGE];
	stage->enter(name);
}
