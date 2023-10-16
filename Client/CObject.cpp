#include "pch.h"
#include "CObject.h"

#include "CComponent.h"

CObject::CObject()
	: m_arrComponent{}
	, m_bDead(false)
{
}

CObject::CObject(const CObject& _obj)
	: CEntity(_obj)
	, m_arrComponent{}
	, m_vPos(_obj.m_vPos)
	, m_vScale(_obj.m_vScale)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _obj.m_arrComponent[i])
		{			
			AddComponent(_obj.m_arrComponent[i]->Clone());
		}
	}
}

CObject::~CObject()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		SAFE_DELETE(m_arrComponent[i]);
	}
}

void CObject::AddComponent(CComponent* _pComponent)
{
	m_arrComponent[(UINT)_pComponent->GetType()] = _pComponent;
	_pComponent->m_pOwner = this;
}

void CObject::SubComponent(COMPONENT_TYPE _componenttype)
{
	SAFE_DELETE(m_arrComponent[(UINT)_componenttype]);
}

void CObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComponent[i])
			continue;

		m_arrComponent[i]->tick();
	}
}

void CObject::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComponent[i])
			continue;

		m_arrComponent[i]->render(_dc);
	}
}
