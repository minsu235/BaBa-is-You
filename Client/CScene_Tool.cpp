#include "pch.h"
#include "CScene_Tool.h"
#include "Client.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CResMgr.h"

#include "CMenu.h"
#include "CMenu_anim.h"
#include "CSound.h"
#include "CToolTile.h"

#include "CUIMgr.h"
#include "CToolPanel.h"
#include "CButton.h"

CScene_Tool::CScene_Tool()
	: m_bFade(true)
	, m_fFadetime(0)
	, m_panel(nullptr)
	, m_Bababutton(nullptr)
	, m_iPoint(1)
	, m_eScenestate(Scenestate::PLAY)
	, m_BGM(BGMtype::BABA)
{
}

CScene_Tool::~CScene_Tool()
{
}

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void CScene_Tool::enter()
{
	CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_editor");
	pBGM->PlayToBGM(true);

	m_bFade = true;
	m_fFadetime = 0;
	m_Bababutton = nullptr;
	m_iPoint = 1;
	m_eScenestate = Scenestate::PLAY;
	m_BGM = BGMtype::BABA;
 
	DialogBox(NULL , MAKEINTRESOURCE(DLG_Map), CEngine::GetInst()->GetMainHwnd(), TileCountProc);

	for (UINT iRow = 0; iRow < m_vSize.y; ++iRow)
	{
		for (UINT iCol = 0; iCol < m_vSize.x; ++iCol)
		{
			CToolTile* pTile = new CToolTile;
			pTile->SetPos(tooltilePos(iRow, iCol));
			pTile->setIdx(Vec2((float)iRow, (float)iCol));
			AddObject(pTile, LAYER_TYPE::TILE);
		}
	}
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	// PanelUI
	CToolPanel* pPanel = new CToolPanel;
	pPanel->SetPos(Vec2(vResolution.x - 342.f, 0.f));
	m_panel = pPanel;
	AddObject(pPanel, LAYER_TYPE::UI);
}

void CScene_Tool::tick()
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
			m_fFadetime = 0;
			m_bFade = false;
			DeleteObject(LAYER_TYPE::FADEINOUT);
		}
		else
			m_fFadetime += DT;
		CScene::tick();
		return;
	}

	if (m_eScenestate == Scenestate::PLAY)
	{
		if (IsTap(KEY_TYPE::ESC))
		{
			m_eScenestate = Scenestate::ESC;
			CMenu* Back = new CMenu(L"ESCBackground");
			Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
			Back->SetPos(Vec2(res.x / 2, res.y / 2));
			AddObject(Back, LAYER_TYPE::MENU);
			CMenu* resume = new CMenu(L"ESCResume");
			resume->SetPos(Vec2(res.x / 2, res.y / 2 - 230));
			AddObject(resume, LAYER_TYPE::MENU);
			CMenu* returntomenu = new CMenu(L"ESCRmenu");
			returntomenu->SetPos(Vec2(res.x / 2, res.y / 2 - 180));
			AddObject(returntomenu, LAYER_TYPE::MENU);
			CMenu_anim* Bababutton = new CMenu_anim(L"BabaButton");
			Bababutton->SetPos(Vec2(res.x / 2 - 168, res.y / 2 - 246));
			m_Bababutton = Bababutton;
			AddObject(Bababutton, LAYER_TYPE::MENU);
		}
		else if (IsTap(KEY_TYPE::NUM_6))
		{
			if ((UINT)m_BGM == (UINT)BGMtype::STARS)
				m_BGM = BGMtype::BABA;
			else
				m_BGM = (BGMtype)((UINT)m_BGM + 1);

			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_" + BGM_arr[(UINT)m_BGM]);
			pBGM->PlayToBGM(true);
		}
		else if (IsTap(KEY_TYPE::NUM_4))
		{
			if ((UINT)m_BGM == (UINT)BGMtype::BABA)
				m_BGM = BGMtype::STARS;
			else
				m_BGM = (BGMtype)((UINT)m_BGM - 1);

			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_" + BGM_arr[(UINT)m_BGM]);
			pBGM->PlayToBGM(true);
		}
		else if (IsTap(KEY_TYPE::NUM_5))
		{
			CSound* pBGM = CResMgr::GetInst()->FindSound(L"BGM_editor");
			pBGM->PlayToBGM(true);
		}
		else if (IsTap(KEY_TYPE::N_1))
		{
			MapSave();
		}
		else if (IsTap(KEY_TYPE::N_2))
		{
			MapLoad();
		}
		else if (IsTap(KEY_TYPE::R))
		{
			SceneChange(TOOL);
		}
	}
	else if (m_eScenestate == Scenestate::ESC)
	{
		if (IsTap(KEY_TYPE::UP))
		{
			if (m_iPoint != 1)
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
			if (m_iPoint != 2)
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
	else if (m_eScenestate == Scenestate::MENUFADEOUT)
	{
		if (m_fFadetime == 0)
		{
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

	CScene::tick();
}

void CScene_Tool::exit()
{
	DeleteAllObject();
}

void CScene_Tool::render(HDC _dc)
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

void CScene_Tool::MapSave()
{
	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Stage\0*.stage\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	// 파일 입출력
	FILE* pFile = nullptr;

	// 쓰기 모드로 열기
	_wfopen_s(&pFile, szFilePath, L"wb");

	if (nullptr == pFile)
		return;

	// BGM
	wstring str = L"BGM_" + BGM_arr[(UINT)m_BGM];
	SaveWString(str, pFile);

	UINT _iCol = m_vSize.y;
	UINT _iRow = m_vSize.x;

	fwrite(&_iCol, sizeof(UINT), 1, pFile);
	fwrite(&_iRow, sizeof(UINT), 1, pFile);

	vector<CObject*> vec = GetObjects(LAYER_TYPE::TILE);
	int textcount = 0;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (dynamic_cast<CToolTile*>(vec[i])->getThingtype() == Thingtype::TEXT)
			++textcount;
	}
	fwrite(&textcount, sizeof(int), 1, pFile);

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (dynamic_cast<CToolTile*>(vec[i])->getThingtype() == Thingtype::TEXT)
		{
			Text _type = dynamic_cast<CToolTile*>(vec[i])->getText();
			fwrite(&_type, sizeof(Thingtype), 1, pFile);
			Vec2 _vec2 = dynamic_cast<CToolTile*>(vec[i])->getIdx();
			fwrite(&_vec2, sizeof(Vec2), 1, pFile);
		}
	}

	int Objcount = 0;
	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (dynamic_cast<CToolTile*>(vec[i])->getThingtype() != Thingtype::END && dynamic_cast<CToolTile*>(vec[i])->getThingtype() != Thingtype::TEXT)
			++Objcount;
	}
	fwrite(&Objcount, sizeof(int), 1, pFile);

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (dynamic_cast<CToolTile*>(vec[i])->getThingtype() != Thingtype::END && 
			dynamic_cast<CToolTile*>(vec[i])->getThingtype() != Thingtype::TEXT)
		{
			Thingtype _thingtype = dynamic_cast<CToolTile*>(vec[i])->getThingtype();
			fwrite(&_thingtype, sizeof(Thingtype), 1, pFile);
			Vec2 _vec2 = dynamic_cast<CToolTile*>(vec[i])->getIdx();
			fwrite(&_vec2, sizeof(Vec2), 1, pFile);
			DIR dir = dynamic_cast<CToolTile*>(vec[i])->getDIR();;
			fwrite(&dir, sizeof(DIR), 1, pFile);
		}
	}

	fclose(pFile);
}

