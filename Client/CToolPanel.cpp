#include "pch.h"
#include "CToolPanel.h"

#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CImage.h"

CToolPanel::CToolPanel()
	: m_pImage(nullptr)
	, m_pImage2(nullptr)
	, m_Info(cursorInfo(Thingtype::END))
	, m_dir(DIR::RIGHT)
{
	m_pImage = CResMgr::GetInst()->LoadImg(L"background", L"image\\Tool\\background.bmp");
	CResMgr::GetInst()->LoadImg(L"KeyUI_UP", L"image\\UI\\KeyUI_UP.bmp");
	m_pImage2 = CResMgr::GetInst()->LoadImg(L"KeyUI_RIGHT", L"image\\UI\\KeyUI_RIGHT.bmp");
	CResMgr::GetInst()->LoadImg(L"KeyUI_DOWN", L"image\\UI\\KeyUI_DOWN.bmp");
	CResMgr::GetInst()->LoadImg(L"KeyUI_LEFT", L"image\\UI\\KeyUI_LEFT.bmp");
}

CToolPanel::~CToolPanel()
{
}

void CToolPanel::tick()
{
	if (IsTap(KEY_TYPE::W))
	{
		if(m_cursor.y != 0)
			m_cursor.y -= 1;
	}
	else if (IsTap(KEY_TYPE::S))
	{
		if (m_cursor.y != 4)
			m_cursor.y += 1;
	}
	else if (IsTap(KEY_TYPE::A))
	{
		if (m_cursor.x != 0)
			m_cursor.x -= 1;
	}
	else if (IsTap(KEY_TYPE::D))
	{
		if (m_cursor.x != 9)
			m_cursor.x += 1;
	}
	else if (IsTap(KEY_TYPE::UP))
	{
		m_dir = DIR::UP;
		m_pImage2 = CResMgr::GetInst()->FindImg(L"KeyUI_UP");
	}
	else if (IsTap(KEY_TYPE::DOWN))
	{
		m_dir = DIR::DOWN;
		m_pImage2 = CResMgr::GetInst()->FindImg(L"KeyUI_DOWN");
	}
	else if (IsTap(KEY_TYPE::LEFT))
	{
		m_dir = DIR::LEFT;
		m_pImage2 = CResMgr::GetInst()->FindImg(L"KeyUI_LEFT");
	}
	else if (IsTap(KEY_TYPE::RIGHT))
	{
		m_dir = DIR::RIGHT;
		m_pImage2 = CResMgr::GetInst()->FindImg(L"KeyUI_RIGHT");
	}

	if (m_cursor == Vec2(1, 0))
		m_Info = cursorInfo(Thingtype::BABA);
	else if (m_cursor == Vec2(2, 0))
		m_Info = cursorInfo(Thingtype::DOOR);
	else if (m_cursor == Vec2(3, 0))
		m_Info = cursorInfo(Thingtype::KEY);
	else if (m_cursor == Vec2(4, 0))
		m_Info = cursorInfo(Thingtype::FLAG);
	else if (m_cursor == Vec2(5, 0))
		m_Info = cursorInfo(Thingtype::ROCK);
	else if (m_cursor == Vec2(6, 0))
		m_Info = cursorInfo(Thingtype::WALL);
	else if (m_cursor == Vec2(7, 0))
		m_Info = cursorInfo(Thingtype::SKULL);
	else if (m_cursor == Vec2(8, 0))
		m_Info = cursorInfo(Thingtype::WATER);
	else if (m_cursor == Vec2(9, 0))
		m_Info = cursorInfo(Thingtype::LAVA);
	else if (m_cursor == Vec2(0, 1))
		m_Info = cursorInfo(Thingtype::HEDGE);
	else if (m_cursor == Vec2(1, 1))
		m_Info = cursorInfo(Thingtype::GRASS);
	else if (m_cursor == Vec2(2, 1))
		m_Info = cursorInfo(Thingtype::TILE);
	else if (m_cursor == Vec2(3, 1))
		m_Info = cursorInfo(Thingtype::FLOWER);
	else if (m_cursor == Vec2(0, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::IS);
	else if (m_cursor == Vec2(1, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::AND);
	else if (m_cursor == Vec2(2, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::BABA);
	else if (m_cursor == Vec2(3, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::YOU);
	else if (m_cursor == Vec2(4, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::KEY);
	else if (m_cursor == Vec2(5, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::OPEN);
	else if (m_cursor == Vec2(6, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::DOOR);
	else if (m_cursor == Vec2(7, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::SHUT);
	else if (m_cursor == Vec2(8, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::FLAG);
	else if (m_cursor == Vec2(9, 2))
		m_Info = cursorInfo(Thingtype::TEXT, Text::WIN);
	else if (m_cursor == Vec2(0, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::ROCK);
	else if (m_cursor == Vec2(1, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::PUSH);
	else if (m_cursor == Vec2(2, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::SKULL);
	else if (m_cursor == Vec2(3, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::DEFEAT);
	else if (m_cursor == Vec2(4, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::WALL);
	else if (m_cursor == Vec2(5, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::STOP);
	else if (m_cursor == Vec2(6, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::WATER);
	else if (m_cursor == Vec2(7, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::SINK);
	else if (m_cursor == Vec2(8, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::HEDGE);
	else if (m_cursor == Vec2(9, 3))
		m_Info = cursorInfo(Thingtype::TEXT, Text::LAVA);
	else if (m_cursor == Vec2(0, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::GRASS);
	else if (m_cursor == Vec2(1, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::FLOWER);
	else if (m_cursor == Vec2(2, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::MELT);
	else if (m_cursor == Vec2(3, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::HOT);
	else if (m_cursor == Vec2(4, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::TEXT);
	else if (m_cursor == Vec2(5, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::FLOAT);
	else if (m_cursor == Vec2(6, 4))
		m_Info = cursorInfo(Thingtype::TEXT, Text::MOVE);
	else
		m_Info = cursorInfo(Thingtype::END);

	CUI::tick();
}

void CToolPanel::render(HDC _dc)
{
	Vec2 vPos = GetPos();

	BLENDFUNCTION tFunc = {};
	tFunc.BlendOp = AC_SRC_OVER;
	tFunc.BlendFlags = 0;
	tFunc.AlphaFormat = AC_SRC_ALPHA;
	tFunc.SourceConstantAlpha = 255;

	AlphaBlend(_dc, (int)vPos.x
		, (int)vPos.y
		, m_pImage->GetWidth(), m_pImage->GetHeight()
		, m_pImage->GetDC(), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), tFunc);

	tSelectPen pen(_dc, PEN_COLOR::RED);
	tSelectBrush Brush(_dc, BRUSH_COLOR::HOLLOW);

	Rectangle(_dc
		, (int)(vPos.x + 11 + m_cursor.x * TILE_SIZE)
		, (int)(vPos.y + 12 + m_cursor.y * TILE_SIZE)
		, (int)(vPos.x + 11 + m_cursor.x * TILE_SIZE + 32)
		, (int)(vPos.y + 12 + m_cursor.y * TILE_SIZE + 32));

	AlphaBlend(_dc, (int)vPos.x + 11
		, (int)vPos.y + 12
		, m_pImage2->GetWidth(), m_pImage2->GetHeight()
		, m_pImage2->GetDC(), 0, 0, m_pImage2->GetWidth(), m_pImage2->GetHeight(), tFunc);

	CUI::render(_dc);
}
