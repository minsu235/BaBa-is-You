#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "CImage.h"
#include "CSound.h"


CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	Safe_Del_Map(m_mapImage);
	Safe_Del_Map(m_mapSound);
}

void CResMgr::init()
{
	LoadSound(L"BGM_menu", L"sound\\BGM\\menu.wav");
	LoadSound(L"BGM_map", L"sound\\BGM\\map.wav");
	LoadSound(L"BGM_noise", L"sound\\BGM\\noise.wav");
	LoadSound(L"BGM_float", L"sound\\BGM\\float.wav");
	LoadSound(L"BGM_menuselect", L"sound\\BGM\\menuselect.wav");
	LoadSound(L"BGM_stage", L"sound\\BGM\\stage.wav");
	LoadSound(L"BGM_win", L"sound\\BGM\\win.wav");
	LoadSound(L"BGM_baba", L"sound\\BGM\\baba.wav");
	LoadSound(L"BGM_burn", L"sound\\BGM\\burn.wav");
	LoadSound(L"BGM_cave", L"sound\\BGM\\cave.wav");
	LoadSound(L"BGM_crystal", L"sound\\BGM\\crystal.wav");
	LoadSound(L"BGM_editor", L"sound\\BGM\\editor.wav");
	LoadSound(L"BGM_factory", L"sound\\BGM\\factory.wav");
	LoadSound(L"BGM_forest", L"sound\\BGM\\forest.wav");
	LoadSound(L"BGM_garden", L"sound\\BGM\\garden.wav");
	LoadSound(L"BGM_mountain", L"sound\\BGM\\mountain.wav");
	LoadSound(L"BGM_rain", L"sound\\BGM\\rain.wav");
	LoadSound(L"BGM_ruin", L"sound\\BGM\\ruin.wav");
	LoadSound(L"BGM_space", L"sound\\BGM\\space.wav");
	LoadSound(L"BGM_stars", L"sound\\BGM\\stars.wav");

	LoadSound(L"Effect_restart", L"sound\\Effect\\restart.wav");

	LoadSound(L"Effect_move0", L"sound\\Effect\\move0.wav");
	LoadSound(L"Effect_move1", L"sound\\Effect\\move1.wav");
	LoadSound(L"Effect_move2", L"sound\\Effect\\move2.wav");
	LoadSound(L"Effect_move3", L"sound\\Effect\\move3.wav");
	LoadSound(L"Effect_move4", L"sound\\Effect\\move4.wav");
	LoadSound(L"Effect_move5", L"sound\\Effect\\move5.wav");
	LoadSound(L"Effect_move6", L"sound\\Effect\\move6.wav");
	LoadSound(L"Effect_move7", L"sound\\Effect\\move7.wav");
	LoadSound(L"Effect_move8", L"sound\\Effect\\move8.wav");
	LoadSound(L"Effect_move9", L"sound\\Effect\\move9.wav");
	LoadSound(L"Effect_move10", L"sound\\Effect\\move10.wav");
	LoadSound(L"Effect_move11", L"sound\\Effect\\move11.wav");

	LoadSound(L"Effect_menumove0", L"sound\\Effect\\menumove0.wav");
	LoadSound(L"Effect_menumove1", L"sound\\Effect\\menumove1.wav");
	LoadSound(L"Effect_menumove2", L"sound\\Effect\\menumove2.wav");
	LoadSound(L"Effect_menumove3", L"sound\\Effect\\menumove3.wav");
	LoadSound(L"Effect_menumove4", L"sound\\Effect\\menumove4.wav");

	LoadSound(L"Effect_undo0", L"sound\\Effect\\undo0.wav");
	LoadSound(L"Effect_undo1", L"sound\\Effect\\undo1.wav");
	LoadSound(L"Effect_undo2", L"sound\\Effect\\undo2.wav");
	LoadSound(L"Effect_undo3", L"sound\\Effect\\undo3.wav");
	LoadSound(L"Effect_undo4", L"sound\\Effect\\undo4.wav");

	LoadSound(L"Effect_texton0", L"sound\\Effect\\texton0.wav");
	LoadSound(L"Effect_texton1", L"sound\\Effect\\texton1.wav");
	LoadSound(L"Effect_texton2", L"sound\\Effect\\texton2.wav");
	LoadSound(L"Effect_texton3", L"sound\\Effect\\texton3.wav");
	LoadSound(L"Effect_texton4", L"sound\\Effect\\texton4.wav");

	LoadSound(L"Effect_open0", L"sound\\Effect\\open0.wav");
	LoadSound(L"Effect_open1", L"sound\\Effect\\open1.wav");
	LoadSound(L"Effect_open2", L"sound\\Effect\\open2.wav");
	LoadSound(L"Effect_open3", L"sound\\Effect\\open3.wav");

	LoadSound(L"Effect_sink0", L"sound\\Effect\\sink0.wav");
	LoadSound(L"Effect_sink1", L"sound\\Effect\\sink1.wav");
	LoadSound(L"Effect_sink2", L"sound\\Effect\\sink2.wav");
	LoadSound(L"Effect_sink3", L"sound\\Effect\\sink3.wav");

	LoadSound(L"Effect_burned", L"sound\\Effect\\burned.wav");
	LoadSound(L"Effect_defeat0", L"sound\\Effect\\defeat0.wav");
	LoadSound(L"Effect_defeat1", L"sound\\Effect\\defeat1.wav");
	LoadSound(L"Effect_defeat2", L"sound\\Effect\\defeat2.wav");
}

