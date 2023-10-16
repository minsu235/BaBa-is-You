#include "pch.h"
#include "CScene_Stage.h"

#include "CKeyMgr.h"
#include "CRuleEventMgr.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CSceneMgr.h"
#include "CResMgr.h"
#include "CSound.h"

#include "CTile.h"
#include "CThing.h"
#include "CMenu.h"
#include "CMenu_anim.h"
#include "CWinTitle.h"

CScene_Stage::CScene_Stage()
	: m_bFade(true)
	, m_fFadetime(0)
	, m_done(false)
	, m_ftime(0)
	, m_fYoutime(0)
	, m_actCount(0)
	, m_iscountup(true)
	, m_isUndo(true)
	, m_uProperty(0)
	, m_eScenestate(Scenestate::PLAY)
	, m_Bababutton(nullptr)
	, m_iPoint(1)
	, m_sMapName{}
	, m_bGrid(false)
	, m_movesound(false)
	, m_textsound(false)
	, m_noise(nullptr)
{
}

CScene_Stage::~CScene_Stage()
{

}

void CScene_Stage::enter()
{
}

void CScene_Stage::enter(wstring _stage)
{
	m_sMapName = _stage;
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"Stage\\"+ _stage + L".stage";

	const wchar_t* szFilePath = strFilePath.c_str();
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"rb");

	wstring str;
	LoadWString(str, pFile);
	CSound* pBGM = CResMgr::GetInst()->FindSound(str);
	pBGM->PlayToBGM(true);

	UINT _iRow = 0;
	UINT _iCol = 0;
	fread(&_iRow, sizeof(UINT), 1, pFile);
	fread(&_iCol, sizeof(UINT), 1, pFile);
	m_vSize.y = (float)_iRow;
	m_vSize.x = (float)_iCol;

	m_bGrid = CSceneMgr::GetInst()->getGrid();
	if (m_bGrid)
	{
		for (UINT iRow = 0; iRow < _iRow; ++iRow)
		{
			for (UINT iCol = 0; iCol < _iCol; ++iCol)
			{
				CTile* pTile = new CTile;
				pTile->SetPos(tilePos(iRow, iCol));
				pTile->setIdx(Vec2((float)iRow, (float)iCol));
				AddObject(pTile, LAYER_TYPE::TILE);
			}
		}
	}

	int i = 0;
	fread(&i, sizeof(int), 1, pFile);
	Text _type = Text::END;
	Vec2 vec2;
	for (int count = 0; count < i; ++count)
	{
		fread(&_type, sizeof(Text), 1, pFile);
		fread(&vec2, sizeof(Vec2), 1, pFile);
		makeText(_type, vec2);
	}
	fread(&i, sizeof(int), 1, pFile);
	Thingtype thingtype = Thingtype::END;
	DIR dir = DIR::NONE;
	for (int count = 0; count < i; ++count)
	{
		fread(&thingtype, sizeof(Thingtype), 1, pFile);
		fread(&vec2, sizeof(Vec2), 1, pFile);
		fread(&dir, sizeof(DIR), 1, pFile);
		makeUnit(thingtype, vec2, dir);
	}

	fclose(pFile);

	sentence();
}

void CScene_Stage::exit()
{
	DeleteAllObject();
}

void CScene_Stage::render(HDC _dc)
{
	tSelectPen pen(_dc);
	pen.SetPenColor(PEN_COLOR::BLACK);

	tSelectBrush brush(_dc, BRUSH_COLOR::BLACK);


	Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
	Vec2 vPos = res / 2;
	Vec2 vSize = m_vSize * TILE_SIZE;

	Rectangle(_dc, (int)(vPos.x - vSize.x / 2.f)
		, (int)(vPos.y - vSize.y / 2.f)
		, (int)(vPos.x + vSize.x / 2.f)
		, (int)(vPos.y + vSize.y / 2.f));

	CScene::render(_dc);
}

