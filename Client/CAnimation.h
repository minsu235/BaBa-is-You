#pragma once
#include "CEntity.h"

class CImage;


struct tAnimFrm
{
    Vec2    vLeftTop;   // 이미지 내에서 좌상단 픽셀좌표
    Vec2    vSize;      // 좌상단으로부터 잘라낼 가로 세로 픽셀 길이    
    Vec2    vOffset;    // 출력 위치 오프셋
    float   fDuration;  // 해당 프레임 유지시간
};


class CAnimator;

class CAnimation :
    public CEntity
{
private:
    CAnimator*          m_pAnimator;

    vector<tAnimFrm>    m_vecFrm;   // 모든 프레임 정보
    CImage*             m_pImage;   // Animation Frame Image

    int                 m_iCurFrm;  // 현재 노출 시킬 프레임 인덱스
    float               m_fAccTime; // 누적시간

    bool                m_bFinish;  // 재생 완료


public:
    void tick();
    void render(HDC _dc);

public:
    void Create(CImage* _pImage, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset
        , float _fStep, UINT _iFrmCount, float _fDuration);

    void Reset()
    {
        m_iCurFrm = 0;
        m_bFinish = false;
        m_fAccTime = 0.f;
    }

    bool IsFinish() { return m_bFinish; }

public:
    CAnimation();
    ~CAnimation();

    friend class CAnimator;
};