void CScene_Tool::MapLoad()
{
	DeleteAllObject();
	m_bFade = true;
	m_fFadetime = 0;
	m_Bababutton = nullptr;
	m_iPoint = 1;
	m_eScenestate = Scenestate::PLAY;
	m_BGM = BGMtype::BABA;

	OPENFILENAME ofn = {};

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Stage\0*.stage\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	// 파일 입출력
	FILE* pFile = nullptr;

	// 쓰기 모드로 열기
	_wfopen_s(&pFile, szFilePath, L"rb");

	if (nullptr == pFile)
		return;

	// 타일 가로 세로 개수
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

	for (UINT iRow = 0; iRow < m_vSize.y; ++iRow)
	{
		for (UINT iCol = 0; iCol < m_vSize.x; ++iCol)
		{
			CToolTile* pTile = new CToolTile;
			pTile->SetPos(tooltilePos(iRow, iCol));
			pTile->setIdx(Vec2((float)iRow, (float)iCol));
			AddObject(pTile, LAYER_TYPE::TILE);
		}
	}

	vector<CObject*> vec = GetObjects(LAYER_TYPE::TILE);
	int i = 0;
	fread(&i, sizeof(int), 1, pFile);
	Text _type = Text::END;
	Vec2 vec2;
	for (int count = 0; count < i; ++count)
	{
		fread(&_type, sizeof(Text), 1, pFile);
		fread(&vec2, sizeof(Vec2), 1, pFile);
		
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (dynamic_cast<CToolTile*>(vec[i])->getIdx() == vec2)
			{
				dynamic_cast<CToolTile*>(vec[i])->setThingtype(Thingtype::TEXT);
				dynamic_cast<CToolTile*>(vec[i])->setText(_type);
				break;
			}
		}
	}
	fread(&i, sizeof(int), 1, pFile);
	Thingtype thingtype = Thingtype::END;
	DIR dir = DIR::NONE;
	for (int count = 0; count < i; ++count)
	{
		fread(&thingtype, sizeof(Thingtype), 1, pFile);
		fread(&vec2, sizeof(Vec2), 1, pFile);
		fread(&dir, sizeof(DIR), 1, pFile);

		for (size_t i = 0; i < vec.size(); ++i)
		{
			if (dynamic_cast<CToolTile*>(vec[i])->getIdx() == vec2)
			{
				dynamic_cast<CToolTile*>(vec[i])->setThingtype(thingtype);
				dynamic_cast<CToolTile*>(vec[i])->setDIR(dir);
				break;
			}
		}
	}

	fclose(pFile);

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	// PanelUI
	CToolPanel* pPanel = new CToolPanel;
	pPanel->SetPos(Vec2(vResolution.x - 342.f, 0.f));
	m_panel = pPanel;
	AddObject(pPanel, LAYER_TYPE::UI);
}

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:

		switch (wParam)
		{
		case IDOK:
			if (LOWORD(wParam) == IDOK)
			{
				// Edit Control 에 입력한 수치를 알아낸다.
				UINT iCol = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
				UINT iRow = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

				CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
				if (nullptr == dynamic_cast<CScene_Tool*>(pScene))
				{
					MessageBox(nullptr, L"에러", L"에러", MB_OK);
				}
				else
				{
					dynamic_cast<CScene_Tool*>(pScene)->setSize(Vec2(iCol, iRow));
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		}

		break;
	}
	return (INT_PTR)FALSE;
}