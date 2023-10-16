#include "pch.h"
#include "CScene_World.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CResMgr.h"

#include "CMenu.h"
#include "CMenu_anim.h"
#include "CThing.h"
#include "CSound.h"

CScene_World::CScene_World()
	: m_fAccTime(0)
	, m_bFade(true)
	, m_fFadetime(0)
	, m_Cursor(nullptr)
	, m_eMoveDir(DIR::NONE)
	, m_fMoveTime(0)
	, m_eScenestate(Scenestate::PLAY)
	, m_Bababutton(nullptr)
	, m_iPoint(1)
{
}

CScene_World::~CScene_World()
{
}

void CScene_World::enter()
{
	CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_map");
	pBGM->PlayToBGM(true);

	m_fAccTime = 0;
	m_bFade = true;
	m_fFadetime = 0;
	m_Cursor = nullptr;
	m_eMoveDir = DIR::NONE;
	m_fMoveTime = 0;
	m_eScenestate = Scenestate::PLAY;
	m_Bababutton = nullptr;
	m_iPoint = 1;
	m_CursorMatrix = CSceneMgr::GetInst()->getCursor();

	Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();

	CMenu_anim* pBack = new CMenu_anim(L"WorldMap");
	pBack->SetPos(Vec2(res.x / 2 - 528, res.y / 2 - 288));
	AddObject(pBack, LAYER_TYPE::BACKGROUND);

	CMenu_anim* pCursor = new CMenu_anim(L"Cursor");
	pCursor->SetPos(Vec2(res.x / 2 - 528 + 286 + m_CursorMatrix.x * TILE_SIZE, res.y / 2 - 288 + 478 - m_CursorMatrix.y * TILE_SIZE));
	m_CursorLocation = Vec2(res.x / 2 - 528 + 286 + m_CursorMatrix.x * TILE_SIZE, res.y / 2 - 288 + 478 - m_CursorMatrix.y * TILE_SIZE);

	AddObject(pCursor, LAYER_TYPE::OBJECT);
	m_Cursor = pCursor;

	CThing* pThing = new CThing(Text::BABA);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 96, res.y / 2 + 288 - 96));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 96, res.y / 2 + 288 - 96));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);

	pThing = new CThing(Text::IS);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 64, res.y / 2 + 288 - 96));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 64, res.y / 2 + 288 - 96));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);

	pThing = new CThing(Text::YOU);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 32, res.y / 2 + 288 - 96));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 32, res.y / 2 + 288 - 96));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);

	pThing = new CThing(Text::FLAG);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 96, res.y / 2 + 288 - 32));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 96, res.y / 2 + 288 - 32));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);

	pThing = new CThing(Text::IS);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 64, res.y / 2 + 288 - 32));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 64, res.y / 2 + 288 - 32));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);

	pThing = new CThing(Text::WIN);
	pThing->SetPos(Vec2(res.x / 2 + 528 - 32, res.y / 2 + 288 - 32));
	pThing->setOriginPos(Vec2(res.x / 2 + 528 - 32, res.y / 2 + 288 - 32));
	pThing->setThingtype(Thingtype::TEXT);
	pThing->on();
	AddObject(pThing, LAYER_TYPE::OBJECT);
}

