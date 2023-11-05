#include "pch.h"
#include <HiteModLoader.h>
#include <Helpers.h>
#include "SigScan.h"


struct ObjectPlayer
{
	char padding0[0x7E4];
	unsigned int superSonicPalette[30];
	unsigned int superSonicPalette_Water1[30];
	unsigned int superSonicPalette_Water2[30];
	unsigned int hyperSonicPalette[54];
	unsigned int hyperSonicPalette_Water1[54];
	unsigned int hyperSonicPalette_Water2[54];
	unsigned int* activeSuperSonicPalette;
	unsigned int* activeSuperSonicPalette_Water;
	unsigned int* activeHyperSonicPalette;
	unsigned int* activeHyperSonicPalette_Water;
	unsigned int superTailsPalette[18];
	unsigned int superTailsPalette_Water1[18];
	unsigned int superTailsPalette_Water2[18];
	unsigned int* activeSuperTailsPalette;
	unsigned int* activeSuperTailsPalette_Water;
	unsigned int superKnuxPalette[33];
	unsigned int superKnuxPalette_Water1[33];
	unsigned int superKnuxPalette_Water2[33];
	unsigned int* activeSuperKnuxPalette;
	unsigned int* activeSuperKnuxPalette_Water;
	unsigned int superAmyPalette[55];
	unsigned int superAmyPalette_Water1[55];
	unsigned int superAmyPalette_Water2[55];

};

bool Dumped = false;
ModLoader* ModLoaderData;
ObjectPlayer* Player;

// Specific player palattes 
unsigned int SuperKnuxPalette_WaterHCZ[33];
unsigned int SuperKnuxPalette_WaterICZ[33];

bool CheckFile(const char* path)
{
	char fullPath[0x400]{};
	memset(fullPath, 0, sizeof(fullPath));
	ModLoaderData->GetRedirectedPath(path, fullPath);
	if (fullPath[0] == 0)
		return false;
	return true;
}

void SavePalette(const char* path, unsigned int* palette, int count)
{
	char data[0x300]{};
	for (int i = 0; i < count; i++)
	{
		auto color = (char*)&palette[i];
		data[i * 3 + 0] = *(color + 2);
		data[i * 3 + 1] = *(color + 1);
		data[i * 3 + 2] = *(color + 0);
	}
	FILE* file = nullptr;
	auto err = fopen_s(&file, path, "wb");
	if (file)
	{
		fwrite(data, 3, 0x100, file);
		fclose(file);
	}
}

bool LoadPalette(const char* path, unsigned int* palette, int count)
{
	char data[0x300]{};
	char fullPath[0x400]{};
	FILE* file = nullptr;

	// Dump Palettes
	ModLoaderData->GetRedirectedPath("DumpPalettes.txt", fullPath);
	auto err = fopen_s(&file, fullPath, "rb");
	if (file)
	{
		fclose(file);
		SavePalette(path, palette, count);
		Dumped = true;
	}
	file = nullptr;

	// Load Palette
	memset(fullPath, 0, sizeof(fullPath));
	ModLoaderData->GetRedirectedPath(path, fullPath);
	if (fullPath[0] == 0)
		return false;
	err = fopen_s(&file, fullPath, "rb");
	if (err)
		return false;

	printf("[PlayerPatcher] Loading Palette from %s\n", path);
	fread(data, 3, 0x100, file);
	fclose(file);
	for (int i = 0; i < count; i++)
	{
		auto color = (char*)&palette[i];
		*(color + 2) = data[i * 3 + 0];
		*(color + 1) = data[i * 3 + 1];
		*(color + 0) = data[i * 3 + 2];
	}
	return true;
}

// Specific player palattes 
HOOK(void, __fastcall, HCZSetup_StageLoad, SigHCZSetup_StageLoad(), void* obj)
{
	originalHCZSetup_StageLoad(obj);
	if (LoadPalette("SuperKnuxPalette_WaterHCZ.act", SuperKnuxPalette_WaterHCZ, sizeof(SuperKnuxPalette_WaterHCZ) / 4))
		Player->activeSuperKnuxPalette_Water = SuperKnuxPalette_WaterHCZ;
}

