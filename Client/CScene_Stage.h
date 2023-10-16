#pragma once
#include "CScene.h"
#include "CThing.h"

enum class Sentence
{
    ObjISObj,
    ObjISPro,
};

struct SentenceInfo
{
    CThing* first;
    CThing* second;
};

enum class MoveType
{
    MOVE,
    FOCUS,
    CHANGE,
    DEAD,
};

struct moveinfo
{
    CThing*     moveThing;
    UINT        actCount;
    MoveType    moveType;
    DWORD_PTR   first;
    DWORD_PTR   second;


public:
    moveinfo(CThing* _thing, UINT _count, MoveType _type, DWORD_PTR _first, DWORD_PTR _second)
        : moveThing(_thing)
        , actCount(_count)
        , moveType(_type)
        , first(_first)
        , second(_second)
    {
    }
};

class CMenu_anim;
class CSound;

class CScene_Stage :
    public CScene
{
private:
    //map name
    wstring                     m_sMapName;
    //fade out, win
    bool                        m_bFade;
    float                       m_fFadetime;
    //stage
    vector<CThing*>	            m_arrText[(UINT)Texttype::NONE];
    list<list<CThing*>>         m_listTextlist;
    list<SentenceInfo>          m_listSentence;
    vector<CThing*>             m_arrThing[(UINT)Thingtype::END];
    Vec2                        m_vSize;
    multimap<int, CThing*>      m_mapMatrix;
    list<moveinfo>              m_stackMoveinfo;
    bool                        m_done;
    float                       m_ftime;
    bool                        m_movesound;
    bool                        m_textsound;
    CSound*                     m_noise;
    float                       m_fYoutime;

    //stage에 존재하는 property 저장해서 존재하는 property만 검사
    UINT                        m_uProperty;

    //UNDO용 기능
    UINT                        m_actCount;
    bool                        m_iscountup;
    bool                        m_isUndo;

    //ESC
    Scenestate                  m_eScenestate;
    CMenu_anim*                 m_Bababutton;
    int                         m_iPoint;
    bool                        m_bGrid;

public:
    void setSize(Vec2 _vec2) { m_vSize = _vec2; }
    Vec2 getSize() { return m_vSize; }
    multimap<int, CThing*>& getMultimap() { return m_mapMatrix; }
    vector<CThing*>& getarrThing(Thingtype _type) { return m_arrThing[(UINT)_type]; }
    vector<CThing*>& getarrText(Texttype _type) { return m_arrText[(UINT)_type]; }
    UINT getActCount() { return m_actCount; }

    bool IsCountUp() { return m_iscountup; }
    void CountUp() { if (m_iscountup) { ++m_actCount; m_iscountup = false; } }
    bool IsUndo() { return m_isUndo; }
    void UndoFinish() { m_isUndo = false; m_iscountup = true; }

    void Stackpushback(moveinfo _info) { m_stackMoveinfo.push_back(_info); }

    bool isgetProperty(RULE _rule) { return (m_uProperty & 1 << (UINT)_rule); }
    void Sentenceclear() { m_listSentence.clear(); }

    void sceneWin() { m_eScenestate = Scenestate::WIN; }
    wstring getMapname() { return m_sMapName; }
    void textsound();
    void sentence() { textcheck(); sentencedelete(); sentencecheck(); sentenceapply(); };

private:
    void makeText(Text _text, Vec2 _matrix);
    void makeUnit(Thingtype _thingtype, Vec2 _matrix, DIR _dir = DIR::RIGHT);
    bool thingMove(CThing* _thing, DIR _dir, bool _you = true);
    void movethingMove(CThing* _thing);
    void textcheck();                                       //textlist 작성
    void sentencecheck();                                   //문장 모두 추출
    void sentencedelete();                                  //원래 있던 문장 다 제거
    void sentenceapply();                                   //문장 적용
    bool sentenceorganize(list<CThing*> _list);             //문장 정리
    
    void addProperty(RULE _rule) { m_uProperty |= 1 << (UINT)_rule; }
 
public:
    virtual void tick() override;
    virtual void enter() override;
    void enter(wstring _stage);
    virtual void exit() override;
    virtual void render(HDC _dc) override;

public:
    CScene_Stage();
    ~CScene_Stage();
};

