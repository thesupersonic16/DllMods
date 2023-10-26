#include "pch.h"
#include "SigScan.h"
#include "Mod.hpp"
#include "Data.hpp"

ControllerState* controller = (ControllerState*)0x143729A90;
uint8& engineVersion = *(uint8*)0x143DB1B78;
ScreenInfo** currentScreen_ptr = (ScreenInfo**)0x14371C360;
const char* gameTitle = (const char*)0x143380830;
const char* version = (const char*)0x143380970;
uint16* rgb32To16_B = (uint16*)0x14371C870;
uint16* rgb32To16_G = (uint16*)0x14371C670;
uint16* rgb32To16_R = (uint16*)0x14371C470;
DataStorage* dataStorage = (DataStorage*)0x14372E2B0;
void*& devmenuState = *(void**)0x143363180;
SceneInfo& sceneInfo = *(SceneInfo*)0x143DB14E0;
ScreenInfo* screens = (ScreenInfo*)0x1434C42A0;
bool32* drawGroupVisible = (bool32*)0x1434C0220;
RetroStates& gameMode = *(RetroStates*)0x1428EDF38;
StageModes& stageMode = *(StageModes*)0x142C64CD0;
int32& gameSpeed = *(int32*)0x142856584;
bool& frameStep = *(bool*)0x143380981;