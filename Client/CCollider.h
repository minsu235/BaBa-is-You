#pragma once
#include "CComponent.h"

union COLLIDER_ID
{
    struct
    {
        UINT LEFT_ID;
        UINT RIGHT_ID;
    };
    LONGLONG ID;
};


class CCollider :
    public CComponent
{
private:
    Vec2        m_vOffsetPos;   // Object 로 부터 상대적인 거리
    Vec2        m_vScale;
    Vec2        m_vFinalPos;    // 충돌체의 최종 위치

    int         m_iCollisionCount; // 충돌 횟수

public:
    void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetOffset(Vec2 _vOffset) { m_vOffsetPos = _vOffset; }

    Vec2 GetFinalPos() const { return m_vFinalPos;}
    Vec2 GetScale() const { return m_vScale; }

public:    
    // 충돌이 처음 발생하는 시점
    void CollisionBeginOverlap(CCollider* _pOtherCollider);   

    // 충돌 중이다.
    void CollisionOverlap(CCollider* _pOtherCollider);

    // 충돌이 벗어나지는 시점
    void CollisionEndOverlap(CCollider* _pOtherCollider);
  
public:
    CLONE(CCollider);

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

public:
    CCollider();
    CCollider(const CCollider& _collider);
    ~CCollider();
};

