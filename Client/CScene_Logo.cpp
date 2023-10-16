#include "pch.h"
#include "CScene_Logo.h"

#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "CMenu.h"
#include "CMenu_anim.h"
#include "CSound.h"

CScene_Logo::CScene_Logo()
	: m_pBababutton(nullptr)
	, m_pBack(nullptr)
	, m_iPoint(1)
	, m_bChange(false)
	, m_fAccTime(0)
	, m_bFade(true)
	, m_fFadetime(0)
{
}

CScene_Logo::~CScene_Logo()
{
}

void CScene_Logo::enter()
{
	CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_menu");
	pBGM->PlayToBGM(true);

	m_pBababutton = nullptr;
	m_pBack = nullptr;
	m_iPoint = 1;
	m_bChange = false;
	m_fAccTime = 0;
	m_bFade = true;
	m_fFadetime = 0;

	Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();

	CMenu* pBack = new CMenu(L"StartBackground");
	pBack->SetPos(Vec2(1280, 768));
	AddObject(pBack, LAYER_TYPE::BACKGROUND);
	m_pBack = pBack;

	CMenu* pStart = new CMenu(L"StartButton");
	pStart->SetPos(Vec2(res.x / 2, res.y / 2 + 75));
	AddObject(pStart, LAYER_TYPE::MENU);

	CMenu* pTool = new CMenu(L"EditorButton");
	pTool->SetPos(Vec2(res.x / 2, res.y / 2 + 120));
	AddObject(pTool, LAYER_TYPE::MENU);

	CMenu* pExit = new CMenu(L"ExitButton");
	pExit->SetPos(Vec2(res.x / 2, res.y / 2 + 165));
	AddObject(pExit, LAYER_TYPE::MENU);

	CMenu_anim* pBabaButton = new CMenu_anim(L"BabaButton");
	pBabaButton->SetPos(Vec2(res.x / 2 - 170, res.y / 2 + 60));
	AddObject(pBabaButton, LAYER_TYPE::MENU);
	m_pBababutton = pBabaButton;

	CMenu_anim* pTitle = new CMenu_anim(L"BabaTitle");
	pTitle->SetPos(Vec2(res.x / 2 - 289, res.y / 2 - 250));
	AddObject(pTitle, LAYER_TYPE::MENU);
}

void CScene_Logo::tick()
{
	Vec2 vbackPos = m_pBack->GetPos();
	vbackPos -= Vec2(64 * DT, 30 * DT);
	if (vbackPos.x < 0)
		vbackPos.x = 1280;
	if (vbackPos.y < 0)
		vbackPos.y = 768;
	m_pBack->SetPos(vbackPos);

	if (m_bFade)
	{
		if (m_fFadetime == 0)
		{
			CMenu_anim* Fadein = new CMenu_anim(L"Fadein");
			Fadein->SetPos(Vec2(0, 0));
			AddObject(Fadein, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fFadetime > 0.8)
		{
			m_bFade = false;
			DeleteObject(LAYER_TYPE::FADEINOUT);
		}
		m_fFadetime += DT;
		CScene::tick();
		return;
	}

	if (!m_bChange)
	{
		Vec2 vPos = m_pBababutton->GetPos();

		if (IsTap(KEY_TYPE::UP))
		{
			if (m_iPoint != 1)
			{
				--m_iPoint;
				vPos.y -= 45;

				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
			}
		}

		if (IsTap(KEY_TYPE::DOWN))
		{
			if (m_iPoint != 3)
			{
				++m_iPoint;
				vPos.y += 45;

				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
			}
		}

		if (IsTap(KEY_TYPE::ENTER))
		{
			if (m_iPoint == 1)
			{
				m_bChange = true;
			}
			else if (m_iPoint == 2)
			{
				m_bChange = true;
			}
			else if (m_iPoint == 3)
			{
				HWND mainhwnd = CEngine::GetInst()->GetMainHwnd();
				SendMessage(mainhwnd, WM_CLOSE, 0, 0);
			}
		}

		m_pBababutton->SetPos(vPos);
	}
	else if (m_bChange)
	{
		if (m_fAccTime == 0)
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_menuselect");
			pBGM->PlayToBGM();
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fAccTime > 2)
		{
			if (m_iPoint == 2)
				SceneChange(SCENE_TYPE::TOOL);
			else if (m_iPoint == 1)
				SceneChange(SCENE_TYPE::WORLD);
		}
		m_fAccTime += DT;
	}

	CScene::tick();
}

void CScene_Logo::exit()
{
	DeleteAllObject();
}
