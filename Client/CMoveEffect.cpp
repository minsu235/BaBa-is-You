#include "pch.h"
#include "CMoveEffect.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

#include "CThing.h"

CMoveEffect::CMoveEffect(Thingtype _thingtype, DIR _dir, Vec2 _location)
	: m_eMoveDir(_dir)
	, m_fMoveTime(0.f)
{
	Vec2 vPos = _location;
	vPos += Vec2(17.f, 24.f);
	SetPos(vPos);
	AddComponent(new CAnimator);
	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(Thing_arr[(UINT)_thingtype] + L"DustAtlas", L"image\\Effect\\" + Thing_arr[(UINT)_thingtype] + L"Dust.bmp");
	GetAnimator()->CreateAnimation(L"dust", pObjAtlas, Vec2(0.f, 0.f), Vec2(13.f, 13.f), Vec2(0.f, 0.f), 13.f, 9, 0.08f);
	GetAnimator()->Play(L"dust", false);
}

CMoveEffect::~CMoveEffect()
{
}

void CMoveEffect::tick()
{
	Vec2 vPos = GetPos();

	switch (m_eMoveDir)
	{
	case DIR::UP:
		vPos.y += 40 * DT;
		break;
	case DIR::DOWN:
		vPos.y -= 40 * DT;
		break;
	case DIR::LEFT:
		vPos.x += 40 * DT;
		break;
	case DIR::RIGHT:
		vPos.x -= 40 * DT;
		break;
	}

	SetPos(vPos);
	if (m_fMoveTime > 0.8f)
	{
		tEventInfo info = {};
		info.eType = EVENT_TYPE::DELETE_OBJECT;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}

	m_fMoveTime += DT;

	CObject::tick();
}