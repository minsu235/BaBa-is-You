#include "pch.h"
#include "CThing.h"

#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "CScene_Stage.h"
#include "CRuleEventMgr.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CSound.h"

#include "CMoveEffect.h"
#include "CWinTwinkle.h"
#include "CWinSpark.h"

CThing::CThing(Thingtype _thingtype, DIR _dir)
	: m_eMoveDir(DIR::NONE)
	, m_eFocusDir(DIR::RIGHT)
	, m_bFocusChange(false)
	, m_uMoveCount(1)
	, m_fMoveTime(0)
	, m_uState(0)
	, m_eThingtype(Thingtype::END)
	, m_iFloat(0)
	, m_fFloatTime(0)
	, m_fWinTime(0)
	//unit
	, m_eUnitType(UnitType::Default)
	, m_eStickDir(StickDir::END)
	, m_isChange(false)
	, m_Changecheck(true)
	//text
	, m_eText(Text::END)
	, m_eTexttype(Texttype::NONE)
	, m_isON(false)
	, m_nextonoff(false)
	, m_bonoffchange(false)
{
	if ((UINT)Thingtype::BABA <= (UINT)_thingtype && (UINT)_thingtype <= (UINT)Thingtype::SKULL)
		m_eUnitType = UnitType::FourDir;
	else if ((UINT)Thingtype::WALL <= (UINT)_thingtype && (UINT)_thingtype <= (UINT)Thingtype::WATER)
		m_eUnitType = UnitType::Stick;

	m_eThingtype = _thingtype;
	m_eFocusDir = _dir;
	AddComponent(new CAnimator);

	if ((UINT)_thingtype >= (UINT)Thingtype::END)
		return;

	CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(Thing_arr[(UINT)_thingtype] + L"Atlas", L"image\\Obj\\" + Thing_arr[(UINT)_thingtype] + L".bmp");

	switch (m_eUnitType)
	{
	case UnitType::Default:
		GetAnimator()->CreateAnimation(L"stop", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		GetAnimator()->Play(L"stop", true);
		break;
	case UnitType::FourDir:
		if (_thingtype == Thingtype::BABA)
		{
			GetAnimator()->CreateAnimation(L"Walk_Up1", pObjAtlas, Vec2(128.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up2", pObjAtlas, Vec2(160.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up3", pObjAtlas, Vec2(192.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up4", pObjAtlas, Vec2(224.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down1", pObjAtlas, Vec2(384.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down2", pObjAtlas, Vec2(416.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down3", pObjAtlas, Vec2(448.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down4", pObjAtlas, Vec2(480.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left1", pObjAtlas, Vec2(256.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left2", pObjAtlas, Vec2(288.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left3", pObjAtlas, Vec2(320.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left4", pObjAtlas, Vec2(352.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right1", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right2", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right3", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right4", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		}
		else
		{
			GetAnimator()->CreateAnimation(L"Walk_Up1", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up2", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up3", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Up4", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down1", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down2", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down3", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Down4", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left1", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left2", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left3", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Left4", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right1", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right2", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right3", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->CreateAnimation(L"Walk_Right4", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		}
		switch (m_eFocusDir)
		{
		case DIR::RIGHT:
			GetAnimator()->Play(L"Walk_Right1", true);
			break;
		case DIR::DOWN:
			GetAnimator()->Play(L"Walk_Down1", true);
			break;
		case DIR::LEFT:
			GetAnimator()->Play(L"Walk_Left1", true);
			break;
		case DIR::UP:
			GetAnimator()->Play(L"Walk_Up1", true);
			break;
		case DIR::NONE:
			GetAnimator()->Play(L"Walk_Right1", true);
			break;
		}
		break;
	case UnitType::Stick:
		for (size_t i = 0; i < (size_t)StickDir::END; ++i)
			GetAnimator()->CreateAnimation(StickDir_arr[i], pObjAtlas, Vec2(32.f * (float)i, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		break;
	}
}

CThing::CThing(Text _text)
	: m_eMoveDir(DIR::NONE)
	, m_eFocusDir(DIR::RIGHT)
	, m_bFocusChange(false)
	, m_uMoveCount(1)
	, m_fMoveTime(0)
	, m_uState(0)
	, m_eThingtype(Thingtype::END)
	, m_iFloat(0)
	, m_fFloatTime(0)
	, m_fWinTime(0)
	//unit
	, m_eUnitType(UnitType::Default)
	, m_eStickDir(StickDir::END)
	, m_isChange(false)
	, m_Changecheck(true)
	//text
	, m_eText(_text)
	, m_eTexttype(Texttype::OBJECT)
	, m_isON(false)
	, m_nextonoff(false)
	, m_bonoffchange(false)
{
	if (_text == Text::IS)
		m_eTexttype = Texttype::IS;
	else if (_text == Text::AND)
		m_eTexttype = Texttype::AND;
	else if (Text::YOU <= _text && _text < Text::END)
		m_eTexttype = Texttype::PROPERTY;

	addState(RULE::PUSH);
	AddComponent(new CAnimator);
	wstring a = L"image\\Text\\" + Text_arr[(UINT)m_eText] + L"Text.bmp";
	CImage* pTextAtlas = CResMgr::GetInst()->LoadImg(Text_arr[(UINT)m_eText] + L"TextAtlas", a);
	GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"ON", pTextAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
	GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"OFF", pTextAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
	GetAnimator()->Play(Text_arr[(UINT)m_eText] + L"OFF", true);
}

CThing::~CThing()
{
}

void CThing::tick()
{
	switch (m_eMoveDir)
	{
	case DIR::UP:
	{
		if (m_fMoveTime < 0.1)
		{
			m_vOriginPos.y -= 10 * TILE_SIZE * DT;
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).y - TILE_SIZE > m_vOriginPos.y)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x - 1, (UINT)m_vMatrix.y);
			m_fMoveTime += DT;
		}
		else
		{
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).y - TILE_SIZE < m_vOriginPos.y)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x - 1, (UINT)m_vMatrix.y);
			m_vMatrix -= Vec2(1, 0);
			m_eMoveDir = DIR::NONE;
			m_fMoveTime = 0;
		}
	}
	break;
	case DIR::DOWN:
	{
		if (m_fMoveTime < 0.1)
		{
			m_vOriginPos.y += 10 * TILE_SIZE * DT;
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).y + TILE_SIZE < m_vOriginPos.y)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x + 1, (UINT)m_vMatrix.y);
			m_fMoveTime += DT;
		}
		else
		{
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).y + TILE_SIZE > m_vOriginPos.y)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x + 1, (UINT)m_vMatrix.y);
			m_vMatrix += Vec2(1, 0);
			m_eMoveDir = DIR::NONE;
			m_fMoveTime = 0;
		}
	}
	break;
	case DIR::LEFT:
	{
		if (m_fMoveTime < 0.1)
		{
			m_vOriginPos.x -= 10 * TILE_SIZE * DT;
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).x - TILE_SIZE > m_vOriginPos.x)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y - 1);
			m_fMoveTime += DT;
		}
		else
		{
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).x - TILE_SIZE < m_vOriginPos.x)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y - 1);
			m_vMatrix -= Vec2(0, 1);
			m_eMoveDir = DIR::NONE;
			m_fMoveTime = 0;
		}
	}

	break;
	case DIR::RIGHT:
	{
		if (m_fMoveTime < 0.1)
		{
			m_vOriginPos.x += 10 * TILE_SIZE * DT;
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).x + TILE_SIZE < m_vOriginPos.x)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y + 1);
			m_fMoveTime += DT;
		}
		else
		{
			if (tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y).y - TILE_SIZE > m_vOriginPos.y)
				m_vOriginPos = tilePos((UINT)m_vMatrix.x, (UINT)m_vMatrix.y + 1);
			m_vMatrix += Vec2(0, 1);
			m_eMoveDir = DIR::NONE;
			m_fMoveTime = 0;
		}
	}
	break;
	}

	Vec2 Pos = m_vOriginPos;

	if (isgetState(RULE::FLOAT))
	{
		m_iFloat = (int)(5 * sin(2 * m_fFloatTime));
		m_fFloatTime += DT;
		Pos = Vec2(m_vOriginPos.x, m_vOriginPos.y + m_iFloat);
	}

	SetPos(Pos);

	if (m_eThingtype != Thingtype::TEXT)
	{
		if (m_bFocusChange)
		{
			if (m_eUnitType == UnitType::FourDir)
			{
				switch (m_eFocusDir)
				{
				case DIR::UP:
					GetAnimator()->Play(L"Walk_Up" + to_wstring(m_uMoveCount), true);
					break;
				case DIR::DOWN:
					GetAnimator()->Play(L"Walk_Down" + to_wstring(m_uMoveCount), true);
					break;
				case DIR::LEFT:
					GetAnimator()->Play(L"Walk_Left" + to_wstring(m_uMoveCount), true);
					break;
				case DIR::RIGHT:
					GetAnimator()->Play(L"Walk_Right" + to_wstring(m_uMoveCount), true);
					break;
				case DIR::NONE:
					GetAnimator()->Play(L"Walk_Right" + to_wstring(m_uMoveCount), true);
					break;
				}
			}
			m_bFocusChange = false;
		}

		if (m_eUnitType == UnitType::Stick)
		{
			if (m_eMoveDir == DIR::NONE)
			{
				StickDirCheck();
			}
		}
	}
	else
	{
		if (m_bonoffchange)
		{
			if (m_fMoveTime == 0)
			{
				if (m_isON == false && m_nextonoff == true)
				{
					CScene_Stage* pCurscene = dynamic_cast<CScene_Stage*>(CSceneMgr::GetInst()->GetCurScene());
					if (pCurscene != nullptr && pCurscene->getActCount() != 0)
						pCurscene->textsound();
					GetAnimator()->Play(Text_arr[(UINT)m_eText] + L"ON", true);
					m_isON = true;
					m_bonoffchange = false;
				}
				else if (m_isON == true && m_nextonoff == false)
				{
					GetAnimator()->Play(Text_arr[(UINT)m_eText] + L"OFF", true);
					m_isON = false;
					m_bonoffchange = false;
				}
				else
					m_bonoffchange = false;
			}
		}

		if (!isgetState(RULE::PUSH))
			addState(RULE::PUSH);
	}

	if (isgetState(RULE::WIN))
	{
		if (m_fWinTime > 0.8)
		{
			CWinTwinkle* pWinEffect = new CWinTwinkle(GetPos());
			tEventInfo info = {};
			info.eType = EVENT_TYPE::CREATE_OBJECT;
			info.first = (DWORD_PTR)pWinEffect;
			info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
			CEventMgr::GetInst()->AddEvent(info);
			m_fWinTime = 0;
		}
		else
			m_fWinTime += DT;
	}

	CObject::tick();
}