void CScene_Stage::tick()
{
	if (m_bFade)
	{
		if (m_fFadetime == 0)
		{
			CMenu_anim* Fadein = new CMenu_anim(L"Fadein");
			Fadein->SetPos(Vec2(0, 0));
			AddObject(Fadein, LAYER_TYPE::FADEINOUT);
			m_fFadetime += DT;
		}
		else if (m_fFadetime > 0.8)
		{
			DeleteObject(LAYER_TYPE::FADEINOUT);
			m_bFade = false;
			m_fFadetime = 0;
		}
		else
			m_fFadetime += DT;
		CScene::tick();
		return;
	}

	const vector<CObject*>& arr = GetObjects(LAYER_TYPE::YOU);
	int youcount = 0;
	for (size_t i = 0; i < arr.size(); ++i)
	{
		if (!arr[i]->IsDead())
			++youcount;
	}
	if (youcount == 0 && m_eScenestate == Scenestate::PLAY)
	{
		if (m_noise == nullptr)
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_noise");
			m_noise = pBGM;
			pBGM->PlayToNoise();
			m_fYoutime += DT;
		}
		else if (m_fYoutime == 0)
		{
			m_fYoutime = 0;
		}
		else if (m_fYoutime > 3)
		{
			CMenu* Z = new CMenu(L"KeyUI_Z");
			Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
			Z->SetPos(Vec2(res.x / 2 - 121.f, 30.f));
			AddObject(Z, LAYER_TYPE::FADEINOUT);

			CMenu* R = new CMenu(L"KeyUI_R");
			R->SetPos(Vec2(res.x / 2 + 31.f, 30.f));
			AddObject(R, LAYER_TYPE::FADEINOUT);

			CMenu_anim* VocaUI_RESTART = new CMenu_anim(L"VocaUI_RESTART");
			VocaUI_RESTART->SetPos(Vec2(res.x / 2 + 107.f, 30.f));
			AddObject(VocaUI_RESTART, LAYER_TYPE::FADEINOUT);

			CMenu_anim* VocaUI_UNDO = new CMenu_anim(L"VocaUI_UNDO");
			VocaUI_UNDO->SetPos(Vec2(res.x / 2 - 45.f, 30.f));
			AddObject(VocaUI_UNDO, LAYER_TYPE::FADEINOUT);

			m_fYoutime = 0;
		}
		else
			m_fYoutime += DT;
	}
	else if(youcount != 0 && m_noise != nullptr)
	{
		m_noise->StopToNoise();
		m_noise = nullptr;
		m_fYoutime = 0;
		DeleteObject(LAYER_TYPE::FADEINOUT);
	}

	if (m_eScenestate == Scenestate::PLAY)
	{
		if (m_done == true)
		{
			vector<CObject*> YOUarr = GetObjects(LAYER_TYPE::YOU);
			if (IsPressed(KEY_TYPE::UP))
			{
				m_iscountup = true;
				for (size_t i = 0; i < YOUarr.size(); ++i)
				{
					if (!YOUarr[i]->IsDead())
						thingMove((CThing*)YOUarr[i], DIR::UP);
				}

				for (UINT arrthingcount = 0; arrthingcount <= (UINT)Thingtype::TEXT; arrthingcount++)
				{
					for (size_t vectorcount = 0; vectorcount < m_arrThing[arrthingcount].size(); vectorcount++)
						m_arrThing[arrthingcount][vectorcount]->Change(true);
				}
				if (isgetProperty(RULE::MOVE))
				{
					for (UINT thinglayer = 0; thinglayer < (UINT)Thingtype::END; ++thinglayer)
					{
						for (size_t i = 0; i < m_arrThing[thinglayer].size(); ++i)
						{
							if (m_arrThing[thinglayer][i]->isgetState(RULE::MOVE))
								movethingMove(m_arrThing[thinglayer][i]);
						}
					}
				}
				m_done = false;
			}
			else if (IsPressed(KEY_TYPE::DOWN))
			{
				m_iscountup = true;
				for (size_t i = 0; i < YOUarr.size(); ++i)
				{
					if (!YOUarr[i]->IsDead())
						thingMove((CThing*)YOUarr[i], DIR::DOWN);
				}

				for (UINT arrthingcount = 0; arrthingcount <= (UINT)Thingtype::TEXT; arrthingcount++)
				{
					for (size_t vectorcount = 0; vectorcount < m_arrThing[arrthingcount].size(); vectorcount++)
						m_arrThing[arrthingcount][vectorcount]->Change(true);
				}
				if (isgetProperty(RULE::MOVE))
				{
					for (UINT thinglayer = 0; thinglayer < (UINT)Thingtype::END; ++thinglayer)
					{
						for (size_t i = 0; i < m_arrThing[thinglayer].size(); ++i)
						{
							if (m_arrThing[thinglayer][i]->isgetState(RULE::MOVE))
								movethingMove(m_arrThing[thinglayer][i]);
						}
					}
				}
				m_done = false;
			}
			else if (IsPressed(KEY_TYPE::LEFT))
			{
				m_iscountup = true;
				for (size_t i = 0; i < YOUarr.size(); ++i)
				{
					if (!YOUarr[i]->IsDead())
						thingMove((CThing*)YOUarr[i], DIR::LEFT);
				}

				for (UINT arrthingcount = 0; arrthingcount <= (UINT)Thingtype::TEXT; arrthingcount++)
				{
					for (size_t vectorcount = 0; vectorcount < m_arrThing[arrthingcount].size(); vectorcount++)
						m_arrThing[arrthingcount][vectorcount]->Change(true);
				}
				if (isgetProperty(RULE::MOVE))
				{
					for (UINT thinglayer = 0; thinglayer < (UINT)Thingtype::END; ++thinglayer)
					{
						for (size_t i = 0; i < m_arrThing[thinglayer].size(); ++i)
						{
							if (m_arrThing[thinglayer][i]->isgetState(RULE::MOVE))
								movethingMove(m_arrThing[thinglayer][i]);
						}
					}
				}
				m_done = false;
			}
			else if (IsPressed(KEY_TYPE::RIGHT))
			{
				m_iscountup = true;
				for (size_t i = 0; i < YOUarr.size(); ++i)
				{
					if (!YOUarr[i]->IsDead())
						thingMove((CThing*)YOUarr[i], DIR::RIGHT);
				}

				for (UINT arrthingcount = 0; arrthingcount <= (UINT)Thingtype::TEXT; arrthingcount++)
				{
					for (size_t vectorcount = 0; vectorcount < m_arrThing[arrthingcount].size(); vectorcount++)
						m_arrThing[arrthingcount][vectorcount]->Change(true);
				}
				if (isgetProperty(RULE::MOVE))
				{
					for (UINT thinglayer = 0; thinglayer < (UINT)Thingtype::END; ++thinglayer)
					{
						for (size_t i = 0; i < m_arrThing[thinglayer].size(); ++i)
						{
							if (m_arrThing[thinglayer][i]->isgetState(RULE::MOVE))
								movethingMove(m_arrThing[thinglayer][i]);
						}
					}
				}
				m_done = false;
			}
			else if (IsPressed(KEY_TYPE::SPACE))
			{
				m_iscountup = true;
				if (isgetProperty(RULE::MOVE))
				{
					for (UINT thinglayer = 0; thinglayer < (UINT)Thingtype::END; ++thinglayer)
					{
						for (size_t i = 0; i < m_arrThing[thinglayer].size(); ++i)
						{
							if (m_arrThing[thinglayer][i]->isgetState(RULE::MOVE))
								movethingMove(m_arrThing[thinglayer][i]);
						}
					}
				}
				m_done = false;
			}
			else if (IsPressed(KEY_TYPE::Z))
			{
				if (m_actCount != 0)
				{
					m_done = false;
					m_iscountup = false;
					m_isUndo = true;

					if (m_stackMoveinfo.back().actCount == m_actCount)
					{
						random_device rnd;
						wstring str = to_wstring(rnd() % 5);
						CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_undo" + str);
						pEffect->Play();
					}

					while (m_stackMoveinfo.back().actCount == m_actCount)
					{
						moveinfo pMove = m_stackMoveinfo.back();
						m_stackMoveinfo.pop_back();
						switch (pMove.moveType)
						{
						case MoveType::MOVE:
						{
							tRuleEventInfo info = {};
							info.eType = RULE_EVENT_TYPE::MOVE_MAP;
							info._Thing = pMove.moveThing;
							info.second = (DWORD_PTR)ReverseDIR((DIR)pMove.first);
							CRuleEventMgr::GetInst()->AddRule(info);
							pMove.moveThing->thingReverseMove(ReverseDIR((DIR)pMove.first), (DIR)pMove.second);
						}
						break;
						case MoveType::FOCUS:
							pMove.moveThing->setFocusDIR((DIR)pMove.second);
							pMove.moveThing->FocusChange();
							break;
						case MoveType::CHANGE:
						{
							tRuleEventInfo _info = {};
							_info.eType = RULE_EVENT_TYPE::CHANGE_LAYER;
							_info._Thing = pMove.moveThing;
							_info.second = (DWORD_PTR)pMove.moveThing->getThingtype();
							_info.third = (DWORD_PTR)pMove.first;
							CRuleEventMgr::GetInst()->AddRule(_info);
							if ((Thingtype)pMove.first != Thingtype::TEXT)
							{
								vector<CThing*>::iterator iter = m_arrThing[(UINT)pMove.moveThing->getThingtype()].begin();
								for (; iter != m_arrThing[(UINT)pMove.moveThing->getThingtype()].end(); ++iter)
								{
									if (*iter == pMove.moveThing)
										break;
								}
								if (iter != m_arrThing[(UINT)pMove.moveThing->getThingtype()].end())
								{
									m_arrThing[(UINT)pMove.moveThing->getThingtype()].erase(iter);
									m_arrThing[(UINT)pMove.first].push_back(pMove.moveThing);
								}
								if (pMove.moveThing->getThingtype() == Thingtype::TEXT)
								{
									vector<CThing*>::iterator iter2 = m_arrText[(UINT)Texttype::OBJECT].begin();
									for (; iter2 != m_arrText[(UINT)Texttype::OBJECT].end(); ++iter2)
									{
										if (*iter2 == pMove.moveThing)
											break;
									}
									m_arrText[(UINT)Texttype::OBJECT].erase(iter2);
								}

								for (UINT c = 0; c < (UINT)RULE::END; ++c)
								{
									pMove.moveThing->subState((RULE)c);
								}

								pMove.moveThing->ChangeUnit((Thingtype)pMove.first);

							}
							else if ((Thingtype)pMove.first == Thingtype::TEXT)
							{
								vector<CThing*>::iterator iter = m_arrThing[(UINT)pMove.moveThing->getThingtype()].begin();
								for (; iter != m_arrThing[(UINT)pMove.moveThing->getThingtype()].end(); ++iter)
								{
									if (*iter == pMove.moveThing)
										break;
								}
								if (iter != m_arrThing[(UINT)pMove.moveThing->getThingtype()].end())
								{
									m_arrThing[(UINT)pMove.moveThing->getThingtype()].erase(iter);
									m_arrThing[(UINT)pMove.first].push_back(pMove.moveThing);
								}
								Texttype _texttype = Texttype::OBJECT;
								if ((Text)pMove.second == Text::IS)
									_texttype = Texttype::IS;
								else if ((Text)pMove.second == Text::AND)
									_texttype = Texttype::AND;
								else if (Text::YOU <= (Text)pMove.second && (Text)pMove.second < Text::END)
									_texttype = Texttype::PROPERTY;

								m_arrText[(UINT)_texttype].push_back(pMove.moveThing);

								for (UINT c = 0; c < (UINT)RULE::END; ++c)
								{
									pMove.moveThing->subState((RULE)c);
								}

								pMove.moveThing->ChangeUnit((Text)pMove.second);
								tRuleEventInfo ruleevent = {};
								ruleevent.eType = RULE_EVENT_TYPE::TEXT_OFF;
								ruleevent._Thing = pMove.moveThing;
								CRuleEventMgr::GetInst()->AddRule(ruleevent);
							}
						}
						break;
						case MoveType::DEAD:
						{
							tRuleEventInfo info = {};
							info.eType = RULE_EVENT_TYPE::ALIVE_THING;
							info._Thing = pMove.moveThing;
							info.second = pMove.first;
							info.third = pMove.second;
							CRuleEventMgr::GetInst()->AddRule(info);
						}
						break;
						}

						if (m_stackMoveinfo.empty())
							break;
					}
					--m_actCount;
				}
			}
			else if (IsTap(KEY_TYPE::ESC))
			{
				m_eScenestate = Scenestate::ESC;
				CMenu* Back = new CMenu(L"ESCBackground");
				Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
				Back->SetPos(Vec2(res.x / 2, res.y / 2));
				AddObject(Back, LAYER_TYPE::MENU);
				CMenu* resume = new CMenu(L"ESCResume");
				resume->SetPos(Vec2(res.x / 2, res.y / 2 - 230));
				AddObject(resume, LAYER_TYPE::MENU);
				CMenu* restart = new CMenu(L"ESCRestart");
				restart->SetPos(Vec2(res.x / 2, res.y / 2 - 195));
				AddObject(restart, LAYER_TYPE::MENU);
				CMenu* enablegrid = new CMenu(L"ESCGrid");
				enablegrid->SetPos(Vec2(res.x / 2, res.y / 2 - 160));
				AddObject(enablegrid, LAYER_TYPE::MENU);
				CMenu* returntomap = new CMenu(L"ESCMapOn");
				returntomap->SetPos(Vec2(res.x / 2, res.y / 2 - 125));
				AddObject(returntomap, LAYER_TYPE::MENU);
				CMenu* returntomenu = new CMenu(L"ESCRmenu");
				returntomenu->SetPos(Vec2(res.x / 2, res.y / 2 - 75));
				AddObject(returntomenu, LAYER_TYPE::MENU);
				CMenu_anim* Bababutton = new CMenu_anim(L"BabaButton");
				Bababutton->SetPos(Vec2(res.x / 2 - 168, res.y / 2 - 246));
				m_Bababutton = Bababutton;
				AddObject(Bababutton, LAYER_TYPE::MENU);
			}
			else if (IsTap(KEY_TYPE::R))
			{
				m_eScenestate = Scenestate::RESTART;
				CMenu* Back = new CMenu(L"ESCBackground");
				Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
				Back->SetPos(Vec2(res.x / 2, res.y / 2));
				AddObject(Back, LAYER_TYPE::MENU);
				CMenu* restart = new CMenu(L"Restart");
				restart->SetPos(Vec2(res.x / 2, res.y / 2));
				AddObject(restart, LAYER_TYPE::MENU);
				CMenu_anim* Bababutton = new CMenu_anim(L"BabaButton");
				Bababutton->SetPos(Vec2(res.x / 2 - 225, res.y / 2 - 6));
				m_Bababutton = Bababutton;
				AddObject(Bababutton, LAYER_TYPE::MENU);
			}
		}
		else if (m_done == false)
		{
			for (UINT arrthingcount = 0; arrthingcount < (UINT)Thingtype::TEXT; arrthingcount++)
			{
				for (size_t vectorcount = 0; vectorcount < m_arrThing[arrthingcount].size(); vectorcount++)
					m_arrThing[arrthingcount][vectorcount]->Change(true);
			}
			m_ftime += DT;
		}

		if (m_ftime > 0.15)
		{
			m_ftime = 0;
			m_done = true;
			m_movesound = false;
			m_textsound = false;
			sentence();
			tRuleEventInfo info = {};

			if (m_isUndo)
			{
				info.eType = RULE_EVENT_TYPE::UNDO_FINISH;
				CRuleEventMgr::GetInst()->AddRule(info);
			}
			else
			{
				if (isgetProperty(RULE::SINK))
				{
					info.eType = RULE_EVENT_TYPE::SINK;
					CRuleEventMgr::GetInst()->AddRule(info);
				}
				if (isgetProperty(RULE::SHUT))
				{
					info.eType = RULE_EVENT_TYPE::SHUTOPEN;
					CRuleEventMgr::GetInst()->AddRule(info);
				}
				if (isgetProperty(RULE::MELT))
				{
					info.eType = RULE_EVENT_TYPE::MELT;
					CRuleEventMgr::GetInst()->AddRule(info);
				}
				if (isgetProperty(RULE::DEFEAT))
				{
					info.eType = RULE_EVENT_TYPE::DEFEAT;
					CRuleEventMgr::GetInst()->AddRule(info);
				}
				if (isgetProperty(RULE::WIN))
				{
					info.eType = RULE_EVENT_TYPE::WIN;
					CRuleEventMgr::GetInst()->AddRule(info);
				}
			}
		}
	}
	else if (m_eScenestate == Scenestate::RESTART)
	{
		if (IsTap(KEY_TYPE::UP))
		{
			if (m_iPoint != 1)
			{
				--m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos -= Vec2(0, 48);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::DOWN))
		{
			if (m_iPoint != 2)
			{
				++m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos += Vec2(0, 48);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::ENTER))
		{
			if (m_iPoint == 1)
			{
				m_Bababutton = nullptr;
				m_eScenestate = Scenestate::PLAY;
				DeleteObject(LAYER_TYPE::MENU);
			}
			else if (m_iPoint == 2)
				m_eScenestate = Scenestate::RESTARTFADEOUT;
		}
		else if (IsTap(KEY_TYPE::ESC))
		{
			m_Bababutton = nullptr;
			m_eScenestate = Scenestate::PLAY;
			DeleteObject(LAYER_TYPE::MENU);
		}
	}
	else if (m_eScenestate == Scenestate::RESTARTFADEOUT)
	{
		if (m_fFadetime == 0)
		{
			if (m_noise != nullptr)
				m_noise->StopToNoise();
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"Effect_restart");
			pBGM->Play();
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fFadetime > 0.8)
		{
			tEventInfo info = {};
			info.eType = EVENT_TYPE::RESTART;
			CEventMgr::GetInst()->AddEvent(info);
		}
		m_fFadetime += DT;
	}
	else if (m_eScenestate == Scenestate::ESC)
	{
		if (IsTap(KEY_TYPE::UP))
		{
			if (m_iPoint != 1 && m_iPoint != 5)
			{
				--m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos -= Vec2(0, 35);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
			else if (m_iPoint == 5)
			{
				--m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos -= Vec2(0, 50);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::DOWN))
		{
			if (m_iPoint != 5 && m_iPoint != 4)
			{
				++m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos += Vec2(0, 35);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
			else if (m_iPoint == 4)
			{
				++m_iPoint;
				Vec2 vPos = m_Bababutton->GetPos();
				vPos += Vec2(0, 50);
				random_device rnd;
				wstring str = to_wstring(rnd() % 5);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_menumove" + str);
				pEffect->Play();
				m_Bababutton->SetPos(vPos);
			}
		}
		else if (IsTap(KEY_TYPE::ENTER))
		{
			if (m_iPoint == 1)
			{
				m_Bababutton = nullptr;
				m_eScenestate = Scenestate::PLAY;
				DeleteObject(LAYER_TYPE::MENU);
			}
			else if (m_iPoint == 2)
				m_eScenestate = Scenestate::RESTARTFADEOUT;
			else if (m_iPoint == 3)
				CSceneMgr::GetInst()->GridChange();
			else if (m_iPoint == 4)
			{
				m_eScenestate = Scenestate::MAPFADEOUT;
				if (m_noise != nullptr)
					m_noise->StopToNoise();
				CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_menuselect");
				pBGM->PlayToBGM();
			}
			else if (m_iPoint == 5)
				m_eScenestate = Scenestate::MENUFADEOUT;
		}
		else if (IsTap(KEY_TYPE::ESC))
		{
			m_Bababutton = nullptr;
			m_eScenestate = Scenestate::PLAY;
			DeleteObject(LAYER_TYPE::MENU);
			m_iPoint = 1;
		}
	}
	else if (m_eScenestate == Scenestate::MAPFADEOUT)
	{
		if (m_fFadetime == 0)
		{
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fFadetime > 2)
			SceneChange(WORLD);

		m_fFadetime += DT;
	}
	else if (m_eScenestate == Scenestate::MENUFADEOUT)
	{
		if (m_fFadetime == 0)
		{
			if (m_noise != nullptr)
				m_noise->StopToNoise();
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_menuselect");
			pBGM->PlayToBGM();
			CMenu_anim* Fadeout = new CMenu_anim(L"Fadeout");
			Fadeout->SetPos(Vec2(0, 0));
			AddObject(Fadeout, LAYER_TYPE::FADEINOUT);
		}
		else if (m_fFadetime > 2)
			SceneChange(LOGO);

		m_fFadetime += DT;
	}
	else if (m_eScenestate == Scenestate::WIN)
	{
		if (m_fFadetime == 0)
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_win");
			pBGM->PlayToBGM();
			CWinTitle* win = new CWinTitle();
			Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
			win->SetPos(Vec2(res.x / 2, res.y / 2));
			AddObject(win, LAYER_TYPE::MENU);
			m_fFadetime += DT;
		}
		else if (m_fFadetime > 1.2)
		{
			m_eScenestate = Scenestate::MAPFADEOUT;
			m_fFadetime = 0;
		}
		else
		{
			m_fFadetime += DT;
		}
	}

	if (m_bGrid != CSceneMgr::GetInst()->getGrid())
	{
		m_bGrid = CSceneMgr::GetInst()->getGrid();
		if (m_bGrid)
		{
			for (UINT iRow = 0; iRow < m_vSize.y; ++iRow)
			{
				for (UINT iCol = 0; iCol < m_vSize.x; ++iCol)
				{
					CTile* pTile = new CTile;
					pTile->SetPos(tilePos(iRow, iCol));
					pTile->setIdx(Vec2((float)iRow, (float)iCol));
					AddObject(pTile, LAYER_TYPE::TILE);
				}
			}
		}
		else
		{
			DeleteObject(LAYER_TYPE::TILE);
		}
	}

	CScene::tick();
}

