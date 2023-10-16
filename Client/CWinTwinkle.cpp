#include "pch.h"
#include "CWinTwinkle.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

CWinTwinkle::CWinTwinkle(Vec2 _location)
	: m_fMoveTime(0)
{
	random_device rnd;
	m_vDir = Vec2((float)cos((double)rnd()), (float)sin((double)rnd()));
	m_vDir.Normalize();

	Vec2 vPos = _location;
	vPos += Vec2(6.f, 6.f);
	SetPos(vPos);

	AddComponent(new CAnimator);
	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(L"WinTwinkleAtlas", L"image\\Effect\\WinTwinkle.bmp");
	GetAnimator()->CreateAnimation(L"WinTwinkle", pObjAtlas, Vec2(0.f, 0.f), Vec2(20.f, 20.f), Vec2(10.f, 10.f), 20.f, 8, 0.08f);
	GetAnimator()->Play(L"WinTwinkle", false);
}

CWinTwinkle::~CWinTwinkle()
{
}

void CWinTwinkle::tick()
{
	Vec2 vPos = GetPos();
	vPos.x += 40 * m_vDir.x * DT;
	vPos.y += 40 * m_vDir.y * DT;
	SetPos(vPos);

	if (m_fMoveTime > 0.72f)
	{
		tEventInfo info = {};
		info.eType = EVENT_TYPE::DELETE_OBJECT;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}

	m_fMoveTime += DT;

	CObject::tick();
}

