#pragma once

/*

# RSDKv5(U) DECOMPILATION SOURCE CODE LICENSE v2.1

The code in this repository is a decompilation of RSDK (Retro-Engine) version 5 and 5-Ultimate.
There is original code in this repo, but most of the code is to be functionally the same as the version of RSDK this repo specifies.

This code is provided as-is, that is to say, without liability or warranty.
Original authors of RSDK and authors of the decompilation are not held responsible for any damages or other claims said.

You may copy, modify, contribute, and distribute, for public or private use, **as long as the following are followed:**
- All pre-built executables provided TO ANYONE *PRIVATE OR OTHERWISE* **must be built with DLC disabled by __default.__**
  - DLC is managed by the DummyCore Usercore. A define, `RSDK_AUTOBUILD`, and a CMake flag, `RETRO_DISABLE_PLUS`, are already provided for you to force DLC off.
  - Creating a configuration setting *is allowed,* so long as it is set to off by default.
    - *No such configuration will be pushed to the master repository.*
  - This is to ensure an extra layer of legal protection for Sonic Mania Plus and Sonic Origins Plus.
- You may not use the decompilation for commercial (any sort of profit) use.
- You must clearly specify that the decompilation and original code are not yours: the developers of both must be credited.
- You may not distribute assets used to run any game not directly provided by the repository (other than unique, modded assets).
- This license must be in all modified copies of source code, and all forks must follow this license.

Original RSDK authors: Evening Star

Decompilation authors: Rubberduckycooly and chuliRMG

*/

// Thanks Mania/RSSDKv5 decomp team for giving away such valuable information.
typedef long long int64;
typedef unsigned long long uint64;
typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef signed char int8;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef int bool32;

#define RETRO_REV02 1
#define RETRO_REV0U 1
#define TO_FIXED(x)   ((x) << 16)
#define RETRO_HASH_MD5(name) uint32 name[4]

