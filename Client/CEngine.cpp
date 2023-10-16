#include "pch.h"
#include "CEngine.h"

#include "CSceneMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CRuleEventMgr.h"

#include "resource.h"
#include "CImage.h"
#include "CSound.h"


CEngine::CEngine()
	: m_hMainWnd(0)
	, m_hDC(0)
	, m_ptResolution{}
	, m_pBackBuffer(nullptr)
	, m_arrPen{}
	, m_arrBrush{}
	, m_hMenu(0)
{

}

CEngine::~CEngine()
{
	ReleaseDC(m_hMainWnd, m_hDC);

	// 펜 삭제요청
	for (UINT i = 0; i < (UINT)PEN_COLOR::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}	

	// 브러쉬 삭제요청
	for (UINT i = 0; i < ((UINT)BRUSH_COLOR::END - 1); ++i)
	{
		DeleteObject(m_arrBrush[i]);
	}

	// 메뉴 삭제	
	DestroyMenu(m_hMenu);
}


void CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;

	m_hDC = GetDC(m_hMainWnd);

	// 윈도우 해상도 설정
	m_ptResolution.x = _iWidth;
	m_ptResolution.y = _iHeight;

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = {0, 0, (LONG)_iWidth , (LONG)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// 기본 펜 및 브러쉬 생성
	CreateDefaultGDIObject();

	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_CLIENT));

	// 이중 버퍼링
	// 윈도우 해상도와 동일한 크기의 비트맵을 생성
	m_pBackBuffer = CResMgr::GetInst()->CreateImg(L"BackBuffer", m_ptResolution.x, m_ptResolution.y);


	// Manager 초기화
	CPathMgr::GetInst()->init();
	
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();	

	CSoundMgr::GetInst()->init();
	CResMgr::GetInst()->init();

	CSceneMgr::GetInst()->init();
}

void CEngine::tick()
{	
	// ======
	// Update
	// ======
	// 매니저 업데이트
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();

	// Scene 업데이트
	CSceneMgr::GetInst()->tick(); 

	// UI 이벤트 검사
	CUIMgr::GetInst()->tick();

	// 충돌 검사
	CCollisionMgr::GetInst()->tick();


	// ======
	// 렌더링
	// ======

	// 버퍼 클리어 
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_pBackBuffer->GetDC(), m_arrBrush[(UINT)BRUSH_COLOR::DEFAULT]);
	Rectangle(m_pBackBuffer->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	SelectObject(m_pBackBuffer->GetDC(), hPrevBrush);

	// Scene Rendering
	CSceneMgr::GetInst()->render(m_pBackBuffer->GetDC());
	CTimeMgr::GetInst()->render(m_pBackBuffer->GetDC());

	// BackBuffer -> MainBuffer
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pBackBuffer->GetDC(), 0, 0, SRCCOPY);


	// EventMgr
	CRuleEventMgr::GetInst()->tick();
	CEventMgr::GetInst()->tick();
}

void CEngine::CreateDefaultGDIObject()
{
	// 자주 쓸 색상의 펜 생성
	m_arrPen[(UINT)PEN_COLOR::RED] = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_COLOR::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_COLOR::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_COLOR::BLACK] = CreatePen(PS_SOLID, 1, RGB(8, 8, 8));
	m_arrPen[(UINT)PEN_COLOR::DEFAULT] = CreatePen(PS_SOLID, 1, RGB(21, 24, 31));

	// 자주 쓸 Brush 생성
	m_arrBrush[(UINT)BRUSH_COLOR::BLACK] = CreateSolidBrush(RGB(8, 8, 8));
	m_arrBrush[(UINT)BRUSH_COLOR::DEFAULT] = CreateSolidBrush(RGB(21, 24, 31));
	m_arrBrush[(UINT)BRUSH_COLOR::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}




void CEngine::AttachMenu()
{
	SetMenu(m_hMainWnd, m_hMenu);

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = { 0, 0, m_ptResolution.x , m_ptResolution.y};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);
}

void CEngine::DetachMenu()
{
	SetMenu(m_hMainWnd, 0);

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = { 0, 0, m_ptResolution.x , m_ptResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);
}