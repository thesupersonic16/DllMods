#include "pch.h"
#include "SigScan.h"
#include "Mod.hpp"
#include "Data.hpp"

ControllerState* controller = (ControllerState*)0x143728FF0;
uint8& engineVersion = *(uint8*)0x143DB10D8;
ScreenInfo** currentScreen_ptr = (ScreenInfo**)0x14371B8C0;
const char* gameTitle = (const char*)0x14337FD90;
const char* version = (const char*)0x14337FED0;
uint16* rgb32To16_B = (uint16*)0x14371BDD0;
uint16* rgb32To16_G = (uint16*)0x14371BBD0;
uint16* rgb32To16_R = (uint16*)0x14371B9D0;
DataStorage* dataStorage = (DataStorage*)0x14372D810;
void*& devmenuState = *(void**)0x1433626E0;
SceneInfo& sceneInfo = *(SceneInfo*)0x143DB0A40;
ScreenInfo* screens = (ScreenInfo*)0x1434C3800;
bool32* drawGroupVisible = (bool32*)0x1434BF780;