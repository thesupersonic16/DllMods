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

void LoadPalette(const char* path, unsigned int* palette, int count)
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
		return;
	err = fopen_s(&file, fullPath, "rb");
	if (!err)
	{
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
	}
}

HOOK(ObjectPlayer*, __fastcall, Player_StaticLoad, SigPlayer_StaticLoad(), ObjectPlayer* playerVars)
{
	originalPlayer_StaticLoad(playerVars);

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


extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	ModLoaderData = modInfo->ModLoader;
	
	if (ModLoaderData == (void*)1)
	{
		MessageBoxW(nullptr, L"The installed version of HiteModLoader is too old for OriginsPlayerPatcher to run,\n\nPlease update the modloader to " ML_VERSION " or newer.", L"OriginsPlayerPatcher Error", MB_ICONERROR);
		return;
	}

	// Check signatures
	if (!SigValid)
	{
		MessageBoxW(nullptr, L"Signature Scan Failed!\n\nThis usually means there is a conflict or the mod is running on an incompatible game version.", L"Scan Error", MB_ICONERROR);
		return;
	}

	INSTALL_HOOK(Player_StaticLoad);
	ModLoaderData = modInfo->ModLoader;
}