void CScene_World::tick()
{
	if (m_bFade)
	{
		if (m_fFadetime == 0)
		{
			CMenu_anim* Fadein = new CMenu_anim(L"Fadein");
			Fadein->SetPos(Vec2(0, 0));
			AddObject(Fadein, LAYER_TYPE::FADEINOUT);
			m_fFadetime += DT;
		}
		else if (m_fFadetime > 0.8)
		{
			m_fFadetime = 0;
			m_bFade = false;
			DeleteObject(LAYER_TYPE::FADEINOUT);
		}
		else
			m_fFadetime += DT;
		CScene::tick();
		return;
	}

	if (m_eScenestate == Scenestate::PLAY)
	{
		Vec2 vPos = m_Cursor->GetPos();

		if (m_eMoveDir == DIR::NONE)
		{
			if (IsTap(KEY_TYPE::UP))
			{
				if (CursorMove(DIR::UP))
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_eMoveDir = DIR::UP;
				}
			}
			else if (IsTap(KEY_TYPE::DOWN))
			{
				if (CursorMove(DIR::DOWN))
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_eMoveDir = DIR::DOWN;
				}
			}
			else if (IsTap(KEY_TYPE::LEFT))
			{
				if (CursorMove(DIR::LEFT))
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_eMoveDir = DIR::LEFT;
				}
			}
			else if (IsTap(KEY_TYPE::RIGHT))
			{
				if (CursorMove(DIR::RIGHT))
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_eMoveDir = DIR::RIGHT;
				}
			}
			else if (IsTap(KEY_TYPE::ENTER))
			{
				if (m_CursorMatrix == Vec2(0, 0) || m_CursorMatrix == Vec2(1, 2) || m_CursorMatrix == Vec2(1, 3) || m_CursorMatrix == Vec2(1, 4)
					|| m_CursorMatrix == Vec2(2, 2) || m_CursorMatrix == Vec2(2, 3) || m_CursorMatrix == Vec2(2, 4) || m_CursorMatrix == Vec2(3, 3)
					|| m_CursorMatrix == Vec2(6, 2))
					m_eScenestate = Scenestate::CHANGE;
			}
			else if (IsTap(KEY_TYPE::ESC))
			{
				m_eScenestate = Scenestate::ESC;
				CMenu* Back = new CMenu(L"ESCBackground");
				Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
				Back->SetPos(Vec2(res.x / 2, res.y / 2));
				AddObject(Back, LAYER_TYPE::MENU);
				CMenu* resume = new CMenu(L"ESCResume");
				resume->SetPos(Vec2(res.x / 2, res.y / 2 - 230));
				AddObject(resume, LAYER_TYPE::MENU);
				CMenu* returntomenu = new CMenu(L"ESCRmenu");
				returntomenu->SetPos(Vec2(res.x / 2, res.y / 2 - 180));
				AddObject(returntomenu, LAYER_TYPE::MENU);
				CMenu_anim* Bababutton = new CMenu_anim(L"BabaButton");
				Bababutton->SetPos(Vec2(res.x / 2 - 168, res.y / 2 - 246));
				m_Bababutton = Bababutton;
				AddObject(Bababutton, LAYER_TYPE::MENU);
			}
		}

		switch (m_eMoveDir)
		{
		case DIR::UP:
		{
			if (m_fMoveTime < 0.1)
			{
				vPos.y -= 10 * TILE_SIZE * DT;
				if (m_CursorLocation.y - TILE_SIZE > vPos.y)
					vPos.y = m_CursorLocation.y - TILE_SIZE;
				m_fMoveTime += DT;
			}
			else
			{
				if (m_CursorLocation.y - TILE_SIZE < vPos.y)
					vPos.y = m_CursorLocation.y - TILE_SIZE;
				m_eMoveDir = DIR::NONE;
				m_fMoveTime = 0;
				m_CursorLocation = vPos;
			}
		}
		break;
		case DIR::DOWN:
		{
			if (m_fMoveTime < 0.1)
			{
				vPos.y += 10 * TILE_SIZE * DT;
				if (m_CursorLocation.y + TILE_SIZE < vPos.y)
					vPos.y = m_CursorLocation.y + TILE_SIZE;
				m_fMoveTime += DT;
			}
			else
			{
				if (m_CursorLocation.y + TILE_SIZE > vPos.y)
					vPos.y = m_CursorLocation.y + TILE_SIZE;
				m_eMoveDir = DIR::NONE;
				m_fMoveTime = 0;
				m_CursorLocation = vPos;
			}
		}
		break;
		case DIR::LEFT:
		{
			if (m_fMoveTime < 0.1)
			{
				vPos.x -= 10 * TILE_SIZE * DT;
				if (m_CursorLocation.x - TILE_SIZE > vPos.x)
					vPos.x = m_CursorLocation.x - TILE_SIZE;
				m_fMoveTime += DT;
			}
			else
			{
				if (m_CursorLocation.x - TILE_SIZE < vPos.x)
					vPos.x = m_CursorLocation.x - TILE_SIZE;
				m_eMoveDir = DIR::NONE;
				m_fMoveTime = 0;
				m_CursorLocation = vPos;
			}
		}

		break;
		case DIR::RIGHT:
		{
			if (m_fMoveTime < 0.1)
			{
				vPos.x += 10 * TILE_SIZE * DT;
				if (m_CursorLocation.x + TILE_SIZE < vPos.x)
					vPos.x = m_CursorLocation.x + TILE_SIZE;
				m_fMoveTime += DT;
			}
			else
			{
				if (m_CursorLocation.x + TILE_SIZE > vPos.x)
					vPos.x = m_CursorLocation.x + TILE_SIZE;
				m_eMoveDir = DIR::NONE;
				m_fMoveTime = 0;
				m_CursorLocation = vPos;
			}
		}
		break;
		}

		m_Cursor->SetPos(vPos);
	}
	else if (m_eScenestate == Scenestate::CHANGE)
	{
		if (m_fAccTime == 0)
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_stage");
			pBGM->PlayToBGM();
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fAccTime > 2)
		{
			if (m_CursorMatrix == Vec2(0, 0) || m_CursorMatrix == Vec2(1, 2) || m_CursorMatrix == Vec2(1, 3) || m_CursorMatrix == Vec2(1, 4)
				|| m_CursorMatrix == Vec2(2, 2) || m_CursorMatrix == Vec2(2, 3) || m_CursorMatrix == Vec2(2, 4) || m_CursorMatrix == Vec2(3, 3)
				|| m_CursorMatrix == Vec2(6, 2))

			if (m_CursorMatrix == Vec2(0, 0))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 0);
			}
			else if (m_CursorMatrix == Vec2(1, 2))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 1);
			}
			else if (m_CursorMatrix == Vec2(1, 3))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 2);
			}
			else if (m_CursorMatrix == Vec2(2, 2))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 3);
			}
			else if (m_CursorMatrix == Vec2(2, 3))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 4);
			}
			else if (m_CursorMatrix == Vec2(1, 4))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 5);
			}
			else if (m_CursorMatrix == Vec2(3, 3))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 6);
			}
			else if (m_CursorMatrix == Vec2(2, 4))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 7);
			}
			else if (m_CursorMatrix == Vec2(6, 2))
			{
				CSceneMgr::GetInst()->setCursor(m_CursorMatrix);
				SceneChange(STAGE, 8);
			}
		}
		m_fAccTime += DT;
	}
	else if (m_eScenestate == Scenestate::ESC)
	{
		if (IsTap(KEY_TYPE::UP))
		{
			if (m_iPoint != 1)
			{
				--m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos -= Vec2(0, 50);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::DOWN))
		{
			if (m_iPoint != 2)
			{
				++m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos += Vec2(0, 50);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::ENTER))
		{
			if (m_iPoint == 1)
			{
				m_Bababutton = nullptr;
				m_eScenestate = Scenestate::PLAY;
				DeleteObject(LAYER_TYPE::MENU);
			}
			else if (m_iPoint == 2)
				m_eScenestate = Scenestate::MENUFADEOUT;
		}
		else if (IsTap(KEY_TYPE::ESC))
		{
			m_Bababutton = nullptr;
			m_eScenestate = Scenestate::PLAY;
			DeleteObject(LAYER_TYPE::MENU);
			m_iPoint = 1;
		}
	}
	else if (m_eScenestate == Scenestate::MENUFADEOUT)
	{
		if (m_fFadetime == 0)
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_menuselect");
			pBGM->PlayToBGM();
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fFadetime > 2)
			SceneChange(LOGO);

		m_fFadetime += DT;
	}

	CScene::tick();
}

