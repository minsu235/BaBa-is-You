#pragma once
#include "CObject.h"

class CBurned :
    public CObject
{
public:
	float   m_fMoveTime;

public:
	virtual void tick() override;

public:
	CLONE(CBurned);

public:
	CBurned(Vec2 _location);
	~CBurned();
};