HOOK(void, __fastcall, ICZSetup_StageLoad, SigICZSetup_StageLoad(), void* obj)
{
	originalICZSetup_StageLoad(obj);
	if (LoadPalette("SuperKnuxPalette_WaterICZ.act", SuperKnuxPalette_WaterICZ, sizeof(SuperKnuxPalette_WaterICZ) / 4))
		Player->activeSuperKnuxPalette_Water = SuperKnuxPalette_WaterICZ;
}


HOOK(ObjectPlayer*, __fastcall, Player_StaticLoad, SigPlayer_StaticLoad(), ObjectPlayer* playerVars)
{
	originalPlayer_StaticLoad(playerVars);

	// Store player object
	Player = playerVars;

	// Load palettes
	LoadPalette("SuperSonicPalette.act", playerVars->superSonicPalette, sizeof(playerVars->superSonicPalette) / 4);
	LoadPalette("SuperSonicPalette_Water1.act", playerVars->superSonicPalette_Water1, sizeof(playerVars->superSonicPalette_Water1) / 4);
	LoadPalette("SuperSonicPalette_Water2.act", playerVars->superSonicPalette_Water2, sizeof(playerVars->superSonicPalette_Water2) / 4);
	LoadPalette("HyperSonicPalette.act", playerVars->hyperSonicPalette, sizeof(playerVars->hyperSonicPalette) / 4);
	LoadPalette("HyperSonicPalette_Water1.act", playerVars->hyperSonicPalette_Water1, sizeof(playerVars->hyperSonicPalette_Water1) / 4);
	LoadPalette("HyperSonicPalette_Water2.act", playerVars->hyperSonicPalette_Water2, sizeof(playerVars->hyperSonicPalette_Water2) / 4);
	LoadPalette("SuperTailsPalette.act", playerVars->superTailsPalette, sizeof(playerVars->superTailsPalette) / 4);
	LoadPalette("SuperTailsPalette_Water1.act", playerVars->superTailsPalette_Water1, sizeof(playerVars->superTailsPalette_Water1) / 4);
	LoadPalette("SuperTailsPalette_Water2.act", playerVars->superTailsPalette_Water2, sizeof(playerVars->superTailsPalette_Water2) / 4);
	LoadPalette("SuperKnuxPalette.act", playerVars->superKnuxPalette, sizeof(playerVars->superKnuxPalette) / 4);
	LoadPalette("SuperKnuxPalette_Water1.act", playerVars->superKnuxPalette_Water1, sizeof(playerVars->superKnuxPalette_Water1) / 4);
	LoadPalette("SuperKnuxPalette_Water2.act", playerVars->superKnuxPalette_Water2, sizeof(playerVars->superKnuxPalette_Water2) / 4);
	LoadPalette("SuperAmyPalette.act", playerVars->superAmyPalette, sizeof(playerVars->superAmyPalette) / 4);
	LoadPalette("SuperAmyPalette_Water1.act", playerVars->superAmyPalette_Water1, sizeof(playerVars->superAmyPalette_Water1) / 4);
	LoadPalette("SuperAmyPalette_Water2.act", playerVars->superAmyPalette_Water2, sizeof(playerVars->superAmyPalette_Water2) / 4);
	return playerVars;
}

extern "C" __declspec(dllexport) void PostInit()
{
	// Install hooks
	INSTALL_HOOK(Player_StaticLoad);

	// Specific zone palettes
	INSTALL_HOOK(HCZSetup_StageLoad);
	INSTALL_HOOK(ICZSetup_StageLoad);
}

extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	ModLoaderData = modInfo->ModLoader;
	
	if (ModLoaderData == (void*)1)
	{
		MessageBoxW(nullptr, L"The installed version of HiteModLoader is too old for OriginsPlayerPatcher to run,\n\nPlease update the modloader to " ML_VERSION " or newer.", L"OriginsPlayerPatcher Error", MB_ICONERROR);
		return;
	}

	// Scan signatures
	SigPlayer_StaticLoad();
	SigHCZSetup_StageLoad();
	SigICZSetup_StageLoad();

	// Check signatures
	if (!SigValid)
	{
		MessageBoxW(nullptr, L"Signature Scan Failed!\n\nThis usually means there is a conflict or the mod is running on an incompatible game version.", L"Scan Error", MB_ICONERROR);
		return;
	}

	ModLoaderData = modInfo->ModLoader;
}