void CScene_World::exit()
{
	DeleteAllObject();
}

bool CScene_World::CursorMove(DIR _dir)
{
	Vec2 vMatrix = m_CursorMatrix;

	switch (_dir)
	{
	case DIR::UP:
		vMatrix.y += 1;
		break;
	case DIR::DOWN:
		vMatrix.y -= 1;
		break;
	case DIR::LEFT:
		vMatrix.x -= 1;
		break;
	case DIR::RIGHT:
		vMatrix.x += 1;
		break;
	}

	if (
		vMatrix == Vec2(0, 0)
		|| (vMatrix.x == 1 && (vMatrix.y == 0 || vMatrix.y == 1 || vMatrix.y == 2 || vMatrix.y == 3 || vMatrix.y == 4))
		|| (vMatrix.x == 2 && (vMatrix.y == 2 || vMatrix.y == 3 || vMatrix.y == 4))
		|| (vMatrix.x == 3 && (vMatrix.y == 2 || vMatrix.y == 3 || vMatrix.y == 4))
		|| (vMatrix.x == 4 && (vMatrix.y == 2 || vMatrix.y == 4))
		|| (vMatrix.x == 5 && (vMatrix.y == 2 || vMatrix.y == 4))
		|| (vMatrix.x == 6 && (vMatrix.y == 2 || vMatrix.y == 4 || vMatrix.y == 5 || vMatrix.y == 6))
		)
	{
		m_CursorMatrix = vMatrix;
		return true;
	}
	else
		return false;
}