enum FunctionTableIDs {
    FunctionTable_RegisterGlobalVariables,
    FunctionTable_RegisterObject,
#if RETRO_REV02
    FunctionTable_RegisterStaticVariables,
#endif
    FunctionTable_GetActiveEntities,
    FunctionTable_GetAllEntities,
    FunctionTable_BreakForeachLoop,
    FunctionTable_SetEditableVar,
    FunctionTable_GetEntity,
    FunctionTable_GetEntitySlot,
    FunctionTable_GetEntityCount,
    FunctionTable_GetDrawListRefSlot,
    FunctionTable_GetDrawListRef,
    FunctionTable_ResetEntity,
    FunctionTable_ResetEntitySlot,
    FunctionTable_CreateEntity,
    FunctionTable_CopyEntity,
    FunctionTable_CheckOnScreen,
    FunctionTable_CheckPosOnScreen,
    FunctionTable_AddDrawListRef,
    FunctionTable_SwapDrawListEntries,
    FunctionTable_SetDrawGroupProperties,
    FunctionTable_SetScene,
    FunctionTable_SetEngineState,
#if RETRO_REV02
    FunctionTable_ForceHardReset,
#endif
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_LoadScene,
    FunctionTable_FindObject,
    FunctionTable_ClearCameras,
    FunctionTable_AddCamera,
#if !RETRO_REV02
    FunctionTable_GetAPIFunction,
#endif
    FunctionTable_GetVideoSetting,
    FunctionTable_SetVideoSetting,
    FunctionTable_UpdateWindow,
    FunctionTable_Sin1024,
    FunctionTable_Cos1024,
    FunctionTable_ATan1024,
    FunctionTable_ASin1024,
    FunctionTable_ACos1024,
    FunctionTable_Sin512,
    FunctionTable_Cos512,
    FunctionTable_ATan512,
    FunctionTable_ASin512,
    FunctionTable_ACos512,
    FunctionTable_Sin256,
    FunctionTable_Cos256,
    FunctionTable_ATan256,
    FunctionTable_ASin256,
    FunctionTable_ACos256,
    FunctionTable_Rand,
    FunctionTable_RandSeeded,
    FunctionTable_SetRandSeed,
    FunctionTable_ATan2,
    FunctionTable_SetIdentityMatrix,
    FunctionTable_MatrixMultiply,
    FunctionTable_MatrixTranslateXYZ,
    FunctionTable_MatrixScaleXYZ,
    FunctionTable_MatrixRotateX,
    FunctionTable_MatrixRotateY,
    FunctionTable_MatrixRotateZ,
    FunctionTable_MatrixRotateXYZ,
    FunctionTable_MatrixInverse,
    FunctionTable_MatrixCopy,
    FunctionTable_InitString,
    FunctionTable_CopyString,
    FunctionTable_SetString,
    FunctionTable_AppendString,
    FunctionTable_AppendText,
    FunctionTable_LoadStringList,
    FunctionTable_SplitStringList,
    FunctionTable_GetCString,
    FunctionTable_CompareStrings,
    FunctionTable_GetDisplayInfo,
    FunctionTable_GetWindowSize,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
#if RETRO_REV02
    FunctionTable_SetScreenVertices,
#endif
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetTintLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
#if RETRO_REV02
    FunctionTable_LoadPalette,
#endif
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
#if RETRO_REV02
    FunctionTable_BlendColors,
#endif
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawFace,
    FunctionTable_DrawBlendedFace,
    FunctionTable_DrawSprite,
    FunctionTable_DrawDeformedSprite,
    FunctionTable_DrawString,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
#if RETRO_REV0U
    FunctionTable_DrawDynamicAniTile,
#endif
    FunctionTable_FillScreen,
    FunctionTable_LoadMesh,
    FunctionTable_Create3DScene,
    FunctionTable_Prepare3DScene,
    FunctionTable_SetDiffuseColor,
    FunctionTable_SetDiffuseIntensity,
    FunctionTable_SetSpecularIntensity,
    FunctionTable_AddModelToScene,
    FunctionTable_SetModelAnimation,
    FunctionTable_AddMeshFrameToScene,
    FunctionTable_Draw3DScene,
    FunctionTable_LoadSpriteAnimation,
    FunctionTable_CreateSpriteAnimation,
    FunctionTable_SetSpriteAnimation,
    FunctionTable_EditSpriteAnimation,
    FunctionTable_SetSpriteString,
    FunctionTable_FindSpriteAnimation,
    FunctionTable_GetFrame,
    FunctionTable_GetHitbox,
    FunctionTable_GetFrameID,
    FunctionTable_GetStringWidth,
    FunctionTable_ProcessAnimation,
    FunctionTable_GetTileLayerID,
    FunctionTable_GetTileLayer,
    FunctionTable_GetLayerSize,
    FunctionTable_GetTile,
    FunctionTable_SetTile,
    FunctionTable_CopyTileLayer,
    FunctionTable_ProcessParallax,
    FunctionTable_GetScanlines,
    FunctionTable_CheckObjectCollisionTouch,
    FunctionTable_CheckObjectCollisionCircle,
    FunctionTable_CheckObjectCollisionBox,
    FunctionTable_CheckObjectCollisionPlatform,
    FunctionTable_ObjectTileCollision,
    FunctionTable_ObjectTileGrip,
    FunctionTable_ProcessObjectMovement,
#if RETRO_REV0U
    FunctionTable_SetupCollisionConfig,
    FunctionTable_SetPathGripSensors,
    FunctionTable_FloorCollision,
    FunctionTable_LWallCollision,
    FunctionTable_RoofCollision,
    FunctionTable_RWallCollision,
    FunctionTable_FindFloorPosition,
    FunctionTable_FindLWallPosition,
    FunctionTable_FindRoofPosition,
    FunctionTable_FindRWallPosition,
#endif
    FunctionTable_GetTileAngle,
    FunctionTable_SetTileAngle,
    FunctionTable_GetTileFlags,
    FunctionTable_SetTileFlags,
#if RETRO_REV0U
    FunctionTable_CopyCollisionMask,
    FunctionTable_GetCollisionInfo,
#endif
    FunctionTable_GetSfx,
    FunctionTable_PlaySfx,
    FunctionTable_StopSfx,
    FunctionTable_PlayMusic,
    FunctionTable_SetChannelAttributes,
    FunctionTable_StopChannel,
    FunctionTable_PauseChannel,
    FunctionTable_ResumeChannel,
    FunctionTable_SfxPlaying,
    FunctionTable_ChannelActive,
    FunctionTable_GetChannelPos,
    FunctionTable_LoadVideo,
    FunctionTable_LoadImage,
#if RETRO_REV02
    FunctionTable_GetInputDeviceID,
    FunctionTable_GetFilteredInputDeviceID,
    FunctionTable_GetInputDeviceType,
    FunctionTable_IsInputDeviceAssigned,
    FunctionTable_GetInputDeviceUnknown,
    FunctionTable_InputDeviceUnknown1,
    FunctionTable_InputDeviceUnknown2,
    FunctionTable_GetInputSlotUnknown,
    FunctionTable_InputSlotUnknown1,
    FunctionTable_InputSlotUnknown2,
    FunctionTable_AssignInputSlotToDevice,
    FunctionTable_IsInputSlotAssigned,
    FunctionTable_ResetInputSlotAssignments,
#endif
#if !RETRO_REV02
    FunctionTable_GetUnknownInputValue,
#endif
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
#if RETRO_REV02
    FunctionTable_PrintLog,
    FunctionTable_PrintText,
    FunctionTable_PrintString,
    FunctionTable_PrintUInt32,
    FunctionTable_PrintInt32,
    FunctionTable_PrintFloat,
    FunctionTable_PrintVector2,
    FunctionTable_PrintHitbox,
#endif
    FunctionTable_SetActiveVariable,
    FunctionTable_AddEnumVariable,
#if !RETRO_REV02
    FunctionTable_PrintMessage,
#endif
#if RETRO_REV02
    FunctionTable_ClearDebugValues,
    FunctionTable_SetDebugValue,
#endif
#if RETRO_REV0U
    FunctionTable_NotifyCallback,
    FunctionTable_SetGameFinished,
    FunctionTable_StopAllSfx,
#endif
    FunctionTable_Count,
};