void CScene_Stage::textsound()
{
	if (!m_textsound)
	{
		random_device rnd;
		wstring str = to_wstring(rnd() % 5);
		CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_texton" + str);
		pEffect->Play();
		m_textsound = true;
	}
}

void CScene_Stage::makeText(Text _text, Vec2 _matrix)
{
	CThing* pText = new CThing(_text);
	pText->SetPos(tilePos((UINT)_matrix.x, (UINT)_matrix.y));
	pText->setOriginPos(tilePos((UINT)_matrix.x, (UINT)_matrix.y));
	pText->setMatrix(_matrix);
	pText->setThingtype(Thingtype::TEXT);
	AddObject(pText, LAYER_TYPE::TEXT);
	m_arrText[(UINT)pText->getTexttype()].push_back(pText);
	m_arrThing[(UINT)Thingtype::TEXT].push_back(pText);
	m_mapMatrix.insert(make_pair(m_vSize.x * _matrix.x + _matrix.y, pText));

	if (Text::YOU <= _text && _text < Text::END)
		addProperty((RULE)((UINT)_text - (UINT)Text::YOU));
}

void CScene_Stage::makeUnit(Thingtype _thingtype, Vec2 _matrix, DIR _dir)
{
	CThing* pObj = new CThing(_thingtype, _dir);
	pObj->SetPos(tilePos((UINT)_matrix.x, (UINT)_matrix.y));
	pObj->setOriginPos(tilePos((UINT)_matrix.x, (UINT)_matrix.y));
	pObj->setMatrix(_matrix);
	LAYER_TYPE Layertype = LAYER_TYPE::OBJECT;
	if ((UINT)Thingtype::WALL <= (UINT)_thingtype && (UINT)_thingtype <= (UINT)Thingtype::WATER)
		Layertype = LAYER_TYPE::STICKOBJ;
	else if ((UINT)Thingtype::TILE == (UINT)_thingtype)
		Layertype = LAYER_TYPE::STICKOBJ;

	AddObject(pObj, Layertype);
	m_arrThing[(UINT)_thingtype].push_back(pObj);
	m_mapMatrix.insert(make_pair(m_vSize.x * _matrix.x + _matrix.y, pObj));
}

