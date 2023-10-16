#pragma once
#include "CEntity.h"

class CImage;


struct tAnimFrm
{
    Vec2    vLeftTop;   // �̹��� ������ �»�� �ȼ���ǥ
    Vec2    vSize;      // �»�����κ��� �߶� ���� ���� �ȼ� ����    
    Vec2    vOffset;    // ��� ��ġ ������
    float   fDuration;  // �ش� ������ �����ð�
};


class CAnimator;

class CAnimation :
    public CEntity
{
private:
    CAnimator*          m_pAnimator;

    vector<tAnimFrm>    m_vecFrm;   // ��� ������ ����
    CImage*             m_pImage;   // Animation Frame Image

    int                 m_iCurFrm;  // ���� ���� ��ų ������ �ε���
    float               m_fAccTime; // �����ð�

    bool                m_bFinish;  // ��� �Ϸ�


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
