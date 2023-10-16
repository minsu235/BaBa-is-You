#include "pch.h"
#include "CRuleEventMgr.h"

#include "CThing.h"
#include "CScene_Stage.h"
#include "CSceneMgr.h"
#include "CBurned.h"
#include "CResMgr.h"
#include "CSound.h"

CRuleEventMgr::CRuleEventMgr()
{

}

CRuleEventMgr::~CRuleEventMgr()
{

}

void CRuleEventMgr::tick()
{
	for (size_t i = 0; i < size_t(RULE_EVENT_TYPE::END); ++i)
	{
		for (size_t layercount = 0; layercount < m_vecRuleEvent[i].size(); ++layercount)
		{
			tRuleEventInfo& evt = m_vecRuleEvent[i][layercount];
			switch (evt.eType)
			{
			case RULE_EVENT_TYPE::MOVE_MAP:
			{
				DIR _dir = (DIR)evt.second;
				Vec2 pos = evt._Thing->getMatrix();
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				Vec2 m_vSize = pCurScene->getSize();
				multimap<int, CThing*>::iterator _iter = mapMatrix.find(int(pos.x * m_vSize.x + pos.y));

				if (_iter != mapMatrix.end())
				{
					_iter = mapMatrix.begin();
					while (_iter->second != evt._Thing)
					{
						++_iter;
					}

					switch (_dir)
					{
					case DIR::UP:
					{
						mapMatrix.insert(make_pair(m_vSize.x * (pos.x - 1) + pos.y, _iter->second));
					}
					break;
					case DIR::DOWN:
					{
						mapMatrix.insert(make_pair(m_vSize.x * (pos.x + 1) + pos.y, _iter->second));
					}
					break;
					case DIR::LEFT:
					{
						mapMatrix.insert(make_pair(m_vSize.x * pos.x + pos.y - 1, _iter->second));
					}
					break;
					case DIR::RIGHT:
					{
						mapMatrix.insert(make_pair(m_vSize.x * pos.x + pos.y + 1, _iter->second));
					}
					break;
					}
					mapMatrix.erase(_iter);
				}
			}
			break;

			case RULE_EVENT_TYPE::TEXT_ON:
				evt._Thing->on();
			break;

			case RULE_EVENT_TYPE::TEXT_OFF:
				evt._Thing->off();
			break;

			case RULE_EVENT_TYPE::ADD_RULE:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				tRuleEventInfo info = {};
				info._Thing = evt._Thing;
				info.second = evt.second;
				info.third = evt.third;
				CThing* third = (CThing*)info.third;
				if (third->getTexttype() == Texttype::PROPERTY)
					info.eType = RULE_EVENT_TYPE::ADD_RULE_OP;
				else if (third->getTexttype() == Texttype::OBJECT)
					info.eType = RULE_EVENT_TYPE::ADD_RULE_OO;
				AddRule(info);
			}
			break;

			case RULE_EVENT_TYPE::ADD_RULE_OP:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				CThing* second = (CThing*)evt.second;
				CThing* third = (CThing*)evt.third;
				if (second->getThingtype() != Thingtype::TEXT || third->getThingtype() != Thingtype::TEXT)
					break;
				vector<CThing*>& m_arrThing = pCurScene->getarrThing((Thingtype)second->getText());
				for (int i = 0; i < m_arrThing.size(); ++i)
				{
					m_arrThing[i]->addState((RULE)((UINT)third->getText() - (UINT)Text::YOU));
					if ((RULE)((UINT)third->getText() - (UINT)Text::YOU) == RULE::YOU)
					{
						tEventInfo info = {};
						info.eType = EVENT_TYPE::LAYER_CHANGE_YOU;
						info.first = (DWORD_PTR)m_arrThing[i];
						CEventMgr::GetInst()->AddEvent(info);
					}
				}
			}
			break;

			case RULE_EVENT_TYPE::ADD_RULE_OO:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				CThing* second = (CThing*)evt.second;
				CThing* third = (CThing*)evt.third;
				vector<CThing*>& m_arrThing = pCurScene->getarrThing((Thingtype)second->getText());
				for (int i = 0; i < m_arrThing.size(); ++i)
				{
					CThing* _unit = m_arrThing[i];
					if ((_unit->getChangecheck() || _unit->isChange()) && !pCurScene->IsUndo())
					{
						tRuleEventInfo info = {};
						info.eType = RULE_EVENT_TYPE::CHANGE_UNIT;
						info._Thing = _unit;
						info.second = (DWORD_PTR)second->getText();
						info.third = (DWORD_PTR)third->getText();
						CRuleEventMgr::GetInst()->AddRule(info);
						pCurScene->CountUp();
						pCurScene->Stackpushback(moveinfo(_unit, pCurScene->getActCount(), MoveType::CHANGE, (DWORD_PTR)_unit->getThingtype(), (DWORD_PTR)_unit->getText()));
					}
				}
			}
			break;

			case RULE_EVENT_TYPE::SUB_RULE:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				CThing* second = (CThing*)evt.second;
				CThing* third = (CThing*)evt.third;
				if ((float)third->getText() < (float)Text::IS)
				{
					vector<CThing*>& m_arr = pCurScene->getarrThing(third->getThingtype());
					for (int j = 0; j < m_arr.size(); ++j)
					{
						CThing* unit = m_arr[j];
						unit->setChangecheck(true);
					}
				}
				else
				{
					vector<CThing*>& m_arrThing = pCurScene->getarrThing((Thingtype)second->getText());
					for (int i = 0; i < m_arrThing.size(); ++i)
					{
						if (second->getText() == third->getText())
							m_arrThing[i]->subState(RULE::ME);
						else
						{
							m_arrThing[i]->subState((RULE)((UINT)third->getText() - (UINT)Text::YOU));
							if ((RULE)((UINT)third->getText() - (UINT)Text::YOU) == RULE::YOU)
							{
								tEventInfo info = {};
								info.eType = EVENT_TYPE::LAYER_CHANGE_OBJ;
								info.first = (DWORD_PTR)m_arrThing[i];
								CEventMgr::GetInst()->AddEvent(info);
							}
						}
					}
				}
			}
			break;

			case RULE_EVENT_TYPE::CHANGE_UNIT:
			{
				if (evt.second == evt.third)
				{
					evt._Thing->addState(RULE::ME);
					break;
				}

				if (evt._Thing->isgetState(RULE::ME))
					break;

				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();

				evt._Thing->ChangeUnit((Thingtype)evt.third);

				if ((Thingtype)evt.second != Thingtype::TEXT)
				{
					vector<CThing*>& vecThing = pCurScene->getarrThing((Thingtype)evt.second);
					vector<CThing*>::iterator iter = vecThing.begin();
					for (; iter != vecThing.end(); ++iter)
					{
						if (*iter == evt._Thing)
							break;
					}
					if (iter != vecThing.end())
					{
						vecThing.erase(iter);
						vector<CThing*>& vecThing2 = pCurScene->getarrThing((Thingtype)evt.third);
						vecThing2.push_back(evt._Thing);
					}
					if ((Thingtype)evt.third == Thingtype::TEXT)
					{
						vector<CThing*>& vecText = pCurScene->getarrText(Texttype::OBJECT);
						vecText.push_back(evt._Thing);
					}

					for (UINT c = 0; c < (UINT)RULE::END; ++c)
					{
						evt._Thing->subState((RULE)c);
					}
				}
				else if ((Thingtype)evt.second == Thingtype::TEXT)
				{
					vector<CThing*>& vecThing = pCurScene->getarrThing((Thingtype)evt.second);
					vector<CThing*>::iterator iter = vecThing.begin();
					for (; iter != vecThing.end(); ++iter)
					{
						if (*iter == evt._Thing)
							break;
					}
					if (iter != vecThing.end())
					{
						vecThing.erase(iter);
						vector<CThing*>& vecThing2 = pCurScene->getarrThing((Thingtype)evt.third);
						vecThing2.push_back(evt._Thing);
					}
					if ((Thingtype)evt.third != Thingtype::TEXT)
					{
						for (UINT textint = 0; textint < (UINT)Texttype::NONE; ++textint)
						{
							vector<CThing*>& vecText = pCurScene->getarrText((Texttype)textint);
							vecText.clear();
						}
					}

					for (UINT c = 0; c < (UINT)RULE::END; ++c)
					{
						evt._Thing->subState((RULE)c);
					}
					tEventInfo info = {};
					info.eType = EVENT_TYPE::ALL_DELETE;
					CEventMgr::GetInst()->AddEvent(info);
					pCurScene->Sentenceclear();
				}

				tRuleEventInfo _info = {};
				_info.eType = RULE_EVENT_TYPE::CHANGE_LAYER;
				_info._Thing = evt._Thing;
				_info.second = evt.second;
				_info.third = evt.third;
				AddRule(_info);
			}
			break;

			case RULE_EVENT_TYPE::CHANGE_LAYER:
			{
				CScene* pCurScene = (CScene*)CSceneMgr::GetInst()->GetCurScene();
				LAYER_TYPE prevLayer = LAYER_TYPE::OBJECT;
				LAYER_TYPE nextLayer = LAYER_TYPE::OBJECT;

				if ((UINT)evt.second == (UINT)Thingtype::TEXT)
					prevLayer = LAYER_TYPE::TEXT;
				else if ((UINT)Thingtype::WALL <= (UINT)evt.second && (UINT)evt.second <= (UINT)Thingtype::WATER)
					prevLayer = LAYER_TYPE::STICKOBJ;

				if ((UINT)evt.third == (UINT)Thingtype::TEXT)
					nextLayer = LAYER_TYPE::TEXT;
				else if ((UINT)Thingtype::WALL <= (UINT)evt.third && (UINT)evt.third <= (UINT)Thingtype::WATER)
					nextLayer = LAYER_TYPE::STICKOBJ;

				if (prevLayer == nextLayer)
					break;

				vector<CObject*>& vec = pCurScene->m_arrLayer[(UINT)prevLayer];
				vector<CObject*>::iterator iter = vec.begin();
				for (; iter != vec.end(); ++iter)
				{
					if (*iter == (CObject*)evt._Thing)
						break;
				}
				if (iter != vec.end())
				{
					pCurScene->AddObject(evt._Thing, nextLayer);
					vec.erase(iter);
				}
			}
			break;

			case RULE_EVENT_TYPE::UNDO_FINISH:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				pCurScene->UndoFinish();
			}
			break;

			case RULE_EVENT_TYPE::SINK:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter2 = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter3 = mapMatrix.begin();

				for (; _iter != mapMatrix.end();)
				{
					bool isSink = false;
					_iter2 = _iter;
					++_iter2;
					_iter3 = _iter2;
					for (; _iter2 != mapMatrix.end(); ++_iter2)
					{
						if (_iter->first == _iter2->first)
						{
							if (((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
								|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								&& (_iter->second->isgetState(RULE::SINK) || _iter2->second->isgetState(RULE::SINK)))
							{
								isSink = true;
								break;
							}
						}
						else
							break;
					}
					if (isSink)
					{
						for (; _iter2 != mapMatrix.end(); ++_iter2)
						{
							if (_iter->first == _iter2->first)
							{
								if ((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
									|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									info.first = (DWORD_PTR)_iter->second;
									CEventMgr::GetInst()->AddEvent(info);
									SparkCreate(L"Sink", _iter->second->GetPos());
									random_device rnd;
									wstring str = to_wstring(rnd() % 4);
									CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_sink" + str);
									pEffect->Play();
								}
							}
							else
								break;
						}
					}
					_iter = _iter3;
				}
			}
			break;
			case RULE_EVENT_TYPE::ALIVE_THING:
			{
				if (evt._Thing->IsDead())
				{
					evt._Thing->setAlive();
					Vec2 AliveThingMatrix = Vec2((float)evt.second, (float)evt.third);
					CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
					multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
					Vec2 Size = pCurScene->getSize();
					mapMatrix.insert(make_pair(Size.x * AliveThingMatrix.x + AliveThingMatrix.y, evt._Thing));
				}
			}
			break;
			case RULE_EVENT_TYPE::SHUTOPEN:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter2 = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter3 = mapMatrix.begin();

				for (; _iter != mapMatrix.end();)
				{
					if (_iter->second->isgetState(RULE::SHUT) && _iter->second->isgetState(RULE::OPEN))
					{
						tEventInfo info = {};
						info.eType = EVENT_TYPE::DEAD_THING;
						info.first = (DWORD_PTR)_iter->second;
						CEventMgr::GetInst()->AddEvent(info);
						SparkCreate(L"Open", _iter->second->GetPos());
						random_device rnd;
						wstring str = to_wstring(rnd() % 4);
						CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
						pEffect->Play();
					}
					bool isShut = false;
					bool isOpen = false;
					bool isFloatShut = false;
					bool isFloatOpen = false;
					bool isOpening = false;
					bool isShutopen = false;
					bool isFloatOpening = false;
					bool isFloatShutopen = false;
					_iter2 = _iter;
					++_iter2;
					for (; _iter2 != mapMatrix.end(); ++_iter2)
					{
						if (_iter->first == _iter2->first)
						{
							if ((_iter->second->isgetState(RULE::SHUT) && !_iter->second->isgetState(RULE::FLOAT))
								|| (_iter2->second->isgetState(RULE::SHUT) && !_iter2->second->isgetState(RULE::FLOAT)))
								isShut = true;
							if ((_iter->second->isgetState(RULE::OPEN) && !_iter->second->isgetState(RULE::FLOAT))
								|| (_iter2->second->isgetState(RULE::OPEN) && !_iter2->second->isgetState(RULE::FLOAT)))
								isOpen = true;
							if ((_iter->second->isgetState(RULE::SHUT) && _iter->second->isgetState(RULE::FLOAT))
								|| (_iter2->second->isgetState(RULE::SHUT) && _iter2->second->isgetState(RULE::FLOAT)))
								isFloatShut = true;
							if ((_iter->second->isgetState(RULE::OPEN) && _iter->second->isgetState(RULE::FLOAT))
								|| (_iter2->second->isgetState(RULE::OPEN) && _iter2->second->isgetState(RULE::FLOAT)))
								isFloatOpen = true;
						}
						else
						{
							_iter3 = _iter2;
							break;
						}
					}

					if (isShut && isOpen)
					{
						_iter2 = _iter;
						++_iter2;
						for (; _iter2 != mapMatrix.end(); ++_iter2)
						{
							if (_iter->first == _iter2->first)
							{
								if (!isShutopen && _iter2->second->isgetState(RULE::SHUT) && !_iter2->second->isgetState(RULE::FLOAT))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									isShutopen = true;
								}
								if (!isOpening && _iter2->second->isgetState(RULE::OPEN) && !_iter2->second->isgetState(RULE::FLOAT))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									isOpening = true;
								}
							}
							else
								break;
						}
						if (!isShutopen && _iter->second->isgetState(RULE::SHUT) && !_iter->second->isgetState(RULE::FLOAT))
						{
							tEventInfo info = {};
							info.eType = EVENT_TYPE::DEAD_THING;
							info.first = (DWORD_PTR)_iter->second;
							CEventMgr::GetInst()->AddEvent(info);
							SparkCreate(L"Open", _iter->second->GetPos());
							random_device rnd;
							wstring str = to_wstring(rnd() % 4);
							CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
							pEffect->Play();
						}
						if (!isOpening && _iter->second->isgetState(RULE::OPEN) && !_iter->second->isgetState(RULE::FLOAT))
						{
							tEventInfo info = {};
							info.eType = EVENT_TYPE::DEAD_THING;
							info.first = (DWORD_PTR)_iter->second;
							CEventMgr::GetInst()->AddEvent(info);
							SparkCreate(L"Open", _iter->second->GetPos());
							random_device rnd;
							wstring str = to_wstring(rnd() % 4);
							CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
							pEffect->Play();
						}
					}

					if (isFloatShut && isFloatOpen)
					{
						_iter2 = _iter;
						++_iter2;
						for (; _iter2 != mapMatrix.end(); ++_iter2)
						{
							if (_iter->first == _iter2->first)
							{
								if (!isShutopen && _iter2->second->isgetState(RULE::SHUT) && _iter2->second->isgetState(RULE::FLOAT))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									isFloatShutopen = true;
								}
								if (!isOpening && _iter2->second->isgetState(RULE::OPEN) && _iter2->second->isgetState(RULE::FLOAT))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									isFloatOpening = true;
								}
							}
							else
								break;
						}
						if (!isShutopen && _iter->second->isgetState(RULE::SHUT) && _iter->second->isgetState(RULE::FLOAT))
						{
							tEventInfo info = {};
							info.eType = EVENT_TYPE::DEAD_THING;
							info.first = (DWORD_PTR)_iter->second;
							CEventMgr::GetInst()->AddEvent(info);
							SparkCreate(L"Open", _iter->second->GetPos());
							random_device rnd;
							wstring str = to_wstring(rnd() % 4);
							CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
							pEffect->Play();
						}
						if (!isOpening && _iter->second->isgetState(RULE::OPEN) && _iter->second->isgetState(RULE::FLOAT))
						{
							tEventInfo info = {};
							info.eType = EVENT_TYPE::DEAD_THING;
							info.first = (DWORD_PTR)_iter->second;
							CEventMgr::GetInst()->AddEvent(info);
							SparkCreate(L"Open", _iter->second->GetPos());
							random_device rnd;
							wstring str = to_wstring(rnd() % 4);
							CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
							pEffect->Play();
						}
					}

					if (_iter2 == mapMatrix.end())
						_iter = _iter2;
					else
						_iter = _iter3;
				}
			}
			break;
			case RULE_EVENT_TYPE::MELT:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter2 = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter3 = mapMatrix.begin();

				for (; _iter != mapMatrix.end();)
				{
					if (_iter->second->isgetState(RULE::MELT) && _iter->second->isgetState(RULE::HOT))
					{
						tEventInfo info = {};
						info.eType = EVENT_TYPE::DEAD_THING;
						info.first = (DWORD_PTR)_iter->second;
						CEventMgr::GetInst()->AddEvent(info);
						CBurned* pBurnEffect = new CBurned(_iter->second->GetPos());
						info = {};
						info.eType = EVENT_TYPE::CREATE_OBJECT;
						info.first = (DWORD_PTR)pBurnEffect;
						info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
						CEventMgr::GetInst()->AddEvent(info);
						CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_burned");
						pEffect->Play();
					}
					bool isMelt = false;
					_iter2 = _iter;
					++_iter2;
					_iter3 = _iter2;
					for (; _iter2 != mapMatrix.end(); ++_iter2)
					{
						if (_iter->first == _iter2->first)
						{
							if (((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
								|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								&& ((_iter->second->isgetState(RULE::HOT) && _iter2->second->isgetState(RULE::MELT))
									|| (_iter->second->isgetState(RULE::MELT) && _iter2->second->isgetState(RULE::HOT))))
							{
								isMelt = true;
								break;
							}
						}
						else
							break;
					}

					if (isMelt)
					{
						_iter2 = _iter;
						++_iter2;
						for (; _iter2 != mapMatrix.end(); ++_iter2)
						{
							if (_iter->first == _iter2->first)
							{
								if (((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
									|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
									&& (_iter2->second->isgetState(RULE::MELT)))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									info.first = (DWORD_PTR)_iter2->second;
									CEventMgr::GetInst()->AddEvent(info);
									CBurned* pBurnEffect = new CBurned(_iter2->second->GetPos());
									info = {};
									info.eType = EVENT_TYPE::CREATE_OBJECT;
									info.first = (DWORD_PTR)pBurnEffect;
									info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
									CEventMgr::GetInst()->AddEvent(info);
									CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_burned");
									pEffect->Play();
								}
							}
							else
								break;
						}
						if (_iter->second->isgetState(RULE::MELT))
						{
							tEventInfo info = {};
							info.eType = EVENT_TYPE::DEAD_THING;
							info.first = (DWORD_PTR)_iter->second;
							CEventMgr::GetInst()->AddEvent(info);
							CBurned* pBurnEffect = new CBurned(_iter->second->GetPos());
							info = {};
							info.eType = EVENT_TYPE::CREATE_OBJECT;
							info.first = (DWORD_PTR)pBurnEffect;
							info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
							CEventMgr::GetInst()->AddEvent(info);
							CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_burned");
							pEffect->Play();
						}
					}
					_iter = _iter3;
				}
			}
			break;
			case RULE_EVENT_TYPE::DEFEAT:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter2 = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter3 = mapMatrix.begin();

				for (; _iter != mapMatrix.end();)
				{
					if (_iter->second->isgetState(RULE::DEFEAT) && _iter->second->isgetState(RULE::YOU))
					{
						tEventInfo info = {};
						info.eType = EVENT_TYPE::DEAD_THING;
						info.first = (DWORD_PTR)_iter->second;
						CEventMgr::GetInst()->AddEvent(info);
						SparkCreate(L"Defeat", _iter->second->GetPos());
						random_device rnd;
						wstring str = to_wstring(rnd() % 3);
						CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_defeat" + str);
						pEffect->Play();
					}
					bool isDefeat = false;
					_iter2 = _iter;
					++_iter2;
					_iter3 = _iter2;
					for (; _iter2 != mapMatrix.end(); ++_iter2)
					{
						if (_iter->first == _iter2->first)
						{
							if (((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
								|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								&& ((_iter->second->isgetState(RULE::DEFEAT) && _iter2->second->isgetState(RULE::YOU))
								|| (_iter->second->isgetState(RULE::YOU) && _iter2->second->isgetState(RULE::DEFEAT))))
							{
								isDefeat = true;
								break;
							}
						}
						else
							break;
					}
					if (isDefeat)
					{
						for (; _iter2 != mapMatrix.end(); ++_iter2)
						{
							if (_iter->first == _iter2->first)
							{
								if ((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
									|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								{
									tEventInfo info = {};
									info.eType = EVENT_TYPE::DEAD_THING;
									if (_iter->second->isgetState(RULE::YOU))
									{
										info.first = (DWORD_PTR)_iter->second;
										CEventMgr::GetInst()->AddEvent(info);
										SparkCreate(L"Defeat", _iter->second->GetPos());
										random_device rnd;
										wstring str = to_wstring(rnd() % 3);
										CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_defeat" + str);
										pEffect->Play();
									}
									if (_iter2->second->isgetState(RULE::YOU))
									{
										info.first = (DWORD_PTR)_iter2->second;
										CEventMgr::GetInst()->AddEvent(info);
										SparkCreate(L"Defeat", _iter2->second->GetPos());
										random_device rnd;
										wstring str = to_wstring(rnd() % 3);
										CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_defeat" + str);
										pEffect->Play();
									}									
								}
							}
							else
								break;
						}
					}
					_iter = _iter3;
				}
			}
			break;

			case RULE_EVENT_TYPE::WIN:
			{
				CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
				multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
				multimap<int, CThing*>::iterator _iter = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter2 = mapMatrix.begin();
				multimap<int, CThing*>::iterator _iter3 = mapMatrix.begin();

				for (; _iter != mapMatrix.end();)
				{
					if (_iter->second->isgetState(RULE::WIN) && _iter->second->isgetState(RULE::YOU))
					{
						tEventInfo info = {};
						info.eType = EVENT_TYPE::WIN;
						info.first = (DWORD_PTR)_iter->second;
						info.second = (DWORD_PTR)_iter->second;
						CEventMgr::GetInst()->AddEvent(info);
					}
					_iter2 = _iter;
					++_iter2;
					_iter3 = _iter2;
					for (; _iter2 != mapMatrix.end(); ++_iter2)
					{
						if (_iter->first == _iter2->first)
						{
							if (((_iter->second->isgetState(RULE::FLOAT) && _iter2->second->isgetState(RULE::FLOAT))
								|| (!_iter->second->isgetState(RULE::FLOAT) && !_iter2->second->isgetState(RULE::FLOAT)))
								&& ((_iter->second->isgetState(RULE::WIN) && _iter2->second->isgetState(RULE::YOU))
									|| (_iter->second->isgetState(RULE::YOU) && _iter2->second->isgetState(RULE::WIN))))
							{
								tEventInfo info = {};
								info.eType = EVENT_TYPE::WIN;
								info.first = (DWORD_PTR)_iter->second;
								info.second = (DWORD_PTR)_iter2->second;
								CEventMgr::GetInst()->AddEvent(info);
								break;
							}
						}
						else
							break;
					}
					
					_iter = _iter3;
				}
			}
			break;

			}
		}
		m_vecRuleEvent[i].clear();
	}
}