enum Alignments {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

enum InkEffects {
    INK_NONE,
    INK_BLEND,
    INK_ALPHA,
    INK_ADD,
    INK_SUB,
    INK_TINT,
    INK_MASKED,
    INK_UNMASKED,
};

struct Vector2
{
    int32 x;
    int32 y;
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

struct ScreenInfo
{
    uint16 frameBuffer[307200];
    Vector2 position;
    Vector2 size;
    Vector2 center;
    int32 pitch;
    int32 clipBound_X1;
    int32 clipBound_Y1;
    int32 clipBound_X2;
    int32 clipBound_Y2;
    int32 waterDrawPos;
};

struct Entity {
#if RETRO_REV0U
    // used for languages such as beeflang that always have vfTables in classes
    void* vfTable;
#endif
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
#if RETRO_REV02
    uint8 filter;
#endif
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

enum EngineStates {
    ENGINESTATE_LOAD,
    ENGINESTATE_REGULAR,
    ENGINESTATE_PAUSED,
    ENGINESTATE_FROZEN,
    ENGINESTATE_STEPOVER = 4,
    ENGINESTATE_DEVMENU = 8,
    ENGINESTATE_VIDEOPLAYBACK,
    ENGINESTATE_SHOWIMAGE,
#if RETRO_REV02
    ENGINESTATE_ERRORMSG,
    ENGINESTATE_ERRORMSG_FATAL,
#endif
    ENGINESTATE_NONE,
#if RETRO_REV0U
    // Prolly origins-only, called by the ending so I assume this handles playing ending movies and returning to menu
    ENGINESTATE_GAME_FINISHED,
#endif
};

enum RetroStates {
    ENGINE_DEVMENU = 0,
    ENGINE_MAINGAME = 1,
    ENGINE_INITDEVMENU = 2,

    ENGINE_SCRIPTERROR = 4,
};

enum StageModes {
    STAGEMODE_LOAD,
    STAGEMODE_NORMAL,
    STAGEMODE_PAUSED,

    STAGEMODE_STEPOVER = 8,
};

#define STORAGE_ENTRY_COUNT (0x1000)

enum StorageDataSets {
    DATASET_STG = 0,
    DATASET_MUS = 1,
    DATASET_SFX = 2,
    DATASET_STR = 3,
    DATASET_TMP = 4,
    DATASET_MAX, // used to signify limits
};

struct DataStorage {
    uint32* memoryTable;
    uint64 usedStorage;
    uint64 storageLimit;
    uint32** dataEntries[STORAGE_ENTRY_COUNT];   // pointer to the actual variable
    uint32* storageEntries[STORAGE_ENTRY_COUNT]; // pointer to the storage in "memoryTable"
    uint32 entryCount;
    uint32 clearCount;
};

struct SceneListInfo {
    RETRO_HASH_MD5(hash);
    char name[0x20];
    uint16 sceneOffsetStart;
    uint16 sceneOffsetEnd;
    uint8 sceneCount;
};

struct SceneListEntry {
    RETRO_HASH_MD5(hash);
    char name[0x20];
    char folder[0x10];
    char id[0x08];
};

struct SceneInfo {
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

struct ViewableVariable {
    char name[0x10];
    void* value;
    int32 type;
    int32 size;
    int32 min;
    int32 max;
};

// Doesn't appear to match
typedef enum {
    VIEWVAR_UINT8,
    VIEWVAR_UNKNOWN1,
    VIEWVAR_UNKNOWN2,
    VIEWVAR_UNKNOWN3,
    VIEWVAR_UNKNOWN4,
    VIEWVAR_UNKNOWN5,
    VIEWVAR_UNKNOWN6,
    VIEWVAR_BOOL,

    VIEWVAR_INVALID,
    VIEWVAR_UINT16,
    VIEWVAR_UINT32,
    VIEWVAR_INT8,
    VIEWVAR_INT16,
    VIEWVAR_INT32,
} ViewableVarTypes;

typedef enum {
    VIEWVAR_DISPLAY_BOOL,
    VIEWVAR_DISPLAY_UNSIGNED,
    VIEWVAR_DISPLAY_SIGNED,
} DebugVarDisplayTypes;

extern ControllerState* controller;
extern uint8& engineVersion;
extern ScreenInfo** currentScreen_ptr;
extern const char* gameTitle;
extern const char* version;
extern uint16* rgb32To16_B;
extern uint16* rgb32To16_G;
extern uint16* rgb32To16_R;
extern DataStorage* dataStorage;
extern void*& devmenuState;
extern SceneInfo& sceneInfo;
extern ScreenInfo* screens;
extern bool32* drawGroupVisible;