bool CScene_Stage::thingMove(CThing* _thing, DIR _dir, bool _you)
{
	multimap<int, CThing*>::iterator iter = m_mapMatrix.begin();
	Vec2 pos = _thing->getMatrix();
	multimap<int, CThing*>::iterator _iter = m_mapMatrix.find((int)(pos.x * m_vSize.x + pos.y));
	size_t iCount = 0;
	bool isMove = true;

	if (_iter != m_mapMatrix.end())
	{
		if (MapMoveDIR(pos, _dir) != LIMIT)
		{
			iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
			iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
			for (size_t i = 0; i < iCount; ++i, ++iter)
			{
				CThing* Obj = iter->second;
				if (((Obj->isgetState(RULE::FLOAT) && _thing->isgetState(RULE::FLOAT))
					|| (!Obj->isgetState(RULE::FLOAT) && !_thing->isgetState(RULE::FLOAT)))
					&& ((Obj->isgetState(RULE::SHUT) && _thing->isgetState(RULE::OPEN))
						|| (Obj->isgetState(RULE::OPEN) && _thing->isgetState(RULE::SHUT))))
				{
					CountUp();
					tEventInfo info = {};
					info.eType = EVENT_TYPE::DEAD_THING;
					info.first = (DWORD_PTR)Obj;
					CEventMgr::GetInst()->AddEvent(info);
					info.first = (DWORD_PTR)_thing;
					CEventMgr::GetInst()->AddEvent(info);
					SparkCreate(L"Open", _thing->GetPos());
					SparkCreate(L"Open", Obj->GetPos());
					random_device rnd;
					wstring str = to_wstring(rnd() % 4);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
					pEffect->Play();
					isMove = false;
				}
				else if (Obj->isgetState(RULE::PUSH))
				{
					if ((Obj->isgetState(RULE::YOU)))
					{
						if (!thingMove(Obj, _dir, false))
							isMove = false;
					}
				}
				else if (Obj->isgetState(RULE::STOP))
				{
					isMove = false;
				}
			}
			iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
			iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
			if (isMove)
			{
				for (size_t i = 0; i < iCount; ++i, ++iter)
				{
					CThing* Obj = iter->second;
					if (Obj->isgetState(RULE::PUSH))
					{
						if (!(Obj->isgetState(RULE::YOU)))
							isMove = thingMove(Obj, _dir);;
					}
				}
			}
		}
		else
			isMove = false;
		if (_you)
		{
			if ((MapMoveDIR(pos, _dir) != LIMIT) && isMove == true)
			{
				CountUp();
				if (!m_movesound)
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_movesound = true;
				}
				m_stackMoveinfo.push_back(moveinfo(_thing, m_actCount, MoveType::MOVE, (DWORD_PTR)_dir, (DWORD_PTR)_thing->getFocusDIR()));

				tRuleEventInfo info = {};
				info.eType = RULE_EVENT_TYPE::MOVE_MAP;
				info._Thing = _thing;
				info.second = (DWORD_PTR)_dir;
				CRuleEventMgr::GetInst()->AddRule(info);
				_thing->thingMove(_dir);

				return true;
			}
			else if ((MapMoveDIR(pos, _dir) == LIMIT || isMove == false) && _thing->getFocusDIR() != _dir)
			{
				CountUp();
				if (!m_movesound)
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_movesound = true;
				}
				m_stackMoveinfo.push_back(moveinfo(_thing, m_actCount, MoveType::FOCUS, (DWORD_PTR)_dir, (DWORD_PTR)_thing->getFocusDIR()));

				_thing->setFocusDIR(_dir);
				_thing->FocusChange();

				return false;
			}
			else
				return false;
		}
		else
		{
			if ((MapMoveDIR(pos, _dir) != LIMIT) && isMove == true)
				return true;
			else if ((MapMoveDIR(pos, _dir) == LIMIT || isMove == false) && _thing->getFocusDIR() != _dir)
				return false;
			else
				return false;
		}

	}
	else
		return false;
}

