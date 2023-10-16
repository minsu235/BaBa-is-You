#include "pch.h"
#include "CToolTile.h"

#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

#include "CThing.h"
#include "CImage.h"
#include "CScene_Tool.h"
#include "CToolPanel.h"

CToolTile::CToolTile()
	: m_eThingtype(Thingtype::END)
	, m_eText(Text::END)
	, m_eDir(DIR::RIGHT)
	, m_bMouseOn(false)
{
}

CToolTile::~CToolTile()
{
}

void CToolTile::CheckMouseOn()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vPos = GetPos();

	if (vPos.x <= vMousePos.x && vMousePos.x <= vPos.x + 32
		&& vPos.y <= vMousePos.y && vMousePos.y <= vPos.y + 32)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CToolTile::tick()
{
	CheckMouseOn();
	if (m_bMouseOn)
	{
		if (IsPressed(KEY_TYPE::LBTN))
		{
			CScene_Tool* pScene = (CScene_Tool*)CSceneMgr::GetInst()->GetCurScene();
			CToolPanel* toolpanel = pScene->getToolpanel();
			cursorInfo info = toolpanel->getInfo();
			m_eThingtype = info._thingtype;
			m_eText = info._text;
			m_eDir = toolpanel->getDIR();
		}
	}

	CObject::tick();
}

void CToolTile::render(HDC _dc)
{
	tSelectPen pen(_dc);
	pen.SetPenColor(PEN_COLOR::DEFAULT);

	tSelectBrush brush(_dc, BRUSH_COLOR::HOLLOW);

	Vec2 vPos = GetPos();

	Rectangle(_dc, (int)(vPos.x)
		, (int)(vPos.y)
		, (int)(vPos.x + TILE_SIZE)
		, (int)(vPos.y + TILE_SIZE));

	CImage* pObjAtlas = nullptr;

	if (m_eThingtype == Thingtype::TEXT)
	{
		wstring a = L"image\\Text\\" + Text_arr[(UINT)m_eText] + L"Text.bmp";
		pObjAtlas = CResMgr::GetInst()->LoadImg(Text_arr[(UINT)m_eText] + L"TextAtlas", a);
		Vec2 vPos = GetPos();
		BLENDFUNCTION tFunc = {};
		tFunc.BlendOp = AC_SRC_OVER;
		tFunc.BlendFlags = 0;
		tFunc.AlphaFormat = AC_SRC_ALPHA;
		tFunc.SourceConstantAlpha = 255;
		AlphaBlend(_dc, (int)vPos.x
			, (int)vPos.y
			, 32, 32
			, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
	}
	else if (m_eThingtype != Thingtype::END)
	{
		pObjAtlas = CResMgr::GetInst()->LoadImg(Thing_arr[(UINT)m_eThingtype] + L"Atlas", L"image\\Obj\\" + Thing_arr[(UINT)m_eThingtype] + L".bmp");
		Vec2 vPos = GetPos();
		BLENDFUNCTION tFunc = {};
		tFunc.BlendOp = AC_SRC_OVER;
		tFunc.BlendFlags = 0;
		tFunc.AlphaFormat = AC_SRC_ALPHA;
		tFunc.SourceConstantAlpha = 255;
		if ((UINT)Thingtype::BABA == (UINT)m_eThingtype)
		{
			switch (m_eDir)
			{
			case DIR::UP:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 128, 0, 32, 32, tFunc);
				break;
			case DIR::DOWN:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 384, 0, 32, 32, tFunc);
				break;
			case DIR::LEFT:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 256, 0, 32, 32, tFunc);
				break;
			case DIR::RIGHT:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
				break;
			}
		}
		else if ((UINT)Thingtype::SKULL == (UINT)m_eThingtype)
		{
			switch (m_eDir)
			{
			case DIR::UP:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 32, 0, 32, 32, tFunc);
				break;
			case DIR::DOWN:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 96, 0, 32, 32, tFunc);
				break;
			case DIR::LEFT:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 64, 0, 32, 32, tFunc);
				break;
			case DIR::RIGHT:
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
				break;
			}
		}
		else
		{
			AlphaBlend(_dc, (int)vPos.x
				, (int)vPos.y
				, 32, 32
				, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
		}
	}

	if (m_bMouseOn)
	{
		CScene_Tool* pScene = (CScene_Tool*)CSceneMgr::GetInst()->GetCurScene();
		CToolPanel* toolpanel = pScene->getToolpanel();
		cursorInfo info = toolpanel->getInfo();

		if (info._thingtype == Thingtype::TEXT)
		{
			wstring a = L"image\\Text\\" + Text_arr[(UINT)info._text] + L"Text.bmp";
			pObjAtlas = CResMgr::GetInst()->LoadImg(Text_arr[(UINT)info._text] + L"TextAtlas", a);
			Vec2 vPos = GetPos();
			BLENDFUNCTION tFunc = {};
			tFunc.BlendOp = AC_SRC_OVER;
			tFunc.BlendFlags = 0;
			tFunc.AlphaFormat = AC_SRC_ALPHA;
			tFunc.SourceConstantAlpha = 123;
			AlphaBlend(_dc, (int)vPos.x
				, (int)vPos.y
				, 32, 32
				, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
		}
		else if (info._thingtype != Thingtype::END)
		{
			pObjAtlas = CResMgr::GetInst()->LoadImg(Thing_arr[(UINT)info._thingtype] + L"Atlas", L"image\\Obj\\" + Thing_arr[(UINT)info._thingtype] + L".bmp");
			Vec2 vPos = GetPos();
			BLENDFUNCTION tFunc = {};
			tFunc.BlendOp = AC_SRC_OVER;
			tFunc.BlendFlags = 0;
			tFunc.AlphaFormat = AC_SRC_ALPHA;
			tFunc.SourceConstantAlpha = 123;
			if ((UINT)Thingtype::BABA == (UINT)info._thingtype)
			{
				switch (toolpanel->getDIR())
				{
				case DIR::UP:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 128, 0, 32, 32, tFunc);
					break;
				case DIR::DOWN:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 384, 0, 32, 32, tFunc);
					break;
				case DIR::LEFT:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 256, 0, 32, 32, tFunc);
					break;
				case DIR::RIGHT:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
					break;
				}
			}
			else if ((UINT)Thingtype::SKULL == (UINT)info._thingtype)
			{
				switch (toolpanel->getDIR())
				{
				case DIR::UP:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 32, 0, 32, 32, tFunc);
					break;
				case DIR::DOWN:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 96, 0, 32, 32, tFunc);
					break;
				case DIR::LEFT:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 64, 0, 32, 32, tFunc);
					break;
				case DIR::RIGHT:
					AlphaBlend(_dc, (int)vPos.x
						, (int)vPos.y
						, 32, 32
						, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
					break;
				}
			}
			else
			{
				AlphaBlend(_dc, (int)vPos.x
					, (int)vPos.y
					, 32, 32
					, pObjAtlas->GetDC(), 0, 0, 32, 32, tFunc);
			}
		}
	}
}