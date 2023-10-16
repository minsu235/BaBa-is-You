#include "pch.h"
#include "CMenu.h"

#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CImage.h"

CMenu::CMenu(wstring _name)
	: m_pImage(nullptr)
	, m_iTransparency(255)
	, m_bGrid(false)
	, m_Grid(false)
{
	if (_name == L"ESCGrid")
	{
		m_Grid = true;
		if (CSceneMgr::GetInst()->getGrid())
		{
			m_pImage = CResMgr::GetInst()->LoadImg(_name + L"On", L"image\\UI\\" + _name + L"On.bmp");
			m_bGrid = true;
		}
		else
		{
			m_pImage = CResMgr::GetInst()->LoadImg(_name + L"Off", L"image\\UI\\" + _name + L"Off.bmp");
			m_bGrid = false;
		}			
	}
	else
		m_pImage = CResMgr::GetInst()->LoadImg(_name, L"image\\UI\\" + _name + L".bmp");

	if (_name == L"ESCBackground")
		m_iTransparency = 155;
}

CMenu::~CMenu()
{
}

void CMenu::render(HDC _dc)
{
	Vec2 vPos = GetPos();

	BLENDFUNCTION tFunc = {};
	tFunc.BlendOp = AC_SRC_OVER;
	tFunc.BlendFlags = 0;
	tFunc.AlphaFormat = AC_SRC_ALPHA;
	tFunc.SourceConstantAlpha = m_iTransparency;

	AlphaBlend(_dc, (int)vPos.x - m_pImage->GetWidth() / 2
		, (int)vPos.y - m_pImage->GetHeight() / 2
		, m_pImage->GetWidth(), m_pImage->GetHeight()
		, m_pImage->GetDC(), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), tFunc);

	CObject::render(_dc);
}

void CMenu::tick()
{
	if (m_Grid)
	{
		if (m_bGrid != CSceneMgr::GetInst()->getGrid())
		{
			m_bGrid = CSceneMgr::GetInst()->getGrid();
			if (m_bGrid)
			{
				m_pImage = CResMgr::GetInst()->LoadImg(L"ESCGridOn", L"image\\UI\\ESCGridOn.bmp");
			}
			else
			{
				m_pImage = CResMgr::GetInst()->LoadImg(L"ESCGridOff", L"image\\UI\\ESCGridOff.bmp");
			}
		}
	}

	CObject::tick();
}
