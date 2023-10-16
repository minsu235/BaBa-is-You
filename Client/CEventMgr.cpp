#include "pch.h"
#include "CEventMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CScene_Stage.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::tick()
{
	// Dead Object 를 실제로 삭제한다.
	for (size_t i = 0; i < m_vecDeadObj.size(); ++i)
	{
		SAFE_DELETE(m_vecDeadObj[i]);
	}
	m_vecDeadObj.clear();


	// 이번 프레임에 쌓인 이벤트를 처리한다.
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		tEventInfo& evt = m_vecEvent[i];

		switch (evt.eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CObject* pNewObject = (CObject*)evt.first;
			LAYER_TYPE eLayer = (LAYER_TYPE)evt.second;

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->AddObject(pNewObject, eLayer);
		}
		break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			CObject* pDeadObj = (CObject*)evt.first;

			if (!pDeadObj->IsDead())
			{
				m_vecDeadObj.push_back(pDeadObj);
				pDeadObj->SetDead();
			}
		}

		break;
		case EVENT_TYPE::SCENE_CHANGE:
		{
			SCENE_TYPE eSceneType = (SCENE_TYPE)evt.first;
			if (eSceneType == SCENE_TYPE::STAGE)
			{
				int i = (int)evt.second;
				if (i == 0)
					CSceneMgr::GetInst()->ChangeScene(L"Baba_Is_You");
				else if (i == 1)
					CSceneMgr::GetInst()->ChangeScene(L"Out_of_Reach");
				else if (i == 2)
					CSceneMgr::GetInst()->ChangeScene(L"Lock");
				else if (i == 3)
					CSceneMgr::GetInst()->ChangeScene(L"Volcano");
				else if (i == 4)
					CSceneMgr::GetInst()->ChangeScene(L"Bridge_Bulding");
				else if (i == 5)
					CSceneMgr::GetInst()->ChangeScene(L"Tiny_Isle");
				else if (i == 6)
					CSceneMgr::GetInst()->ChangeScene(L"Evaporating_River");
				else if (i == 7)
					CSceneMgr::GetInst()->ChangeScene(L"Research_Facility");
				else if (i == 8)
					CSceneMgr::GetInst()->ChangeScene(L"Test");

			}
			else
				CSceneMgr::GetInst()->ChangeScene(eSceneType);
		}
		break;

		case EVENT_TYPE::RESTART:
		{
			CSceneMgr::GetInst()->Restart();
		}
		break;

		case EVENT_TYPE::LAYER_CHANGE_YOU:
		{
			CScene* pCurScene = (CScene*)CSceneMgr::GetInst()->GetCurScene();
			CThing* pThing = (CThing*)evt.first;
			vector<CObject*>& vec = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::OBJECT];
			vector<CObject*>& vec2 = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::STICKOBJ];
			vector<CObject*>& vec3 = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::TEXT];
			if ((UINT)Thingtype::WALL <= (UINT)pThing->getThingtype() && (UINT)pThing->getThingtype() <= (UINT)Thingtype::WATER)
			{
				vector<CObject*>::iterator iter = vec2.begin();
				for (; iter != vec2.end(); ++iter)
				{
					if (*iter == (CObject*)evt.first)
						break;
				}
				if (iter != vec2.end())
				{
					pCurScene->AddObject(pThing, LAYER_TYPE::YOU);
					pThing->addState(RULE::YOU);
					vec2.erase(iter);
				}
			}
			else if ((UINT)Thingtype::TEXT == (UINT)pThing->getThingtype())
			{
				vector<CObject*>::iterator iter = vec3.begin();
				for (; iter != vec3.end(); ++iter)
				{
					if (*iter == (CObject*)evt.first)
						break;
				}
				if (iter != vec3.end())
				{
					pCurScene->AddObject(pThing, LAYER_TYPE::YOU);
					pThing->addState(RULE::YOU);
					vec3.erase(iter);
				}
			}
			else
			{
				vector<CObject*>::iterator iter = vec.begin();
				for (; iter != vec.end(); ++iter)
				{
					if (*iter == (CObject*)evt.first)
						break;
				}
				if (iter != vec.end())
				{
					pCurScene->AddObject(pThing, LAYER_TYPE::YOU);
					pThing->addState(RULE::YOU);
					vec.erase(iter);
				}
			}
		}
		break;

		case EVENT_TYPE::LAYER_CHANGE_OBJ:
		{
			CScene* pCurScene = (CScene*)CSceneMgr::GetInst()->GetCurScene();
			vector<CObject*>& vec = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::YOU];
			vector<CObject*>::iterator iter = vec.begin();
			for (; iter != vec.end(); ++iter)
			{
				if (*iter == (CObject*)evt.first)
					break;
			}
			if (iter != vec.end())
			{
				CThing* p = (CThing*)evt.first;
				if ((UINT)Thingtype::WALL <= (UINT)p->getThingtype() && (UINT)p->getThingtype() <= (UINT)Thingtype::WATER)
					pCurScene->AddObject(p, LAYER_TYPE::STICKOBJ);
				else if ((UINT)Thingtype::TEXT == (UINT)p->getThingtype())
					pCurScene->AddObject(p, LAYER_TYPE::TEXT);
				else
					pCurScene->AddObject(p, LAYER_TYPE::OBJECT);
				p->subState(RULE::YOU);
				vec.erase(iter);
			}
		}
		break;

		case EVENT_TYPE::DEAD_THING:
		{
			CThing* pDeadThing = (CThing*)evt.first;

			if (!pDeadThing->IsDead())
			{
				pDeadThing->SetDead();
				Vec2 DeadThingMatrix = pDeadThing->getMatrix();
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				pCurScene->Stackpushback(moveinfo(pDeadThing, pCurScene->getActCount(), MoveType::DEAD, (DWORD_PTR)DeadThingMatrix.x, (DWORD_PTR)DeadThingMatrix.y));
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				for (; _iter != mapMatrix.end(); ++_iter)
				{
					if (_iter->second == pDeadThing)
					{
						mapMatrix.erase(_iter);
						break;
					}
				}
			}
		}
		break;

		case EVENT_TYPE::ALL_DELETE:
		{
			CScene* pCurScene = (CScene*)CSceneMgr::GetInst()->GetCurScene();
			vector<CObject*>& vec = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::YOU];
			vector<CObject*>::iterator iter = vec.begin();
			for (; iter != vec.end();)
			{
				CThing* p = (CThing*)*iter;
				if ((UINT)Thingtype::WALL <= (UINT)p->getThingtype() && (UINT)p->getThingtype() <= (UINT)Thingtype::WATER)
					pCurScene->AddObject(p, LAYER_TYPE::STICKOBJ);
				else if ((UINT)Thingtype::TEXT == (UINT)p->getThingtype())
					pCurScene->AddObject(p, LAYER_TYPE::TEXT);
				else
					pCurScene->AddObject(p, LAYER_TYPE::OBJECT);
				iter = vec.erase(iter);
			}
			vector<CObject*>& vec2 = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::OBJECT];
			vector<CObject*>::iterator iter2 = vec2.begin();
			for (; iter2 != vec2.end(); ++iter2)
			{
				CThing* p = (CThing*)*iter2;
				p->initState();
			}
			vector<CObject*>& vec3 = pCurScene->m_arrLayer[(UINT)LAYER_TYPE::STICKOBJ];
			vector<CObject*>::iterator iter3 = vec3.begin();
			for (; iter3 != vec3.end(); ++iter3)
			{
				CThing* p = (CThing*)*iter3;
				p->initState();
			}
		}
		break;

		case EVENT_TYPE::WIN:
		{
			CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
			CThing* thing1 = (CThing*)evt.first;
			CThing* thing2 = (CThing*)evt.second;

			if (!thing1->IsDead() && !thing2->IsDead())
			{
				thing1->Winner();
				pCurScene->sceneWin();
			}
		}
		break;
		}

	}

	m_vecEvent.clear();
}