#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CEngine.h"


CCollider::CCollider()
	: CComponent(COMPONENT_TYPE::COLLIDER)
	, m_iCollisionCount(0)
{
}

CCollider::CCollider(const CCollider& _collider)
	: CComponent(_collider)
	, m_vOffsetPos(_collider.m_vOffsetPos)
	, m_vScale(_collider.m_vScale)
	, m_vFinalPos()
	, m_iCollisionCount(0)
{
}

CCollider::~CCollider()
{
}

void CCollider::tick()
{
	CObject* pOwner = GetOwner();

	m_vFinalPos = pOwner->GetPos() + m_vOffsetPos;
}

void CCollider::render(HDC _dc)
{
	// 충돌 회수 제어 오류 발생
	assert(0 <= m_iCollisionCount);

	tSelectPen pen(_dc);
	if ( 0 < m_iCollisionCount)
		pen.SetPenColor(PEN_COLOR::RED);	
	else
		pen.SetPenColor(PEN_COLOR::GREEN);	

	tSelectBrush brush(_dc, BRUSH_COLOR::DEFAULT);

	Vec2 vPos = m_vFinalPos;

	Rectangle(_dc, (int)(vPos.x - m_vScale.x / 2.f)
		, (int)(vPos.y - m_vScale.y / 2.f)
		, (int)(vPos.x + m_vScale.x / 2.f)
		, (int)(vPos.y + m_vScale.y / 2.f));
}


void CCollider::CollisionBeginOverlap(CCollider* _pOtherCollider)
{
	++m_iCollisionCount;

	GetOwner()->CollisionBeginOverlap(_pOtherCollider);
}

void CCollider::CollisionOverlap(CCollider* _pOtherCollider)
{
	GetOwner()->CollisionOverlap(_pOtherCollider);
}

void CCollider::CollisionEndOverlap(CCollider* _pOtherCollider)
{
	--m_iCollisionCount;

	GetOwner()->CollisionEndOverlap(_pOtherCollider);
}