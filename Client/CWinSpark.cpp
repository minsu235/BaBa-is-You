#include "pch.h"
#include "CWinSpark.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

CWinSpark::CWinSpark(Vec2 _location)
	: m_fMoveTime(0)
	, m_iCount(0)
{
	random_device rnd;
	m_vDir = Vec2((float)cos((double)rnd()), (float)sin((double)rnd()));
	m_vDir.Normalize();
	
	Vec2 vPos = _location;
	vPos += Vec2(6.f, 6.f);
	SetPos(vPos);

	AddComponent(new CAnimator);
	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(L"WinCircleAtlas", L"image\\Effect\\WinCircle.bmp");
	GetAnimator()->CreateAnimation(L"WinCircle", pObjAtlas, Vec2(0.f, 0.f), Vec2(20.f, 20.f), Vec2(10.f, 10.f), 20.f, 6, 0.1f);
	CImage* pObjAtlas2 = CResMgr::GetInst()->LoadImg(L"WinSparkAtlas", L"image\\Effect\\WinSpark.bmp");
	GetAnimator()->CreateAnimation(L"WinSpark", pObjAtlas2, Vec2(0.f, 0.f), Vec2(20.f, 20.f), Vec2(10.f, 10.f), 20.f, 5, 0.1f);
	GetAnimator()->Play(L"WinCircle", true);
}

CWinSpark::~CWinSpark()
{
}

void CWinSpark::tick()
{
	if (m_iCount == 0)
	{
		Vec2 vPos = GetPos();
		vPos.x += 60 * m_vDir.x * DT;
		vPos.y += 60 * m_vDir.y * DT;
		SetPos(vPos);
		m_fMoveTime += DT;

		if (m_fMoveTime > 1.f)
		{
			++m_iCount;
			GetAnimator()->Play(L"WinSpark", false);
			m_fMoveTime = 0;
		}
	}
	else if (m_iCount == 1)
	{
		m_fMoveTime += DT;
		if (m_fMoveTime > 0.6f)
		{
			tEventInfo info = {};
			info.eType = EVENT_TYPE::DELETE_OBJECT;
			info.first = (DWORD_PTR)this;
			CEventMgr::GetInst()->AddEvent(info);
		}
	}

	CObject::tick();
}
