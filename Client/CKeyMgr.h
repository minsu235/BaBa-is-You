#pragma once

enum class KEY_TYPE
{
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	 
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	
	ENTER,
	ESC,
	LSHIFT,
	LALT,
	LCTRL,
	SPACE,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	LBTN,
	RBTN,

	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,

	N_0,
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,


	END,
};

enum KEY_STATE
{
	TAP,
	PRESS,
	RELEASE,
	NONE,
};

struct tKeyInfo
{
	KEY_TYPE  eType;
	KEY_STATE eState;
	bool	  bPrevPress;
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vMousePos;

public:
	void init();
	void tick();

public:
	KEY_STATE GetKeyState(KEY_TYPE _key)
	{
		return m_vecKey[(UINT)_key].eState;
	}

	Vec2 GetMousePos() { return m_vMousePos; }
};

