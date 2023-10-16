#pragma once

class CThing;

class CRuleEventMgr
{
	SINGLE(CRuleEventMgr);

public:
	vector<tRuleEventInfo>	m_vecRuleEvent[(UINT)RULE_EVENT_TYPE::END];

public:
	void tick();
	void AddRule(const tRuleEventInfo& _event) { m_vecRuleEvent[(UINT)_event.eType].push_back(_event); }
};

