#include "pch.h"
#include "CWinTitle.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

CWinTitle::CWinTitle()
	: m_fMoveTime(0)
	, m_iCount(0)
{
	AddComponent(new CAnimator);
	CImage* pObjAtlas1 = CResMgr::GetInst()->LoadImg(L"WinUI_CREATE", L"image\\UI\\WinUI_CREATE.bmp");
	CImage* pObjAtlas2 = CResMgr::GetInst()->LoadImg(L"WinUI_COM", L"image\\UI\\WinUI_COM.bmp");
	GetAnimator()->CreateAnimation(L"WinUI_CREATE", pObjAtlas1, Vec2(0.f, 0.f), Vec2(452.f, 154.f), Vec2(0.f, 0.f), 154.f, 19, 0.05f);
	GetAnimator()->CreateAnimation(L"WinUI_COM", pObjAtlas2, Vec2(0.f, 0.f), Vec2(452.f, 154.f), Vec2(0.f, 0.f), 154.f, 3, 0.1f);
	GetAnimator()->Play(L"WinUI_CREATE", false);
}

CWinTitle::~CWinTitle()
{
}

void CWinTitle::tick()
{
	if (m_iCount == 0)
	{
		if (m_fMoveTime > 0.95 && m_iCount == 0)
		{
			GetAnimator()->Play(L"WinUI_COM", true);
			++m_iCount;
		}

		m_fMoveTime += DT;
	}

	CObject::tick();
}