void CScene_Stage::movethingMove(CThing* _thing)
{
	multimap<int, CThing*>::iterator iter = m_mapMatrix.begin();
	Vec2 pos = _thing->getMatrix();
	multimap<int, CThing*>::iterator _iter = m_mapMatrix.find((int)(pos.x * m_vSize.x + pos.y));
	size_t iCount = 0;
	bool isMove = true;
	DIR _dir = _thing->getFocusDIR();

	if (_iter != m_mapMatrix.end())
	{
		if (MapMoveDIR(pos, _dir) != LIMIT)
		{
			iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
			iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
			for (size_t i = 0; i < iCount; ++i, ++iter)
			{
				CThing* Obj = iter->second;
				if (((Obj->isgetState(RULE::FLOAT) && _thing->isgetState(RULE::FLOAT))
					|| (!Obj->isgetState(RULE::FLOAT) && !_thing->isgetState(RULE::FLOAT)))
					&& ((Obj->isgetState(RULE::SHUT) && _thing->isgetState(RULE::OPEN))
						|| (Obj->isgetState(RULE::OPEN) && _thing->isgetState(RULE::SHUT))))
				{
					CountUp();
					tEventInfo info = {};
					info.eType = EVENT_TYPE::DEAD_THING;
					info.first = (DWORD_PTR)Obj;
					CEventMgr::GetInst()->AddEvent(info);
					info.first = (DWORD_PTR)_thing;
					CEventMgr::GetInst()->AddEvent(info);
					SparkCreate(L"Open", _thing->GetPos());
					SparkCreate(L"Open", Obj->GetPos());
					random_device rnd;
					wstring str = to_wstring(rnd() % 4);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_open" + str);
					pEffect->Play();
					isMove = false;
				}
				else if (Obj->isgetState(RULE::PUSH))
				{
					if ((Obj->isgetState(RULE::YOU)))
					{
						if (!thingMove(Obj, _dir, false))
							isMove = false;
					}
				}
				else if (Obj->isgetState(RULE::STOP))
				{
					isMove = false;
				}
			}
			iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
			iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
			if (isMove)
			{
				for (size_t i = 0; i < iCount; ++i, ++iter)
				{
					CThing* Obj = iter->second;
					if (Obj->isgetState(RULE::PUSH))
					{
						if (!(Obj->isgetState(RULE::YOU)))
							isMove = thingMove(Obj, _dir);;
					}
				}
			}
		}
		else
			isMove = false;
		
		if ((MapMoveDIR(pos, _dir) != LIMIT) && isMove == true)
		{
			CountUp();
			if (!m_movesound)
			{
				random_device rnd;
				wstring str = to_wstring(rnd() % 12);
				CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
				pEffect->Play();
				m_movesound = true;
			}
			m_stackMoveinfo.push_back(moveinfo(_thing, m_actCount, MoveType::MOVE, (DWORD_PTR)_dir, (DWORD_PTR)_thing->getFocusDIR()));

			tRuleEventInfo info = {};
			info.eType = RULE_EVENT_TYPE::MOVE_MAP;
			info._Thing = _thing;
			info.second = (DWORD_PTR)_dir;
			CRuleEventMgr::GetInst()->AddRule(info);
			_thing->thingMove(_dir);
		}
		else if ((MapMoveDIR(pos, _dir) == LIMIT || isMove == false))
		{
			multimap<int, CThing*>::iterator iter = m_mapMatrix.begin();
			Vec2 pos = _thing->getMatrix();
			multimap<int, CThing*>::iterator _iter = m_mapMatrix.find((int)(pos.x * m_vSize.x + pos.y));
			size_t iCount = 0;
			bool isMove = true;
			DIR _dir = ReverseDIR(_thing->getFocusDIR());
			if (MapMoveDIR(pos, _dir) != LIMIT)
			{
				iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
				iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
				for (size_t i = 0; i < iCount; ++i, ++iter)
				{
					CThing* Obj = iter->second;
					if (((Obj->isgetState(RULE::FLOAT) && _thing->isgetState(RULE::FLOAT))
						|| (!Obj->isgetState(RULE::FLOAT) && !_thing->isgetState(RULE::FLOAT)))
						&& ((Obj->isgetState(RULE::SHUT) && _thing->isgetState(RULE::OPEN))
							|| (Obj->isgetState(RULE::OPEN) && _thing->isgetState(RULE::SHUT))))
					{
						CountUp();
						tEventInfo info = {};
						info.eType = EVENT_TYPE::DEAD_THING;
						info.first = (DWORD_PTR)Obj;
						CEventMgr::GetInst()->AddEvent(info);
						info.first = (DWORD_PTR)_thing;
						CEventMgr::GetInst()->AddEvent(info);
						SparkCreate(L"Open", _thing->GetPos());
						SparkCreate(L"Open", Obj->GetPos());
						isMove = false;
					}
					else if (Obj->isgetState(RULE::PUSH))
					{
						if ((Obj->isgetState(RULE::YOU)))
						{
							if (!thingMove(Obj, _dir, false))
								isMove = false;
						}
					}
					else if (Obj->isgetState(RULE::STOP))
					{
						isMove = false;
					}
				}
				iter = m_mapMatrix.find(MapMoveDIR(pos, _dir));
				iCount = m_mapMatrix.count(MapMoveDIR(pos, _dir));
				if (isMove)
				{
					for (size_t i = 0; i < iCount; ++i, ++iter)
					{
						CThing* Obj = iter->second;
						if (Obj->isgetState(RULE::PUSH))
						{
							if (!(Obj->isgetState(RULE::YOU)))
								isMove = thingMove(Obj, _dir);;
						}
					}
				}
			}
			else
				isMove = false;

			if ((MapMoveDIR(pos, _dir) != LIMIT) && isMove == true)
			{
				CountUp();
				m_stackMoveinfo.push_back(moveinfo(_thing, m_actCount, MoveType::MOVE, (DWORD_PTR)_dir, (DWORD_PTR)_thing->getFocusDIR()));
				if (!m_movesound)
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_movesound = true;
				}

				tRuleEventInfo info = {};
				info.eType = RULE_EVENT_TYPE::MOVE_MAP;
				info._Thing = _thing;
				info.second = (DWORD_PTR)_dir;
				CRuleEventMgr::GetInst()->AddRule(info);
				_thing->thingMove(_dir);
			}
			else if ((MapMoveDIR(pos, _dir) == LIMIT || isMove == false))
			{
				CountUp();
				m_stackMoveinfo.push_back(moveinfo(_thing, m_actCount, MoveType::FOCUS, (DWORD_PTR)_dir, (DWORD_PTR)_thing->getFocusDIR()));
				if (!m_movesound)
				{
					random_device rnd;
					wstring str = to_wstring(rnd() % 12);
					CSound* pEffect = CResMgr::GetInst()->FindSound(L"Effect_move" + str);
					pEffect->Play();
					m_movesound = true;
				}

				_thing->setFocusDIR(_dir);
				_thing->FocusChange();
			}
		}
	}
}

