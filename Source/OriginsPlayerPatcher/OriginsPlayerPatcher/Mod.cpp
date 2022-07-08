#include "pch.h"
#include <HiteModLoader.h>
#include <Helpers.h>
#include "SigScan.h"


struct Object
{
	unsigned __int16 objectID;
	char active;
	char field_3;
};

typedef unsigned int uint32;
typedef int int32;
typedef int bool32;

struct Vector2
{
	int x;
	int y;
};

struct Hitbox
{
	__int16 left;
	__int16 top;
	__int16 right;
	__int16 bottom;
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

struct __declspec(align(8)) ObjectPlayer
{
	Object base;
	int sonicPhysicsTable[64];
	int32 sonicPhysicsTable2P[24];
	int32 tailsPhysicsTable[64];
	int32 tailsPhysicsTable2P[24];
	int32 knuxPhysicsTable[64];
	int knuxPhysicsTable2P[24];
	int mightyPhysicsTable[64];
	int rayPhysicsTable[64];
	int32 superSonicPaletteIndex1;
	int32 superSonicPaletteIndex2;
	int32 superSonicPaletteIndex3;
	int superSonicPaletteIndex4;
	int32 superTailsPaletteIndex1;
	int superTailsPaletteIndex2;
	int32 superTailsPaletteIndex3;
	int superTailsPaletteIndex4;
	int32 superKnuxPaletteIndex1;
	int superKnuxPaletteIndex2;
	int32 superKnuxPaletteIndex3;
	int superKnuxPaletteIndex4;
	uint32  superSonicPalette[30];
	uint32 superSonicPalette_Water1[30];
	uint32 superSonicPalette_Water2[30];
	uint32 hyperSonicPalette[54];
	uint32 hyperSonicPalette_Water1[54];
	uint32 hyperSonicPalette_Water2[54];
	uint32* activeSuperSonicPalette;
	uint32* activeSuperSonicPalette_Water;
	uint32* activeHyperSonicPalette;
	uint32* activeHyperSonicPalette_Water;
	uint32 superTailsPalette[18];
	uint32 superTailsPalette_Water1[18];
	uint32 superTailsPalette_Water2[18];
	uint32* activeSuperTailsPalette;
	uint32* activeSuperTailsPalette_Water;
	uint32 superKnuxPalette[33];
	uint32 superKnuxPalette_Water1[33];
	uint32 superKnuxPalette_Water2[33];
	uint32* activeSuperKnuxPalette;
	uint32* activeSuperKnuxPalette_Water;
	float spindashChargeSpeeds[13];
	Hitbox shieldHitbox;
	Hitbox instaShieldHitbox;
	int cantSwap;
	int32 playerCount;
	int activePlayerCount;
	unsigned __int16 upState;
	unsigned __int16 downState;
	unsigned __int16 leftState;
	unsigned __int16 rightState;
	unsigned __int16 jumpPressState;
	unsigned __int16 jumpHoldState;
	int32 nextLeaderPosID;
	int lastLeaderPosID;
	Vector2 leaderPositionBuffer[16];
	Vector2 targetLeaderPosition;
	int32 autoJumpTimer;
	int32 respawnTimer;
	int32 aiInputSwapTimer;
	bool32 disableP2KeyCheck;
	int32 rings;
	int32 ringExtraLife;
	int powerups;
	int savedLives;
	int savedScore;
	int savedScore1UP;
	unsigned __int16 sonicFrames;
	unsigned __int16 superFrames;
	unsigned __int16 tailsFrames;
	unsigned __int16 tailsTailsFrames;
	unsigned __int16 knuxFrames;
	__int16 mightyFrames;
	__int16 rayFrames;
	__int16 sfxJump;
	unsigned __int16 sfxRoll;
	__int16 sfxCharge;
	unsigned __int16 sfxRelease;
	__int16 sfxPeelCharge;
	unsigned __int16 sfxPeelRelease;
	__int16 sfxDropdash;
	__int16 sfxLoseRings;
	__int16 sfxHurt;
	unsigned __int16 sfxPimPom;
	__int16 sfxSkidding;
	__int16 sfxGrab;
	__int16 sfxFlying;
	bool32 playingFlySFX;
	_WORD sfxTired;
	bool32 playingTiredSFX;
	unsigned __int16 sfxLand;
	__int16 sfxSlide;
	__int16 sfxOuttaHere;
	__int16 sfxTransform2;
	unsigned __int16 sfxSwap;
	__int16 sfxSwapFail;
	__int16 sfxMightyDeflect;
	__int16 sfxMightyDrill;
    unsigned __int16 sfxMightyLand;
	__int16 sfxMightyUnspin;
	unsigned __int16 sfxEarthquake;
	__int16 sfxUnknown;
	int32 raySwoopTimer;
	int32 rayDiveTimer;
	bool32 gotHit[4];
	StateMachine configureGhostCB;
	bool32(__fastcall * canSuperCB)(bool32 isHUD);
	int superDashCooldown;
	__int16 field_E8C;
	__int16 field_E8E;
	bool32 debugPathTracer;
	int field_E94;
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
	if (file && !Dumped)
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
	return playerVars;
}


extern "C" __declspec(dllexport) void Init(ModInfo* modInfo)
{
	ModLoaderData = modInfo->ModLoader;
	
	if (ModLoaderData == (void*)1)
	{
		MessageBoxW(nullptr, L"The installed version of HiteModLoader is too old for OriginsPlayerPatcher to run,\nplease update the modloader to " ML_VERSION " or newer.", L"OriginsPlayerPatcher Error", MB_ICONERROR);
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