void CThing::thingMove(DIR _dir)
{
	if (m_eUnitType == UnitType::FourDir)
	{
		if (m_eMoveDir == DIR::NONE)
		{
			++m_uMoveCount;
			if (m_uMoveCount > 4)
				m_uMoveCount = 1;

			switch (_dir)
			{
			case DIR::UP:
				GetAnimator()->Play(L"Walk_Up" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::DOWN:
				GetAnimator()->Play(L"Walk_Down" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::LEFT:
				GetAnimator()->Play(L"Walk_Left" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::RIGHT:
				GetAnimator()->Play(L"Walk_Right" + to_wstring(m_uMoveCount), true);
				break;
			}

			m_eMoveDir = _dir;
			m_eFocusDir = _dir;
			CMoveEffect* pMoveEffect = new CMoveEffect(m_eThingtype, _dir, GetPos());
			tEventInfo info = {};
			info.eType = EVENT_TYPE::CREATE_OBJECT;
			info.first = (DWORD_PTR)pMoveEffect;
			info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
			CEventMgr::GetInst()->AddEvent(info);
		}
	}
	else
	{
		if (m_eMoveDir == DIR::NONE)
		{
			m_eMoveDir = _dir;
			m_eFocusDir = _dir;
			CMoveEffect* pMoveEffect = new CMoveEffect(m_eThingtype, _dir, GetPos());
			tEventInfo info = {};
			info.eType = EVENT_TYPE::CREATE_OBJECT;
			info.first = (DWORD_PTR)pMoveEffect;
			info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
			CEventMgr::GetInst()->AddEvent(info);
		}
	}
}

void CThing::thingReverseMove(DIR _movedir, DIR _focusdir)
{
	if (m_eUnitType == UnitType::FourDir)
	{
		if (m_eMoveDir == DIR::NONE)
		{
			--m_uMoveCount;
			if (m_uMoveCount < 1)
				m_uMoveCount = 4;

			switch (_focusdir)
			{
			case DIR::UP:
				GetAnimator()->Play(L"Walk_Up" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::DOWN:
				GetAnimator()->Play(L"Walk_Down" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::LEFT:
				GetAnimator()->Play(L"Walk_Left" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::RIGHT:
				GetAnimator()->Play(L"Walk_Right" + to_wstring(m_uMoveCount), true);
				break;
			case DIR::NONE:
				GetAnimator()->Play(L"Walk_Right" + to_wstring(m_uMoveCount), true);
				break;
			}

			m_eMoveDir = _movedir;
			m_eFocusDir = _focusdir;
		}
	}
	else
	{
		if (m_eMoveDir == DIR::NONE)
		{
			m_eMoveDir = _movedir;
			m_eFocusDir = _focusdir;
		}
	}
}

void CThing::Winner()
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJECT;
	info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;
	
	for (int i = 0; i < 10; ++i)
	{
		CWinSpark* pWinspark = new CWinSpark(GetPos());
		info.first = (DWORD_PTR)pWinspark;
		CEventMgr::GetInst()->AddEvent(info);
	}
}

void CThing::StickDirCheck()
{
	CScene_Stage* pCurScene = (CScene_Stage*)CSceneMgr::GetInst()->GetCurScene();
	multimap<int, CThing*>& mapMatrix = pCurScene->getMultimap();
	multimap<int, CThing*>::iterator iter = mapMatrix.begin();
	Vec2 pos = getMatrix();
	Vec2 Size = pCurScene->getSize();
	multimap<int, CThing*>::iterator _iter = mapMatrix.find((int)(pos.x * Size.x + pos.y));
	size_t iCount = 0;
	bool Up = false;
	bool Down = false;
	bool Left = false;
	bool Right = false;

	if (_iter != mapMatrix.end())
	{
		iter = mapMatrix.find((int)((pos.x - 1) * Size.x + pos.y));
		iCount = mapMatrix.count((int)((pos.x - 1) * Size.x + pos.y));
		for (size_t i = 0; i < iCount; ++i, ++iter)
		{
			CThing* Obj = iter->second;
			if (Obj->getThingtype() == getThingtype())
			{
				Up = true;
				break;
			}
		}

		iter = mapMatrix.find((int)((pos.x + 1) * Size.x + pos.y));
		iCount = mapMatrix.count((int)((pos.x + 1) * Size.x + pos.y));
		for (size_t i = 0; i < iCount; ++i, ++iter)
		{
			CThing* Obj = iter->second;
			if (Obj->getThingtype() == getThingtype())
			{
				Down = true;
				break;
			}
		}

		iter = mapMatrix.find((int)(pos.x * Size.x + pos.y - 1));
		iCount = mapMatrix.count((int)(pos.x * Size.x + pos.y - 1));
		for (size_t i = 0; i < iCount; ++i, ++iter)
		{
			CThing* Obj = iter->second;
			if (Obj->getThingtype() == getThingtype())
			{
				Left = true;
				break;
			}
		}

		iter = mapMatrix.find((int)(pos.x * Size.x + pos.y + 1));
		iCount = mapMatrix.count((int)(pos.x * Size.x + pos.y + 1));
		for (size_t i = 0; i < iCount; ++i, ++iter)
		{
			CThing* Obj = iter->second;
			if (Obj->getThingtype() == getThingtype())
			{
				Right = true;
				break;
			}
		}
	}

	wstring dir = {};
	if (Up)
		dir += L"U";
	if (Down)
		dir += L"D";
	if (Left)
		dir += L"L";
	if (Right)
		dir += L"R";
	if (!(Up || Down || Left || Right))
	{
		dir = L"N";
	}

	int index = 0;

	for (int c = 0; c < (int)StickDir::END; ++c)
	{
		if (StickDir_arr[c] == dir)
		{
			index = c;
			break;
		}
	}

	if ((StickDir)index != m_eStickDir)
	{
		GetAnimator()->Play(dir, true);
	}
	m_eStickDir = (StickDir)index;
}

void CThing::ChangeUnit(Thingtype _thingtype)
{
	//행동할때만 변해야함
	m_isChange = false;
	m_Changecheck = false;

	if (_thingtype != Thingtype::TEXT)
	{
		m_eThingtype = _thingtype;
		m_eUnitType = UnitType::Default;
		m_eStickDir = StickDir::END;
		if ((UINT)Thingtype::BABA <= (UINT)_thingtype && (UINT)_thingtype <= (UINT)Thingtype::SKULL)
			m_eUnitType = UnitType::FourDir;
		else if ((UINT)Thingtype::WALL <= (UINT)_thingtype && (UINT)_thingtype <= (UINT)Thingtype::WATER)
			m_eUnitType = UnitType::Stick;
		m_eText = Text::END;
		m_eTexttype = Texttype::NONE;
		m_isON = false;
		m_nextonoff = false;
		m_bonoffchange = false;

		SubComponent(COMPONENT_TYPE::ANIMATOR);
		AddComponent(new CAnimator);

		if ((UINT)_thingtype >= (UINT)Thingtype::END)
			return;

		CImage* pObjAtlas = CResMgr::GetInst()->LoadImg(Thing_arr[(UINT)_thingtype] + L"Atlas", L"image\\Obj\\" + Thing_arr[(UINT)_thingtype] + L".bmp");

		switch (m_eUnitType)
		{
		case UnitType::Default:
			GetAnimator()->CreateAnimation(L"stop", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			GetAnimator()->Play(L"stop", true);
			break;
		case UnitType::FourDir:
			if (_thingtype == Thingtype::BABA)
			{
				GetAnimator()->CreateAnimation(L"Walk_Up1", pObjAtlas, Vec2(128.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up2", pObjAtlas, Vec2(160.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up3", pObjAtlas, Vec2(192.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up4", pObjAtlas, Vec2(224.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down1", pObjAtlas, Vec2(384.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down2", pObjAtlas, Vec2(416.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down3", pObjAtlas, Vec2(448.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down4", pObjAtlas, Vec2(480.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left1", pObjAtlas, Vec2(256.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left2", pObjAtlas, Vec2(288.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left3", pObjAtlas, Vec2(320.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left4", pObjAtlas, Vec2(352.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right1", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right2", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right3", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right4", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			}
			else
			{
				GetAnimator()->CreateAnimation(L"Walk_Up1", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up2", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up3", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Up4", pObjAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down1", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down2", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down3", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Down4", pObjAtlas, Vec2(96.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left1", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left2", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left3", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Left4", pObjAtlas, Vec2(64.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right1", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right2", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right3", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
				GetAnimator()->CreateAnimation(L"Walk_Right4", pObjAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			}
			switch (m_eFocusDir)
			{
			case DIR::RIGHT:
				GetAnimator()->Play(L"Walk_Right1", true);
				break;
			case DIR::DOWN:
				GetAnimator()->Play(L"Walk_Down1", true);
				break;
			case DIR::LEFT:
				GetAnimator()->Play(L"Walk_Left1", true);
				break;
			case DIR::UP:
				GetAnimator()->Play(L"Walk_Up1", true);
				break;
			case DIR::NONE:
				GetAnimator()->Play(L"Walk_Right1", true);
				break;
			}
			break;
		case UnitType::Stick:
			for (size_t i = 0; i < (size_t)StickDir::END; ++i)
				GetAnimator()->CreateAnimation(StickDir_arr[i], pObjAtlas, Vec2(32.f * (float)i, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
			break;
		}

		tEventInfo info = {};
		info.eType = EVENT_TYPE::LAYER_CHANGE_OBJ;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}
	else
	{
		m_eText = (Text)m_eThingtype;
		m_eTexttype = Texttype::OBJECT;
		m_eThingtype = _thingtype;
		m_eUnitType = UnitType::Default;
		m_eStickDir = StickDir::END;

		addState(RULE::PUSH);
		SubComponent(COMPONENT_TYPE::ANIMATOR);
		AddComponent(new CAnimator);
		wstring a = L"image\\Text\\" + Text_arr[(UINT)m_eText] + L"Text.bmp";
		CImage* pTextAtlas = CResMgr::GetInst()->LoadImg(Text_arr[(UINT)m_eText] + L"TextAtlas", a);
		GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"ON", pTextAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"OFF", pTextAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		GetAnimator()->Play(Text_arr[(UINT)m_eText] + L"OFF", true);

		tEventInfo info = {};
		info.eType = EVENT_TYPE::LAYER_CHANGE_OBJ;
		info.first = (DWORD_PTR)this;
		CEventMgr::GetInst()->AddEvent(info);
	}

	for (UINT c = 0; c < (UINT)RULE::END; ++c)
		this->subState((RULE)c);


}

void CThing::ChangeUnit(Text _text)
{
	//행동할때만 변해야함
	m_isChange = false;
	m_Changecheck = false;

	m_eText = _text;
	m_eTexttype = Texttype::OBJECT;

	if (_text == Text::IS)
		m_eTexttype = Texttype::IS;
	else if (_text == Text::AND)
		m_eTexttype = Texttype::AND;
	else if (Text::YOU <= _text && _text < Text::END)
		m_eTexttype = Texttype::PROPERTY;

	m_eThingtype = Thingtype::TEXT;
	m_eUnitType = UnitType::Default;
	m_eStickDir = StickDir::END;

	addState(RULE::PUSH);
	SubComponent(COMPONENT_TYPE::ANIMATOR);
	AddComponent(new CAnimator);
	wstring a = L"image\\Text\\" + Text_arr[(UINT)m_eText] + L"Text.bmp";
	CImage* pTextAtlas = CResMgr::GetInst()->LoadImg(Text_arr[(UINT)m_eText] + L"TextAtlas", a);
	GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"ON", pTextAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
	GetAnimator()->CreateAnimation(Text_arr[(UINT)m_eText] + L"OFF", pTextAtlas, Vec2(32.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
	GetAnimator()->Play(Text_arr[(UINT)m_eText] + L"OFF", true);

	tEventInfo info = {};
	info.eType = EVENT_TYPE::LAYER_CHANGE_OBJ;
	info.first = (DWORD_PTR)this;
	CEventMgr::GetInst()->AddEvent(info);

for (UINT c = 0; c < (UINT)RULE::END; ++c)
	this->subState((RULE)c);
}
