#include "pch.h"
#include "CSpark.h"

#include "CAnimator.h"
#include "CResMgr.h"
#include "CTimeMgr.h"

CSpark::CSpark(wstring _string, Vec2 _location)
	: m_fMoveTime(0)
{
	random_device rnd;
	random_device rnd2;
	Vec2 vPos = _location;
	vPos += Vec2((float)(rnd() % 32), (float)(rnd2() % 32));
	SetPos(vPos);

	AddComponent(new CAnimator);
	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(_string + L"SparkAtlas", L"image\\Effect\\" + _string + L"Spark.bmp");
	GetAnimator()->CreateAnimation(L"Spark", pObjAtlas, Vec2(0.f, 0.f), Vec2(20.f, 20.f), Vec2(0.f, 0.f), 20.f, 5, 0.08f);
	GetAnimator()->Play(L"Spark", false);
}

CSpark::~CSpark()
{
}

void CSpark::tick()
{
	m_fMoveTime += DT;
	if (m_fMoveTime > 0.4f)
	{
		tEventInfo info = {};
		info.eType = EVENT_TYPE::DELETE_OBJECT;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}

	CObject::tick();
}