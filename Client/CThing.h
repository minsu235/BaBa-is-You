#pragma once
#include "CObject.h"

enum class Thingtype
{
    BABA, // 여기부터               //Text랑 순서맞추기
    SKULL, // 여기까지 검사

    WALL, // 여기부터
    GRASS,
    HEDGE,
    LAVA,
    WATER, // 여기까지 검사

    FLAG,
    FLOWER,
    ROCK,   
    TILE,
    DOOR,
    KEY,

    TEXT,
    END,
};

const wstring Thing_arr[(UINT)Thingtype::END]
=
{
    L"Baba",
    L"Skull",

    L"Wall",
    L"Grass",
    L"Hedge",
    L"Lava",
    L"Water",

    L"Flag",
    L"Flower",
    L"Rock",
    L"ObjTile",
    L"Door",
    L"Key",

    L"Text",
};

enum class StickDir
{
    N,
    R,
    U,
    UR,
    L,
    LR,
    UL,
    ULR,
    D,
    DR,
    UD,
    UDR,
    DL,
    DLR,
    UDL,
    UDLR,
    END,
};

const wstring StickDir_arr[(UINT)StickDir::END]
=
{
    L"N",
    L"R",
    L"U",
    L"UR",
    L"L",
    L"LR",
    L"UL",
    L"ULR",
    L"D",
    L"DR",
    L"UD",
    L"UDR",
    L"DL",
    L"DLR",
    L"UDL",
    L"UDLR",
};

enum class UnitType
{
    Default,
    FourDir,
    Stick,
};

enum class Text
{
    BABA,       //Thingtype 순서맞추기
    SKULL,

    WALL,
    GRASS,
    HEDGE,
    LAVA,
    WATER,

    FLAG,
    FLOWER,
    ROCK,
    TILE,
    DOOR,
    KEY,

    TEXT,

    IS, // 검사
    AND, // 검사

    YOU,    // 여기부터         //RULE이랑 순서맞추기
    DEFEAT,
    HOT,
    MELT,
    PUSH,
    SINK,
    STOP,
    SHUT,
    OPEN,
    FLOAT,
    MOVE,
    WIN,    // 여기까지 검사

    END,
};

const wstring Text_arr[(UINT)Text::END]
=
{
    L"Baba",
    L"Skull",
    L"Wall",
    L"Grass",
    L"Hedge",
    L"Lava",
    L"Water",
    L"Flag",
    L"Flower",
    L"Rock",
    L"Tile",
    L"Door",
    L"Key",

    L"Text",

    L"Is",
    L"And",

    L"You",
    L"Defeat",
    L"Hot",
    L"Melt",
    L"Push",
    L"Sink",
    L"Stop",
    L"Shut",
    L"Open",
    L"Float",
    L"Move",
    L"Win",
};

class CThing :
    public CObject
{
private:
    //thing

    UINT            m_uState;           //부여된 룰
    DIR             m_eMoveDir;         //움직이는 방향
    DIR             m_eFocusDir;        //바라보는 방향
    bool            m_bFocusChange;     //방향 바뀐지 체크
    UINT            m_uMoveCount;       //움직인 수
    float           m_fMoveTime;        
    Vec2            m_vMatrix;
    Thingtype       m_eThingtype;
    float           m_fWinTime;
    //float
    int             m_iFloat;
    float           m_fFloatTime;
    Vec2            m_vOriginPos;
    //unit
    UnitType        m_eUnitType;
    StickDir        m_eStickDir;
    bool            m_isChange;
    bool            m_Changecheck;
    //float
    Text            m_eText;
    Texttype        m_eTexttype;
    bool            m_isON;
    bool            m_nextonoff;
    bool            m_bonoffchange;

public:
    void setMatrix(Vec2 _vec2) { m_vMatrix = _vec2; }
    Vec2 getMatrix() { return m_vMatrix; }
    void addState(RULE _rule) { m_uState |= 1 << (UINT)_rule; }
    void subState(RULE _rule) { m_uState &= ~(1 << (UINT)_rule); }
    void initState() { m_uState = 0; }
    UINT getState() { return m_uState; }
    bool isgetState(RULE _rule) { return (m_uState & 1 << (UINT)_rule); }
    void setMoveDIR(DIR _dir) { m_eMoveDir = _dir; }
    DIR getMoveDIR() { return m_eMoveDir; }
    void setFocusDIR(DIR _dir) { m_eFocusDir = _dir; }
    DIR getFocusDIR() { return m_eFocusDir; }
    void setMoveCount(UINT _mc) { m_uMoveCount = _mc; }
    UINT getMoveCount() { return m_uMoveCount; }
    void setMovetime(float _f) { m_fMoveTime = _f; }
    float getMovetime() { return m_fMoveTime; }
    void setThingtype(Thingtype _t) { m_eThingtype = _t; }
    Thingtype getThingtype() { return m_eThingtype; }
    void setOriginPos(Vec2 _vec2) { m_vOriginPos = _vec2; }

    void FocusChange() { m_bFocusChange = true; }
    void Winner();

    //unit
    void StickDirCheck();
    void ChangeUnit(Thingtype _thingtype);
    bool isChange() { return m_isChange; }
    void Change(bool _b) { m_isChange = _b; }
    void setChangecheck(bool _b) { m_Changecheck = _b; }
    bool getChangecheck() { return m_Changecheck; }

    //text
    void ChangeUnit(Text _text);
    Text getText() { return m_eText; }
    Texttype getTexttype() { return m_eTexttype; }
    bool isON() { return m_isON; }
    void on() { m_bonoffchange = true; m_nextonoff = true; };
    void off() { m_bonoffchange = true; m_nextonoff = false; };

public:
    virtual void tick() override;
    void thingMove(DIR _dir);
    void thingReverseMove(DIR _movedir, DIR _focusdir);

public:
    virtual CThing* Clone() { return new CThing(*this); }

public:
    CThing(Thingtype _thingtype, DIR _dir);
    CThing(Text _text);
    ~CThing();
};

