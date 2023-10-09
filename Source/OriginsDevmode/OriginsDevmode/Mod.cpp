#include "pch.h"
#include <HiteModLoader.h>
#include <Helpers.h>
#include "SigScan.h"
#include "Data.hpp"
#include "Mod.hpp"
#include "Debug.hpp"

const char* drawGroupNames[16] = 
{
	"Draw Group 0", "Draw Group 1", "Draw Group 2", "Draw Group 3",
	"Draw Group 4", "Draw Group 5", "Draw Group 6", "Draw Group 7",
	"Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11",
	"Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

void** FunctionTable;
bool devButtonPressed = false;
bool fastButtonPressed = false;
bool functionButtonPressed[12]{};
int stepTimer = 0;

bool showPaletteOverlay = false;

// The engine normally clears the variables,
//  but since it does not use the function table it got nuked
//  so we have to readd it. For now a hook will do.
HOOK(void, __fastcall, LoadSceneFolder, SigLoadSceneFolder())
{
	ClearViewableVariables();
	originalLoadSceneFolder();
}

HOOK(void, __fastcall, InitObjects, SigInitObjects())
{
	originalInitObjects();
	AddViewableVariable("Show Palettes", &showPaletteOverlay, VIEWVAR_BOOL, false, true);
	for (int i = 0; i < 16; ++i)
		AddViewableVariable(drawGroupNames[i], &drawGroupVisible[i], VIEWVAR_BOOL, false, true);
}


HOOK(void, __fastcall, ProcessObjectDrawLists, SigProcessObjectDrawLists())
{
	originalProcessObjectDrawLists();

	if (showPaletteOverlay)
	{
		// Use first screen
		*currentScreen_ptr = &screens[0];
		auto GetPaletteEntry = (uint32(__fastcall*)(uint8 bankID, uint8 index))(FunctionTable[FunctionTable_GetPaletteEntry]);
		auto DrawRectangle = (void(__fastcall*)(int32 x, int32 y, int32 width, int32 height, uint32 color, int32 alpha, int32 inkEffect, bool32 screenRelative))(SigDrawRectangle());

		for (int32 p = 0; p < 8; ++p)
		{
			int32 x = 424 - 128;
			int32 y = 240 - 64;

			for (int32 i = 0; i < 16 * 16; ++i)
			{
				uint32 clr = GetPaletteEntry(p, i);

				DrawRectangle(x + (i & 0xF) * 2 + (p % 4) * 32,
					y + ((i >> 4) << 1) + ((p / 4) * 32), 2, 2, clr, 0xFF, INK_NONE, true);
			}
		}
	}

}


HOOK(void, __fastcall, StartGameObjects, SigStartGameObjects())
{
	originalStartGameObjects();

	FunctionTable = *(void***)0x142E6F6B0;
	FunctionTable[FunctionTable_ClearDebugValues] = ClearViewableVariables;
	FunctionTable[FunctionTable_SetDebugValue] = AddViewableVariable;
}

extern "C" __declspec(dllexport) void OnRsdkFrame()
{
	// Toggle dev menu
	bool devButtonDown = GetAsyncKeyState(VK_OEM_3) != 0;
	if (devButtonDown && !devButtonPressed)
	{
		ToggleDevMenu();
		devButtonPressed = true;
	}
	else if (!devButtonDown)
		devButtonPressed = false;

	// Fast forward
	bool fastButtonDown = GetAsyncKeyState(VK_BACK) != 0;
	if (fastButtonDown && !fastButtonPressed)
	{
		// You saw nothing
		*((intptr_t*)0x142855AF4) = 8;
		fastButtonPressed = true;
	}
	else if (!fastButtonDown && fastButtonPressed)
	{
		*((intptr_t*)0x142855AF4) = 1;
		fastButtonPressed = false;
	}
	else if (!fastButtonDown)
		fastButtonPressed = false;

	if (engineVersion == 5)
	{
		// Back scene
		bool f1ButtonDown = GetAsyncKeyState(VK_F1) != 0;
		if (f1ButtonDown && !functionButtonPressed[0])
		{
			sceneInfo.listPos--;
			if (sceneInfo.listPos < sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart
				|| sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd)
			{
				sceneInfo.activeCategory--;
				if (sceneInfo.activeCategory >= sceneInfo.categoryCount)
					sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
				sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd - 1;
			}
			sceneInfo.state = ENGINESTATE_LOAD;
			functionButtonPressed[0] = true;
		}
		else if (!f1ButtonDown)
			functionButtonPressed[0] = false;

		// Next scene
		bool f2ButtonDown = GetAsyncKeyState(VK_F2) != 0;
		if (f2ButtonDown && !functionButtonPressed[1])
		{
			sceneInfo.listPos++;
			if (sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd || sceneInfo.listPos == 0)
			{
				sceneInfo.activeCategory++;
				if (sceneInfo.activeCategory >= sceneInfo.categoryCount)
					sceneInfo.activeCategory = 0;
				sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart;
			}
			sceneInfo.state = ENGINESTATE_LOAD;
			functionButtonPressed[1] = true;
		}
		else if (!f2ButtonDown)
			functionButtonPressed[1] = false;

		// Reload scene
		bool f5ButtonDown = GetAsyncKeyState(VK_F5) != 0;
		if (f5ButtonDown && !functionButtonPressed[4])
		{
			sceneInfo.state = ENGINESTATE_LOAD;
			functionButtonPressed[4] = true;
		}
		else if (!f5ButtonDown)
			functionButtonPressed[4] = false;

		// Show palette overlay
		bool f10ButtonDown = GetAsyncKeyState(VK_F10) != 0;
		if (f10ButtonDown && !functionButtonPressed[9])
		{
			showPaletteOverlay = !showPaletteOverlay;
			functionButtonPressed[9] = true;
		}
		else if (!f10ButtonDown)
			functionButtonPressed[9] = false;

		// Frame step
		bool f11ButtonDown = GetAsyncKeyState(VK_F11) != 0;
		if (f11ButtonDown)
		{
			++stepTimer;
			if (!functionButtonPressed[10] || (stepTimer > 30 && (stepTimer & 1) == 0))
				(*(bool*)0x14337FEE1) = true;
			functionButtonPressed[10] = true;
		}
		else if (!f11ButtonDown)
		{
			stepTimer = 0;
			functionButtonPressed[10] = false;
		}

		// Pause
		bool f12ButtonDown = GetAsyncKeyState(VK_F12) != 0;
		if (f12ButtonDown && !functionButtonPressed[11])
		{
			if (sceneInfo.state != ENGINESTATE_NONE)
				sceneInfo.state ^= ENGINESTATE_STEPOVER;
			functionButtonPressed[11] = true;
		}
		else if (!f12ButtonDown)
			functionButtonPressed[11] = false;
	}

}


extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	// Check signatures
	if (!SigValid)
	{
		MessageBoxW(nullptr, L"Signature Scan Failed!\n\nThis usually means there is a conflict or the mod is running on an incompatible game version.", L"Scan Error", MB_ICONERROR);
		return;
	}

	INSTALL_HOOK(LoadSceneFolder);
	INSTALL_HOOK(InitObjects);
	INSTALL_HOOK(ProcessObjectDrawLists);
	INSTALL_HOOK(StartGameObjects);
}