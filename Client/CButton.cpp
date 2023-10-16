#include "pch.h"
#include "CButton.h"

#include "CSceneMgr.h"

CButton::CButton()
	: m_pFuncPointer(nullptr)
{

}

CButton::~CButton()
{
}

void CButton::tick()
{
	// ButtonUI 에서 할 일 작성


	CUI::tick();
}

void CButton::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	tSelectPen pen(_dc, PEN_COLOR::GREEN);
	tSelectBrush Brush(_dc, BRUSH_COLOR::DEFAULT);

	Vec2 vFinalPos = GetFinalPos();

	Rectangle(_dc
		, (int)(vFinalPos.x)
		, (int)(vFinalPos.y)
		, (int)(vFinalPos.x + vScale.x)
		, (int)(vFinalPos.y + vScale.y));

	CUI::render(_dc);
}

void CButton::LBtnClicked()
{
	if (nullptr != m_pFuncPointer)
		m_pFuncPointer();

	if (m_Delegate.IsValid())
	{
		(m_Delegate.pInst->*m_Delegate.pFunc)();
	}
}
