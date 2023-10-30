#pragma once

// RSDK Macros
#define RETRO_REV02 1
#define RETRO_REV0U 1
#define TO_FIXED(x)   ((x) << 16)
#define RETRO_HASH_MD5(name) uint32 name[4]

// RSDK Primitive Types
typedef int int32;
typedef int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int bool32;

// Others
typedef enum
{
    SUPERSTATE_NONE,
    SUPERSTATE_FADEIN,
    SUPERSTATE_SUPER,
    SUPERSTATE_FADEOUT,
    SUPERSTATE_DONE,
} SuperStates;

enum CharacterIDs
{
    ID_NONE = 0x0,
    ID_SONIC = 0x1,
    ID_TAILS = 0x2,
    ID_KNUCKLES = 0x4,
    ID_AMY = 0x8,
};

enum GameModes
{
    MODE_NOSAVE = 0x0,
    MODE_SAVEGAME = 0x1,
    MODE_TIMEATTACK = 0x2
};


// RSDKv5 Structures
struct Animator
{
    void* frames;
    int32 frameID;
    int16 animationID;
    int16 prevAnimationID;
    int16 speed;
    int16 timer;
    int16 frameDuration;
    int16 frameCount;
    uint8 loopIndex;
    uint8 rotationStyle;
};

struct Vector2
{
    int32 x;
    int32 y;
};

struct __declspec(align(8)) StateMachine
{
    void(__fastcall* state)();
    int timer;
    char field_C;
    char field_D;
    char field_E;
    char priority;
};

struct Entity
{
    void* vfTable;
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int32 angle;
    int32 alpha;
    int32 rotation;
    int32 groundVel;
    int32 zdepth;
    uint16 group;
    uint16 classID;
    bool32 inRange;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    uint8 active;
    uint8 filter;
    uint8 direction;
    uint8 drawGroup;
    uint8 collisionLayers;
    uint8 collisionPlane;
    uint8 collisionMode;
    uint8 drawFX;
    uint8 inkEffect;
    uint8 visible;
    uint8 onScreen;
};

struct EntityCamera : Entity
{
    StateMachine state;
    Entity* target;
    int32 screenID;
    Vector2 center;
    Vector2 targetMoveVel;
    Vector2 lastPos;
    Vector2 shakePos;
    Vector2 lookPos;
    Vector2 offset;
    bool32 disableYOffset;
    int32 centerY;
    int32 adjustY;
    int32 lerpPercent;
    int32 lerpSpeed;
    int32 lerpType;
    Vector2 endLerpPos;
    Vector2 startLerpPos;
    Vector2 boundsOffset;
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
};

struct EntityPlayer : Entity
{
    char padding0[160];
    StateMachine state;
    StateMachine nextAirState;
    StateMachine nextGroundState;
    EntityCamera* camera;
    Animator animator;
    Animator tailAnimator;
    char padding2[30];
    uint16 playerID;
    char padding3[92];
    int32 superState;
    char padding4[168];
    int32 controllerID;
    int32 controlLock;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    bool32 jumpHold;
    char padding5[244];
    int32 chargeTimer;
};

struct ObjectZone
{
    char padding0[336];
    int32 cameraBoundsL[4];
    int32 cameraBoundsR[4];
    int32 cameraBoundsT[4];
    int32 cameraBoundsB[4];
    int32 playerBoundsL[4];
    int32 playerBoundsR[4];
    int32 playerBoundsT[4];
};

struct InputState
{
    bool32 down;
    bool32 press;
    int32 keyMap;
};

struct ControllerState
{
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyA;
    InputState keyB;
    InputState keyC;
    InputState keyX;
    InputState keyY;
    InputState keyZ;
    InputState keyStart;
    InputState keySelect;
};

struct SceneListInfo
{
    RETRO_HASH_MD5(hash);
    char name[0x20];
    uint16 sceneOffsetStart;
    uint16 sceneOffsetEnd;
    uint8 sceneCount;
};

struct SceneListEntry
{
    RETRO_HASH_MD5(hash);
    char name[0x20];
    char folder[0x10];
    char id[0x08];
};


struct SceneInfo
{
    Entity* entity;
    SceneListEntry* listData;
    SceneListInfo* listCategory;
    int32 timeCounter;
    int32 currentDrawGroup;
    int32 currentScreenID;
    uint16 listPos;
    uint16 entitySlot;
    uint16 createSlot;
    uint16 classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalObjects;
    bool32 timeEnabled;
    uint8 activeCategory;
    uint8 categoryCount;
    uint8 state;
#if RETRO_REV02
    uint8 filter;
#endif
    uint8 milliseconds;
    uint8 seconds;
    uint8 minutes;
};

struct EntityActClear
{
    Entity entity;
    StateMachine(state);
    int32 timer;
    int32 stageFinishTimer;
    int32 timeBonus;
    int32 ringBonus;
    int32 coolBonus;
    int32 totalScore;
    int32 time;
    int32 newRecordTimer;
    bool32 achievedRank;
    bool32 isNewRecord;
    bool32 showCoolBonus;
    Vector2 playerNamePos;
    Vector2 gotThroughPos;
    Vector2 timeBonusPos;
    Vector2 ringBonusPos;
    Vector2 coolBonusPos;
    Vector2 totalScorePos;
    EntityPlayer* targetPlayer;
    // ...
};

struct GlobalVariables
{
    GameModes gameMode;
    CharacterIDs playerID;
    int specialCleared;
    int specialRingID;
    int blueSpheresID;
    int blueSpheresInit;
    int atlEnabled;
    int atlEntityCount;
    int atlEntitySlot[288];
    void* atlEntityData[288][276];
    int saveLoaded;
    int saveRAM[7168];
    int saveSlotID;
    int noSaveSlot[256];
    int menuParam[16384];
    int itemMode;
    int suppressTitlecard;
    int suppressAutoMusic;
    int competitionSession[16384];
    int medalMods;
    // ...
};
