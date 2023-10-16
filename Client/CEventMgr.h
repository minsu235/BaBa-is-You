#pragma once



class CObject;

class CEventMgr
{
	SINGLE(CEventMgr);

private:
	vector<tEventInfo>	m_vecEvent;
	vector<CObject*>	m_vecDeadObj;

public:
	void tick();
	void AddEvent(const tEventInfo& _event) { m_vecEvent.push_back(_event);}
};

