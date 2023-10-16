#include "pch.h"
#include "CAnimation.h"

#include "CObject.h"
#include "CAnimator.h"

#include "CTimeMgr.h"

#include "CImage.h"


CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pImage(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::tick()
{
	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{		
		m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;

		// ������ �ε����� �ʰ��� ��� ==> Animation ��� �Ϸ�
		if (m_vecFrm.size() <= m_iCurFrm + 1)
		{
			m_bFinish = true;
		}
		else
		{
			++m_iCurFrm;
		}
	}	
}

void CAnimation::render(HDC _dc)
{
	CObject* pOwner = m_pAnimator->GetOwner();
	
	Vec2 vPos = pOwner->GetPos();


	BLENDFUNCTION tFunc = {};
	tFunc.BlendOp = AC_SRC_OVER;
	tFunc.BlendFlags = 0;
	tFunc.AlphaFormat = AC_SRC_ALPHA;
	tFunc.SourceConstantAlpha = 255;

	// Frame �� Offset �� ���Ѵ�. ==> ���� ��ġ
	vPos += m_vecFrm[m_iCurFrm].vOffset;

	AlphaBlend(_dc
		, int(vPos.x - m_vecFrm[m_iCurFrm].vSize.x / 2.f)
		, int(vPos.y - m_vecFrm[m_iCurFrm].vSize.y / 2.f)
		, int(m_vecFrm[m_iCurFrm].vSize.x)
		, int(m_vecFrm[m_iCurFrm].vSize.y)
		, m_pImage->GetDC()
		, int(m_vecFrm[m_iCurFrm].vLeftTop.x)
		, int(m_vecFrm[m_iCurFrm].vLeftTop.y)
		, int(m_vecFrm[m_iCurFrm].vSize.x)
		, int(m_vecFrm[m_iCurFrm].vSize.y)
		, tFunc);
}

void CAnimation::Create(CImage* _pImage, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset
	, float _fStep, UINT _iFrmCount, float _fDuration)
{
	m_pImage = _pImage;

	for (UINT i = 0; i < _iFrmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vLeftTop.x = _vLeftTop.x;
		frm.vLeftTop.y = _vLeftTop.y + (_fStep * (float)i);
		frm.vSize = _vSize;
		frm.vOffset = _vOffset;
		frm.fDuration = _fDuration;				

		m_vecFrm.push_back(frm);
	}
}