CImage* CResMgr::LoadImg(const wstring& _strKey, const wstring& _strRelativePath)
{
	// Ű������ Ž��
	CImage* pImage = FindImg(_strKey);
	if (nullptr != pImage)
	{
		// �ش�Ű�� �̹� �ε��Ȱ� ������ �ش� ���ҽ��� ��ȯ
		return pImage;
	}

	// �ش� Ű�� �ε��� �̹����� ����.
	
	// content ���� ��� + ����� = ���� ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pImage = new CImage;
	if (FAILED(pImage->Load(strFilePath)))
	{
		MessageBox(nullptr, L"�̹��� �ε� ����", L"����", MB_OK);
		return nullptr;
	}

	pImage->SetKey(_strKey);
	pImage->SetRelativePath(_strRelativePath);
	m_mapImage.insert(make_pair(_strKey, pImage));

	return pImage;
}

CImage* CResMgr::CreateImg(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	// Ű������ Ž��
	CImage* pImage = FindImg(_strKey);
	if (nullptr != pImage)
	{
		MessageBox(nullptr, L"�ߺ� Ű �߻�", L"�̹��� ���� ����", MB_OK);
		return nullptr;
	}

	// �ش� Ű�� ������ �̹����� ����.
	pImage = new CImage;
	if (FAILED(pImage->Create(_iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"�̹��� ���� ����", L"����", MB_OK);
		return nullptr;
	}

	pImage->SetKey(_strKey);	
	m_mapImage.insert(make_pair(_strKey, pImage));

	return pImage;
}

CImage* CResMgr::FindImg(const wstring& _strKey)
{
	map<wstring, CImage*>::iterator iter = m_mapImage.find(_strKey);

	// �̹� ������ Ű������ �ٸ� ���ҽ��� ���� ��ϵǾ� �־���.
	if (iter != m_mapImage.end())
	{
		return iter->second;
	}

	return nullptr;
}


CSound* CResMgr::FindSound(const wstring& _strKey)
{
	map<wstring, CSound*>::iterator iter = m_mapSound.find(_strKey);

	if (iter == m_mapSound.end())
		return nullptr;

	return (CSound*)iter->second;
}

CSound* CResMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	CSound* pSound = FindSound(_strKey);

	if (nullptr != pSound)
		return pSound;

	pSound = new CSound;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	HRESULT hr = pSound->Load(strFilePath.c_str());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sound �ε� ����!!!", L"���ҽ� �ε� ����", MB_OK);
		delete pSound;
		return nullptr;
	}

	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	// map �� ���
	m_mapSound.insert(make_pair(_strKey, pSound));

	return pSound;
}