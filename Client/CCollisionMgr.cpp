#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::tick()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (m_matrix[iRow] & (1 << iCol))
			{
				CollisionBtwLayer(pCurScene, (LAYER_TYPE)iRow, (LAYER_TYPE)iCol);
			}
		}
	}
}


void CCollisionMgr::CollisionBtwLayer(CScene* _pCurScene, LAYER_TYPE _eLeft, LAYER_TYPE _eRight)
{
	const vector<CObject*>& vecLeft = _pCurScene->GetObjects(_eLeft);
	const vector<CObject*>& vecRight = _pCurScene->GetObjects(_eRight);
		
	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		size_t j = 0;
		if (_eLeft == _eRight)
		{
			j = i + 1;
		}
		

		for (; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->GetCollider())
			{
				continue;
			}

			CollisionBtwCollider(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider());
		}
	}
}

void CCollisionMgr::CollisionBtwCollider(CCollider* _pLeft, CCollider* _pRight)
{
	// 두 충돌체의 ID 를 확인
	COLLIDER_ID id;
	id.LEFT_ID = _pLeft->GetID();
	id.RIGHT_ID = _pRight->GetID();

	// 이전 충돌 정보를 검색한다.
	map<LONGLONG, bool>::iterator iter = m_mapColInfo.find(id.ID);

	// 충돌정보가 아예 없으면 만들어준다.
	if (iter == m_mapColInfo.end())
	{
		m_mapColInfo.insert(make_pair(id.ID, false));
		iter = m_mapColInfo.find(id.ID);
	}

	// 두 충돌체중 하나 이상의 Dead 상태인지
	bool IsDead = _pLeft->GetOwner()->IsDead() || _pRight->GetOwner()->IsDead();


	// 현재 충돌중인지 검사 한다.			
	if (IsCollision(_pLeft, _pRight))
	{
		// 충돌 중이다.
		if (false == iter->second)
		{
			// 이전 프레임에는 충돌하지 않았다.
			// ㄴ--> 이번 프레임에 충돌 진입
			if (!IsDead)
			{
				_pLeft->CollisionBeginOverlap(_pRight);
				_pRight->CollisionBeginOverlap(_pLeft);
				iter->second = true;
			}
		}

		// 충돌 중
		else 
		{
			// 삭제 예정인 경우, 충돌을 해제하는 방향으로 진행
			if (IsDead)
			{
				_pLeft->CollisionEndOverlap(_pRight);
				_pRight->CollisionEndOverlap(_pLeft);
				iter->second = false;
			}

			// 이전에도 충돌, 지금도 충돌 중
			else
			{
				_pLeft->CollisionOverlap(_pRight);
				_pRight->CollisionOverlap(_pLeft);
			}			
		}
	}

	else
	{
		// 충돌 X
		if (iter->second)
		{
			// 이전에는 충돌하고 있었다
			// ㄴ--> 충돌을 막 벗어난 시점
			_pLeft->CollisionEndOverlap(_pRight);
			_pRight->CollisionEndOverlap(_pLeft);
			iter->second = false;
		}
	}
}


bool CCollisionMgr::IsCollision(CCollider* _pLeft, CCollider* _pRight)
{
	Vec2 vLeftPos = _pLeft->GetFinalPos();
	Vec2 vLeftScale = _pLeft->GetScale();

	Vec2 vRightPos = _pRight->GetFinalPos();
	Vec2 vRightScale = _pRight->GetScale();


	// 두 충돌체의 중점간의 거리와, 각 면적의 절반끼리의 합을 비교해서 
	// 각 축(x, y) 별로 겹치는 부분이 동시에 존재하는지 체크한다.
	if (fabs(vLeftPos.x - vRightPos.x) < (vLeftScale.x / 2.f + vRightScale.x / 2.f) 
		&& fabs(vLeftPos.y - vRightPos.y) < (vLeftScale.y / 2.f + vRightScale.y / 2.f))
	{
		return true;
	}

	return false;
}






void CCollisionMgr::CollisionLayerCheck(LAYER_TYPE _eLeft, LAYER_TYPE _eRight)
{
	// 더 작은 숫자를 행(배열의 인덱스) 으로, 더 큰 숫자를 열(비트 위치) 로
	UINT iRow = 0, iCol = 0;

	if ((UINT)_eLeft <= (UINT)_eRight)
	{
		iRow = (UINT)_eLeft;
		iCol = (UINT)_eRight;
	}
	else
	{
		iRow = (UINT)_eRight; 
		iCol = (UINT)_eLeft;
	}
		
	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionLayerRelease(LAYER_TYPE _eLeft, LAYER_TYPE _eRight)
{
	// 더 작은 숫자를 행(배열의 인덱스) 으로, 더 큰 숫자를 열(비트 위치) 로
	UINT iRow = 0, iCol = 0;

	if ((UINT)_eLeft <= (UINT)_eRight)
	{
		iRow = (UINT)_eLeft;
		iCol = (UINT)_eRight;
	}
	else
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	m_matrix[iRow] &= ~(1 << iCol);
}
