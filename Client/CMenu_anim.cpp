#include "pch.h"
#include "CMenu_anim.h"

#include "CResMgr.h"
#include "CAnimator.h"

CMenu_anim::CMenu_anim(wstring _name)
{
	AddComponent(new CAnimator);

	if (_name == L"BabaButton")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"stop", pMenuAtlas, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(16.f, 16.f), 32.f, 3, 0.2f);
		GetAnimator()->Play(L"stop", true);
	}
	else if (_name == L"BabaTitle")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"stop", pMenuAtlas, Vec2(0.f, 0.f), Vec2(578.f, 190.f), Vec2(289.f, 95.f), 190.f, 3, 0.2f);
		GetAnimator()->Play(L"stop", true);
	}
	else if (_name == L"Fadein")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"Fadein", pMenuAtlas, Vec2(0.f, 0.f), Vec2(1280.f, 768.f), Vec2(640.f, 384.f), 768.f, 10, 0.08f);
		GetAnimator()->Play(L"Fadein", false);
	}
	else if (_name == L"Fadeout")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"Fadeout", pMenuAtlas, Vec2(0.f, 0.f), Vec2(1280.f, 768.f), Vec2(640.f, 384.f), 768.f, 10, 0.08f);
		GetAnimator()->Play(L"Fadeout", false);
	}
	else if (_name == L"WorldMap")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"WorldMap", pMenuAtlas, Vec2(0.f, 0.f), Vec2(1056.f, 576.f), Vec2(528.f, 288.f), 576.f, 3, 0.2f);
		GetAnimator()->Play(L"WorldMap", true);
	}
	else if (_name == L"Cursor")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"Cursor", pMenuAtlas, Vec2(0.f, 0.f), Vec2(44.f, 44.f), Vec2(22.f, 22.f), 44.f, 3, 0.2f);
		GetAnimator()->Play(L"Cursor", true);
	}
	else if (_name == L"VocaUI_RESTART")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"VocaUI_RESTART", pMenuAtlas, Vec2(0.f, 0.f), Vec2(60.f, 30.f), Vec2(0.f, 0.f), 30.f, 3, 0.2f);
		GetAnimator()->Play(L"VocaUI_RESTART", true);
	}
	else if (_name == L"VocaUI_UNDO")
	{
		CImage* pMenuAtlas = CResMgr::GetInst()->LoadImg(_name + L"Atlas", L"image\\UI\\" + _name + L".bmp");
		GetAnimator()->CreateAnimation(L"VocaUI_UNDO", pMenuAtlas, Vec2(0.f, 0.f), Vec2(60.f, 30.f), Vec2(0.f, 0.f), 30.f, 3, 0.2f);
		GetAnimator()->Play(L"VocaUI_UNDO", true);
	}
}

CMenu_anim::~CMenu_anim()
{
}