void CScene_Stage::textcheck()
{
	m_listTextlist.clear();
	multimap<int, CThing*>::iterator iter = m_mapMatrix.begin();
	multimap<int, CThing*>::iterator iter2 = m_mapMatrix.begin();
	
	for (; iter != m_mapMatrix.end(); ++iter)
	{
		size_t iCount = 0;
		Vec2 rightPos;
		Vec2 downPos;
		int size = 0;
		bool sizeup = false;
		bool finding = false;
		list<CThing*> rulearr;
		
		if (iter->second->getThingtype() == Thingtype::TEXT)
		{
			// 왼쪽 검사
			iCount = m_mapMatrix.count(MapMoveDIR(iter->second->getMatrix(), DIR::LEFT));
			iter2 = m_mapMatrix.find(MapMoveDIR(iter->second->getMatrix(), DIR::LEFT));
			for (size_t i = 0; i < iCount; ++i, ++iter2)
			{
				if (iter2 != m_mapMatrix.end())
				{
					if (iter2->second->getThingtype() == Thingtype::TEXT)
					{
						finding = true;
						break;
					}
				}
			}

			if (!finding)
			{
				// 오른쪽 검사
				rulearr.push_back(iter->second);
				++size;
				rightPos = MapDIR(iter->second->getMatrix(), DIR::RIGHT);
				iCount = m_mapMatrix.count(MapMoveDIR(iter->second->getMatrix(), DIR::RIGHT));
				iter2 = m_mapMatrix.find(MapMoveDIR(iter->second->getMatrix(), DIR::RIGHT));
				while (true)
				{
					for (size_t i = 0; i < iCount; ++i, ++iter2)
					{
						if (iter2 != m_mapMatrix.end())
						{
							if (iter2->second->getThingtype() == Thingtype::TEXT)
							{
								rulearr.push_back(iter2->second);
								++size;
								sizeup = true;
								break;
							}
						}
						else
							break;
					}
					if (sizeup)
					{
						iter2 = m_mapMatrix.find(MapMoveDIR(rightPos, DIR::RIGHT));
						iCount = m_mapMatrix.count(MapMoveDIR(rightPos, DIR::RIGHT));
						rightPos = MapDIR(rightPos, DIR::RIGHT);
						sizeup = false;
					}
					else
						break;
				}
				if (size >= 3)
				{
					m_listTextlist.push_back(rulearr);
				}
			}
			
			// 초기화
			finding = false;
			rulearr.clear();
			size = 0;
			iCount = 0;
			sizeup = false;

			// 위 검사
			iCount = m_mapMatrix.count(MapMoveDIR(iter->second->getMatrix(), DIR::UP));
			iter2 = m_mapMatrix.find(MapMoveDIR(iter->second->getMatrix(), DIR::UP));
			for (size_t i = 0; i < iCount; ++i, ++iter2)
			{
				if (iter2 != m_mapMatrix.end())
				{
					if (iter2->second->getThingtype() == Thingtype::TEXT)
					{
						finding = true;
						break;
					}
				}
			}

			if (!finding)
			{
				// 아래 검사
				rulearr.push_back(iter->second);
				++size;
				downPos = MapDIR(iter->second->getMatrix(), DIR::DOWN);
				iCount = m_mapMatrix.count(MapMoveDIR(iter->second->getMatrix(), DIR::DOWN));
				iter2 = m_mapMatrix.find(MapMoveDIR(iter->second->getMatrix(), DIR::DOWN));

				while (true)
				{
					for (size_t i = 0; i < iCount; ++i, ++iter2)
					{
						if (iter2 != m_mapMatrix.end())
						{
							if (iter2->second->getThingtype() == Thingtype::TEXT)
							{
								rulearr.push_back(iter2->second);
								++size;
								sizeup = true;
								break;
							}
						}
						else
							break;
					}
					if (sizeup)
					{
						iter2 = m_mapMatrix.find(MapMoveDIR(downPos, DIR::DOWN));
						iCount = m_mapMatrix.count(MapMoveDIR(downPos, DIR::DOWN));
						downPos = MapDIR(downPos, DIR::DOWN);
						sizeup = false;
					}
					else
						break;
				}
				if (size >= 3)
				{
					m_listTextlist.push_back(rulearr);
				}
			}
		}
	}
}

