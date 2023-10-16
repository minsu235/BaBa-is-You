#include "pch.h"
#include "CScene.h"

#include "CEngine.h"

#include "CObject.h"
#include "CThing.h"
#include "CTile.h"

CScene::CScene()
	: m_iTileCol(0)
	, m_iTileRow(0)
{
}

CScene::~CScene()
{
	DeleteAllObject();	
}

void CScene::SetFocusedUI(CObject* _pUI)
{
	vector<CObject*>::iterator iter = m_arrLayer[(UINT)LAYER_TYPE::UI].begin();
	for (; iter != m_arrLayer[(UINT)LAYER_TYPE::UI].end(); ++iter)
	{
		if (*iter == _pUI)
		{
			m_arrLayer[(UINT)LAYER_TYPE::UI].erase(iter);
			break;
		}
	}	
	m_arrLayer[(UINT)LAYER_TYPE::UI].push_back(_pUI);
}

void CScene::tick()
{	
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_arrLayer[i].size(); ++j)
		{
			if (!m_arrLayer[i][j]->IsDead())
			{
				m_arrLayer[i][j]->tick();
			}
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		vector<CObject*>::iterator iter = m_arrLayer[i].begin();
		for (; iter != m_arrLayer[i].end(); ++iter)
		{
			if ((UINT)LAYER_TYPE::STICKOBJ <= i && i <= (UINT)LAYER_TYPE::TEXT)
			{
				CThing* _thing = (CThing*)(*iter);
				if (_thing->isgetState(RULE::FLOAT))
				{
					m_arrLayer[i + 4].push_back(*iter);
					continue;
				}
			}

			if (!(*iter)->IsDead())
				(*iter)->render(_dc);
		}

		if ((UINT)LAYER_TYPE::FLOATSTICKOBJ <= i && i <= (UINT)LAYER_TYPE::FLOATTEXT)
		{
			m_arrLayer[i].clear();
		}

		if (i == (UINT)LAYER_TYPE::EFFECT)
		{
			vector<CObject*>::iterator iter = m_arrLayer[i].begin();
			for (; iter != m_arrLayer[i].end();)
			{
				if (!(*iter)->IsDead())
					++iter;
				else
					iter = m_arrLayer[i].erase(iter);
			}
		}
	}
}

void CScene::DeleteAllObject()
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		DeleteObject((LAYER_TYPE)i);
	}
}

void CScene::DeleteObject(LAYER_TYPE _eType)
{	
	vector<CObject*>& vecObj = m_arrLayer[(UINT)_eType];

	for (UINT j = 0; j < vecObj.size(); ++j)
	{
		SAFE_DELETE(vecObj[j]);
	}	

	vecObj.clear();
}