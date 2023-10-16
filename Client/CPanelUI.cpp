#include "pch.h"
#include "CPanelUI.h"

#include "CKeyMgr.h"

CPanelUI::CPanelUI()
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::tick()
{
	//if (IsLBtnDown())
	//{
	//	// ĸ�� �����ǰ� ���� ���콺 �������� ���̰��� ����
	//	Vec2 vCurMousePos = CKeyMgr::GetInst()->GetMousePos();
	//	Vec2 vMouseDiff = vCurMousePos - m_vCapturePos;
	//	m_vCapturePos = vCurMousePos; // ���� ���콺 ��ġ�� ĸó����� ����

	//	// ���̰� ��ŭ UI �����ǿ� �ݿ�
	//	Vec2 vUIPos = GetPos();
	//	vUIPos += vMouseDiff;
	//	SetPos(vUIPos);
	//}

	// ���� ��ġ ����
	CUI::tick();
}

void CPanelUI::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	tSelectPen pen(_dc, PEN_COLOR::BLUE);
	tSelectBrush Brush(_dc, BRUSH_COLOR::BLACK);

	Vec2 vFinalPos = GetFinalPos();

	Rectangle(_dc
		, (int)(vFinalPos.x)
		, (int)(vFinalPos.y)
		, (int)(vFinalPos.x + vScale.x)
		, (int)(vFinalPos.y + vScale.y));

	CUI::render(_dc);
}

void CPanelUI::LBtnDown()
{	
	CUI::LBtnDown();

	m_vCapturePos = CKeyMgr::GetInst()->GetMousePos();
}

void CPanelUI::LBtnClicked()
{
	int a = 0;
}