void CScene_Stage::sentencecheck()
{
	for (size_t i = 0; m_listTextlist.size() != 0; ++i)
	{
		list<CThing*> textlist = m_listTextlist.front();
		m_listTextlist.pop_front();
		sentenceorganize(textlist);
	}
}

void CScene_Stage::sentencedelete()
{
	while (m_listSentence.size() != 0)
	{
		SentenceInfo sentenceinfo = m_listSentence.front();
		m_listSentence.pop_front();
		tRuleEventInfo info = {};
		info.eType = RULE_EVENT_TYPE::SUB_RULE;
		info.second = (DWORD_PTR)sentenceinfo.first;
		info.third = (DWORD_PTR)sentenceinfo.second;
		CRuleEventMgr::GetInst()->AddRule(info);

		tRuleEventInfo ruleevent = {};
		ruleevent.eType = RULE_EVENT_TYPE::TEXT_OFF;
		ruleevent._Thing = sentenceinfo.first;
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
		ruleevent._Thing = sentenceinfo.second;
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
	}
	for (size_t i = 0; i < m_arrText[(UINT)Texttype::IS].size(); ++i)
	{
		tRuleEventInfo ruleevent = {};
		ruleevent.eType = RULE_EVENT_TYPE::TEXT_OFF;
		ruleevent._Thing = m_arrText[(UINT)Texttype::IS][i];
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
	}
	for (size_t i = 0; i < m_arrText[(UINT)Texttype::AND].size(); ++i)
	{
		tRuleEventInfo ruleevent = {};
		ruleevent.eType = RULE_EVENT_TYPE::TEXT_OFF;
		ruleevent._Thing = m_arrText[(UINT)Texttype::AND][i];
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
	}
}

