#pragma once

struct Vector2
{
	float x;
	float y;
	
public:
	bool IsZero()
	{
		if (0.f == x && 0.f == y)
			return true;
		return false;
	}


	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2& Normalize()
	{
		float fLength = Length();

		assert(fLength);

		x /= fLength;
		y /= fLength;

		return *this;
	}

	float Distance(Vector2 _vOther)
	{
		float _x = x - _vOther.x;
		float _y = y - _vOther.y;

		return sqrtf(_x * _x + _y * _y);
	}

	float Dot(Vector2 _vOther)
	{
		return x* _vOther.x + y * _vOther.y;
	}


	Vector2 operator + (Vector2 _vOther)
	{
		return Vector2(x + _vOther.x, y + _vOther.y);
	}

	void operator += (Vector2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}

	Vector2 operator -()
	{
		return Vector2(-x, -y);
	}

	Vector2 operator - (const Vector2 _vOther) const
	{
		return Vector2(x - _vOther.x, y - _vOther.y);
	}

	void operator -= (Vector2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}


	void operator -= (float _f)
	{
		x -= _f;
		y -= _f;
	}




	Vector2 operator * (float _f)
	{
		return Vector2(x * _f, y * _f);
	}

	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}

	Vector2 operator * (Vector2 _v)
	{
		return Vector2(x * _v.x, y * _v.y);
	}



	Vector2 operator / (const float _f)
	{
		assert(_f != 0.f);

		return Vector2(x / _f, y / _f);
	}

	void operator /= (float _f)
	{
		assert(_f != 0.f);

		x /= _f;
		y /= _f;
	}

	Vector2 operator / (Vector2 _v)
	{
		assert(_v.x != 0.f && _v.y != 0.f);

		return Vector2(x / _v.x, y / _v.y);
	}

	void operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
	}
	
	bool operator == (Vector2 _vOther)
	{
		return (x == _vOther.x && y == _vOther.y);
	}

public:
	Vector2()
		: x(0.f)
		, y(0.f)
	{}

	Vector2(float _x, float _y)
		: x(_x), y(_y)
	{}

	Vector2(POINT _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}

	~Vector2() {}
};


typedef Vector2 Vec2;



struct tSelectPen
{
private:
	HDC		m_hDC;
	HPEN	m_hPrevPen;

public:
	void SetPenColor(PEN_COLOR _color)
	{
		m_hPrevPen = (HPEN)SelectObject(m_hDC, CEngine::GetInst()->GetPen(_color));
	}

public:
	tSelectPen(HDC _dc)
		: m_hDC(_dc)
		, m_hPrevPen(0)
	{

	}

	tSelectPen(HDC _dc, PEN_COLOR _color)
		: m_hDC(_dc)
		, m_hPrevPen(0)
	{		
		m_hPrevPen = (HPEN)SelectObject(m_hDC, CEngine::GetInst()->GetPen(_color));
	}

	~tSelectPen()
	{
		SelectObject(m_hDC, m_hPrevPen);
	}
};

struct tSelectBrush
{
private:
	HDC		m_hDC;
	HBRUSH	m_hPrevBrush;

public:
	tSelectBrush(HDC _dc, BRUSH_COLOR _color)
		: m_hDC(_dc)
		, m_hPrevBrush(0)
	{		
		m_hPrevBrush = (HBRUSH)SelectObject(m_hDC, CEngine::GetInst()->GetBrush(_color));
	}

	~tSelectBrush()
	{
		SelectObject(m_hDC, m_hPrevBrush);
	}
};


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	first;
	DWORD_PTR	second;
};

class CThing;
struct tRuleEventInfo
{
	RULE_EVENT_TYPE		eType;
	CThing*				_Thing;
	DWORD_PTR			second;
	DWORD_PTR			third;
};

class CText;
struct tTextRuleInfo
{
	CThing* first;
	CThing* second;

public:
	bool operator == (tTextRuleInfo _vOther)
	{
		return((first == _vOther.first && second == _vOther.second));
	}

	bool operator != (tTextRuleInfo _vOther)
	{
		return((first != _vOther.first || second != _vOther.second));
	}

public:
	tTextRuleInfo()
		: first(nullptr)
		, second(nullptr)
	{
	}

	~tTextRuleInfo()
	{
	}
};