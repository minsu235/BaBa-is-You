#pragma once


#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
						private:\
							type();\
							~type();

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define IsPressed(KEY) KEY_STATE::PRESS == CKeyMgr::GetInst()->GetKeyState(KEY)
#define IsTap(KEY) KEY_STATE::TAP == CKeyMgr::GetInst()->GetKeyState(KEY)
#define IsRelease(KEY) KEY_STATE::RELEASE == CKeyMgr::GetInst()->GetKeyState(KEY)

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;
#define CLONE(type) virtual type* Clone() override { return new type(*this); }

#define MAX_LAYER 16
#define PI 3.14159265f

#define TILE_SIZE 32
#define LIMIT -1
#define LIMITVEC Vec2(-1, -1)

enum SCENE_TYPE
{
	TOOL,
	LOGO,
	WORLD,
	STAGE,
	END,
};

enum class PEN_COLOR
{
	RED,
	GREEN,
	BLUE,
	BLACK,
	DEFAULT,
	END,
};

enum class BRUSH_COLOR
{
	BLACK,  // 검은 브러쉬
	DEFAULT, // 기본 브러쉬


	HOLLOW,
	END,
};

enum class COMPONENT_TYPE
{	
	ANIMATOR,	
	RIGIDBODY,	
	COLLIDER,
	END,
};

enum class LAYER_TYPE
{
	DEFAULT,
	BACKGROUND,
	TILE,
	STICKOBJ,
	OBJECT,
	YOU,
	TEXT,
	FLOATSTICKOBJ,
	FLOATOBJECT,
	FLOATYOU,
	FLOATTEXT,
	EFFECT,
	UI,

	MENU,
	FADEINOUT,
	END = MAX_LAYER,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT, // first : Object Adress, second : Layer Type
	DELETE_OBJECT, // first : Object Adress, 
	SCENE_CHANGE,  // first : Scene Type
	LAYER_CHANGE_YOU,
	LAYER_CHANGE_OBJ,
	ALL_DELETE,
	RESTART,
	DEAD_THING, // first : Thing Adress,
	WIN,

	END,
};

enum class RULE_EVENT_TYPE
{
	ALIVE_THING,
	
	MOVE_MAP,
	
	SUB_RULE,
	ADD_RULE,
	ADD_RULE_OO,
	CHANGE_UNIT,
	CHANGE_LAYER,
	ADD_RULE_OP,
	TEXT_OFF,
	TEXT_ON,
	SINK,
	SHUTOPEN,
	MELT,
	DEFEAT,
	WIN,
	UNDO_FINISH,
	END,
};

enum class DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE,
};

enum class RULE
{
	YOU,
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
	WIN,

	ME,
	END,
};

enum class MOVE
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

enum class Texttype
{
	OBJECT,
	IS,
	AND,
	PROPERTY,
	ADJUNCT,
	NONE,
};

enum class Scenestate
{
	PLAY,
	ESC,
	RESTART,
	RESTARTFADEOUT,
	MAPFADEOUT,
	MENUFADEOUT,
	CHANGE,
	WIN,
};