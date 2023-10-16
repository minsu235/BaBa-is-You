#pragma once

class CImage;

class CEngine
{
	SINGLE(CEngine);
private:
	HWND			m_hMainWnd;	// 메인 윈도우 핸들
	HDC				m_hDC;		// DC 핸들

	POINT			m_ptResolution; // 해상도
	CImage*			m_pBackBuffer;	// 백버퍼(이중버퍼 용도)

	HPEN			m_arrPen[(UINT)PEN_COLOR::END];
	HBRUSH			m_arrBrush[(UINT)BRUSH_COLOR::END];

	HMENU			m_hMenu;




public:
	HWND GetMainHwnd() { return m_hMainWnd; };
	HDC GetMainDC() { return m_hDC; }
	HPEN GetPen(PEN_COLOR _color) { return m_arrPen[(UINT)_color]; }
	HBRUSH GetBrush(BRUSH_COLOR _brush) { return m_arrBrush[(UINT)_brush]; }	
	POINT GetResolution() { return m_ptResolution; }

	void AttachMenu();
	void DetachMenu();

	// 초기화
	void init(HWND _hWnd, UINT _iWidth, UINT _iHeight);

	// 매 프레임 실행
	void tick();

private:
	void CreateDefaultGDIObject();

};

