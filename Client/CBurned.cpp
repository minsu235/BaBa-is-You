#include "pch.h"
#include "CBurned.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

CBurned::CBurned(Vec2 _location)
	: m_fMoveTime(0)
{
	Vec2 vPos = _location;
	vPos += Vec2(16.f, 16.f);
	SetPos(vPos);

	AddComponent(new CAnimator);
	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(L"Burned", L"image\\Effect\\Burned.bmp");
	GetAnimator()->CreateAnimation(L"Burned", pObjAtlas, Vec2(0.f, 0.f), Vec2(30.f, 30.f), Vec2(0.f, 0.f), 30.f, 10, 0.06f);
	GetAnimator()->Play(L"Burned", false);
}

CBurned::~CBurned()
{
}

void CBurned::tick()
{
	Vec2 vPos = GetPos();
	vPos.y -= 40 * DT;
	SetPos(vPos);

	m_fMoveTime += DT;

	if (m_fMoveTime > 0.6f)
	{
		tEventInfo info = {};
		info.eType = EVENT_TYPE::DELETE_OBJECT;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}

	CObject::tick();
}