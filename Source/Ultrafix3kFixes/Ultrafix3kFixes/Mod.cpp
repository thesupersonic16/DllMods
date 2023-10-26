#include "pch.h"
#include <HiteModLoader.h>
#include <Helpers.h>
#include "SigScan.h"

// Thanks Mania/RSSDKv5 decomp team for giving away such valuable information without permission.
typedef int int32;
typedef int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int bool32;

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

typedef enum {
    SUPERSTATE_NONE,
    SUPERSTATE_FADEIN,
    SUPERSTATE_SUPER,
    SUPERSTATE_FADEOUT,
    SUPERSTATE_DONE,
} SuperStates;

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

// Too lazy to sig scan these
ObjectZone** Zone = (ObjectZone**)0x143DB5CE0;
ControllerState* controllers = (ControllerState*)0x143729A90;

HOOK(void, __fastcall, Player_State_KnuxGlideLeft, SigPlayer_State_KnuxGlideLeft(), EntityPlayer* _this)
{
	originalPlayer_State_KnuxGlideLeft(_this);

	if (_this->position.x <= (*Zone)->playerBoundsL[_this->playerID] + 0x100000 && !_this->jumpHold)
	{
		_this->direction = 0;
	}
}

HOOK(void, __fastcall, sub_1401EA5E0, Sigsub_1401EA5E0(), EntityPlayer* player)
{
    if (player->onGround && player->chargeTimer >= 19 && player->jumpHold)
    {
        auto Camera_ShakeScreen = (void(__fastcall*)(int32 screen, int32 shakeX, int32 shakeY, int32 a4, int32 a5))(SigCamera_ShakeScreen());
        if (player->superState == SUPERSTATE_SUPER && player->camera)
            Camera_ShakeScreen(player->camera->screenID, 6, 6, 1, 1);
    }

    originalsub_1401EA5E0(player);
}

HOOK(bool *, __fastcall, sub_1403A2550, Sigsub_1403A2550(), uint32 deviceID)
{
    auto inputs = originalsub_1403A2550(deviceID);
    bool mirror = ((bool(__fastcall*)())(SigIsMirrorMode()))();

    if (deviceID == 0)
    {
        controllers[0].keyUp.press    = inputs[0];
        controllers[0].keyDown.press  = inputs[1];
        controllers[0].keyLeft.press  = inputs[2];
        controllers[0].keyRight.press = inputs[3];
        controllers[0].keyA.press     = inputs[4];
        controllers[0].keyB.press     = inputs[5];
        controllers[0].keyC.press     = inputs[6];
        controllers[0].keyStart.press = inputs[20];
    
        controllers[1].keyUp.press     = inputs[10] | inputs[0];
        controllers[1].keyDown.press   = inputs[11] | inputs[1];
        controllers[1].keyLeft.press   = inputs[12] | inputs[2];
        controllers[1].keyRight.press  = inputs[13] | inputs[3];
        controllers[1].keyA.press      = inputs[14];
        controllers[1].keyB.press      = inputs[15];
        controllers[1].keyC.press      = inputs[16];
        controllers[1].keyX.press      = inputs[17];
        controllers[1].keyY.press      = inputs[18];
        controllers[1].keyZ.press      = inputs[19];
        controllers[1].keyStart.press  = inputs[20];
        controllers[1].keySelect.press = inputs[21];

        if (mirror)
        {
            bool buffer = controllers[1].keyLeft.press;
            controllers[1].keyLeft.press = controllers[1].keyRight.press;
            controllers[1].keyRight.press = buffer;
        }

        // TODO: Not sure what this does, but from what it looks,
        //  it may only affect v4 and v3
        //sub_140101BC0(21);
    }

    // Intentionally fail if id is 0
    if (deviceID == 0)
        return nullptr;
    return inputs;
}

extern "C" __declspec(dllexport) void Init(ModInfo * modInfo)
{
    // Check signatures
    if (!SigValid)
    {
        MessageBoxW(nullptr, L"Signature Scan Failed!\n\nThis usually means there is a conflict or the mod is running on an incompatible game version.", L"Scan Error", MB_ICONERROR);
        return;
    }

    INSTALL_HOOK(Player_State_KnuxGlideLeft);
    INSTALL_HOOK(sub_1401EA5E0);
    INSTALL_HOOK(sub_1403A2550);

    // Fix SpecialClear
    for (int i = 0; i < 2; ++i)
        WRITE_MEMORY(((intptr_t)SigSpecialClear_State_FigureOutWhatToDoNext_D0() + 14 * i), 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90);
}