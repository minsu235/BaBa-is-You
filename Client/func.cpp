#include "pch.h"
#include "func.h"

#include "CObject.h"
#include "CImage.h"
#include "CSpark.h"

#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene_Stage.h"
#include "CScene_Tool.h"

void DeleteObject(CObject* _pObj)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::DELETE_OBJECT;
	info.first = (DWORD_PTR)_pObj;
	CEventMgr::GetInst()->AddEvent(info);
}

void SceneChange(SCENE_TYPE _eNextScene)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::SCENE_CHANGE;
	info.first = (DWORD_PTR)_eNextScene;
	CEventMgr::GetInst()->AddEvent(info);
}

void SceneChange(SCENE_TYPE _eNextScene, int _i)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::SCENE_CHANGE;
	info.first = (DWORD_PTR)_eNextScene;
	info.second = (DWORD_PTR)_i;
	CEventMgr::GetInst()->AddEvent(info);
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	wchar_t szBuff[256] = {};

	fread(&iLen, sizeof(size_t), 1, _pFile);
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;
}

void SaveImageRef(CImage* _pImage, FILE* _pFile)
{
	int iCheck = 1;
	if (nullptr == _pImage)
	{
		iCheck = 0;
		fwrite(&iCheck, sizeof(int), 1, _pFile);
	}
	else
	{
		fwrite(&iCheck, sizeof(int), 1, _pFile);

		const wstring& strKey = _pImage->GetKey();
		const wstring& strPath = _pImage->GetRelativePath();

		SaveWString(strKey, _pFile);
		SaveWString(strPath, _pFile);
	}
}

void LoadImageRef(CImage** _ppImage, FILE* _pFile)
{
	int iCheck = 0;		
	fread(&iCheck, sizeof(int), 1, _pFile);

	if (0 == iCheck)
		return;

	wstring strKey;
	wstring strRelativePath;

	LoadWString(strKey, _pFile);
	LoadWString(strRelativePath, _pFile);

	*_ppImage = CResMgr::GetInst()->LoadImg(strKey, strRelativePath);
}

Vec2 RotateVector(Vec2 _vOrigin, float _Degree)
{
	float fRadian = (_Degree / 180.f) * PI;

	_vOrigin.Normalize();
	
	return Vec2(cosf(fRadian) * _vOrigin.x - sinf(fRadian) * _vOrigin.y
		      , sinf(fRadian) * _vOrigin.x + cosf(fRadian) * _vOrigin.y);
}

float CrossVector(Vec2 _v1, Vec2 _v2)
{
	return _v1.x * _v2.y - _v1.y * _v2.x;
}

Vec2 tilePos(UINT _Row, UINT _Col)
{
	CScene_Stage* stage = (CScene_Stage*)(CSceneMgr::GetInst()->GetCurScene());
	Vec2 _Size = stage->getSize();
	Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
	Vec2 vSize = _Size * TILE_SIZE;
	Vec2 vPos = Vec2(res.x / 2 - vSize.x / 2.f, res.y / 2 - vSize.y / 2.f);
	
	return Vec2(vPos.x + (float)(_Col * TILE_SIZE), vPos.y + (float)(_Row * TILE_SIZE));
}

Vec2 tooltilePos(UINT _Row, UINT _Col)
{
	CScene_Tool* stage = (CScene_Tool*)(CSceneMgr::GetInst()->GetCurScene());
	Vec2 _Size = stage->getSize();
	Vec2 res = (Vec2)CEngine::GetInst()->GetResolution();
	Vec2 vSize = _Size * TILE_SIZE;
	Vec2 vPos = Vec2(res.x / 2 - vSize.x / 2.f, res.y / 2 - vSize.y / 2.f);

	return Vec2(vPos.x + (float)(_Col * TILE_SIZE), vPos.y + (float)(_Row * TILE_SIZE));
}

int MapMoveDIR(Vec2 _pos, DIR _dir)
{
	CScene_Stage* stage = (CScene_Stage*)(CSceneMgr::GetInst()->GetCurScene());
	Vec2 _Size = stage->getSize();

	switch (_dir)
	{
	case DIR::UP:
	{
		int up = (int)((_pos.x - 1) * _Size.x + _pos.y);
		if (up < 0)
			return LIMIT;
		else
			return up;
	}
		break;
	case DIR::DOWN:
	{
		int down = (int)((_pos.x + 1) * _Size.x + _pos.y);
		if (down >= (int)(_Size.x * _Size.y))
			return LIMIT;
		else
			return down;
	}
		break;
	case DIR::LEFT:
	{
		int left = (int)(_pos.x * _Size.x + _pos.y - 1);
		if (_pos.y == 0)
			return LIMIT;
		else
			return left;
	}
		break;
	case DIR::RIGHT:
	{
		int right = (int)(_pos.x * _Size.x + _pos.y + 1);
		if (right == (int)((_pos.x + 1) * _Size.x))
			return LIMIT;
		else
			return right;
	}
		break;
	}
	return LIMIT;
}

Vec2 MapDIR(Vec2 _pos, DIR _dir)
{
	CScene_Stage* stage = (CScene_Stage*)(CSceneMgr::GetInst()->GetCurScene());
	Vec2 _Size = stage->getSize();
	if (_pos == LIMITVEC)
		return LIMITVEC;

	switch (_dir)
	{
	case DIR::UP:
	{
		int up = (int)((_pos.x - 1) * _Size.x + _pos.y);
		if (up < 0)
			return LIMITVEC;
		else
			return Vec2(_pos.x -1 , _pos.y);
	}
	break;
	case DIR::DOWN:
	{
		int down = (int)((_pos.x + 1) * _Size.x + _pos.y);
		if (down >= (int)(_Size.x * _Size.y))
			return LIMITVEC;
		else
			return Vec2(_pos.x + 1, _pos.y);
	}
	break;
	case DIR::LEFT:
	{
		int left = (int)(_pos.x * _Size.x + _pos.y - 1);
		if (_pos.y == 0)
			return LIMITVEC;
		else
			return Vec2(_pos.x, _pos.y - 1);
	}
	break;
	case DIR::RIGHT:
	{
		int right = (int)(_pos.x * _Size.x + _pos.y + 1);
		if (right == (int)((_pos.x + 1) * _Size.x))
			return LIMITVEC;
		else
			return Vec2(_pos.x, _pos.y + 1);
	}
	break;
	}

	return LIMITVEC;
}

DIR ReverseDIR(DIR _dir)
{
	switch (_dir)
	{
	case DIR::UP:
		return DIR::DOWN;
	case DIR::DOWN:
		return DIR::UP;
	case DIR::LEFT:
		return DIR::RIGHT;
	case DIR::RIGHT:
		return DIR::LEFT;
	}
}

void SparkCreate(wstring _string, Vec2 _location)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJECT;
	info.second = (DWORD_PTR)LAYER_TYPE::EFFECT;

	for (int i = 0; i < 10; ++i)
	{
		CSpark* pSpark = new CSpark(_string, _location);
		info.first = (DWORD_PTR)pSpark;
		CEventMgr::GetInst()->AddEvent(info);
	}
}