void CScene_Stage::sentenceapply()
{
	list<SentenceInfo>::iterator iter = m_listSentence.begin();
	for(; iter != m_listSentence.end(); ++iter)
	{
		SentenceInfo sentenceinfo = *iter;
		tRuleEventInfo info = {};
		info.eType = RULE_EVENT_TYPE::ADD_RULE;
		info.second = (DWORD_PTR)sentenceinfo.first;
		info.third = (DWORD_PTR)sentenceinfo.second;
		CRuleEventMgr::GetInst()->AddRule(info);

		tRuleEventInfo ruleevent = {};
		ruleevent.eType = RULE_EVENT_TYPE::TEXT_ON;
		ruleevent._Thing = sentenceinfo.first;
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
		ruleevent._Thing = sentenceinfo.second;
		CRuleEventMgr::GetInst()->AddRule(ruleevent);
	}
}

bool CScene_Stage::sentenceorganize(list<CThing*> _list)
{
	while (true)
	{
		if (_list.size() >= 3)
		{
			if (_list.front()->getTexttype() != Texttype::OBJECT)
				_list.pop_front();
			else if (_list.back()->getTexttype() == Texttype::IS || _list.back()->getTexttype() == Texttype::AND)
				_list.pop_back();
			else
				break;
		}
		else
			return false;
	}
	
	list<CThing*> front;
	list<CThing*> back;
	list<CThing*>::iterator _listiter = _list.begin();
	list<CThing*>::iterator _listiter2 = _list.begin();
	CThing* thing = *_listiter;
	CThing* thing2 = *_listiter2;
	vector<CThing*> andlist;

	for (; _listiter != _list.end(); ++_listiter)
	{
		thing = *_listiter;
		if (thing->getTexttype() == Texttype::IS)
		{
			//is 앞쪽 검사
			_listiter2 = _listiter;
			--_listiter2;
			while (true)
			{
				thing2 = *_listiter2;
				if (thing2->getTexttype() == Texttype::OBJECT)
				{
					front.push_back(thing2);
					if (_listiter2 != _list.begin())
					{
						--_listiter2;
						thing2 = *_listiter2;
						if (thing2->getTexttype() == Texttype::AND)
						{
							if (_listiter2 != _list.begin())
							{
								andlist.push_back(thing2);
								--_listiter2;
								thing2 = *_listiter2;
								if (thing2->getTexttype() != Texttype::OBJECT)
									andlist.pop_back();
							}
							else
								break;
						}
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
			//is 뒤쪽 검사
			_listiter2 = _listiter;
			++_listiter2;
			while (_listiter2 != _list.end())
			{
				thing2 = *_listiter2;
				if (thing2->getTexttype() == Texttype::OBJECT || thing2->getTexttype() == Texttype::PROPERTY)
				{
					back.push_back(thing2);
					++_listiter2;
					if (_listiter2 != _list.end())
					{
						thing2 = *_listiter2;
						if (thing2->getTexttype() == Texttype::AND)
						{
							++_listiter2;
							if (_listiter2 != _list.end())
							{
								andlist.push_back(thing2);
								thing2 = *_listiter2;
								if (thing2->getTexttype() != Texttype::OBJECT && thing2->getTexttype() != Texttype::PROPERTY)
									andlist.pop_back();
							}
						}	
						else
							break;
					}
				}
				else
					break;
			}
			// 앞 뒤 보고 문장 저장
			if (front.size() != 0 && back.size() != 0)
			{
				//is on
				tRuleEventInfo ruleevent = {};
				ruleevent.eType = RULE_EVENT_TYPE::TEXT_ON;
				ruleevent._Thing = *_listiter;
				CRuleEventMgr::GetInst()->AddRule(ruleevent);
				//and on
				while (andlist.size() != 0)
				{
					ruleevent._Thing = andlist.back();
					CRuleEventMgr::GetInst()->AddRule(ruleevent);
					andlist.pop_back();
				}

				list<CThing*>::iterator frontiter = front.begin();
				list<CThing*>::iterator backiter = back.begin();
				for (; frontiter != front.end(); ++frontiter)
				{
					for (backiter = back.begin(); backiter != back.end(); ++backiter)
					{
						CThing* F = *frontiter;
						CThing* B = *backiter;
						if (F->getText() == B->getText())
						{
							SentenceInfo Info = {};
							Info.first = *frontiter;
							Info.second = *backiter;
							m_listSentence.push_front(Info);
						}
						else
						{
							SentenceInfo Info = {};
							Info.first = *frontiter;
							Info.second = *backiter;
							m_listSentence.push_back(Info);
						}
					}
				}
			}
			front.clear();
			back.clear();
		}
	}
}