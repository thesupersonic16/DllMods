#pragma once
#include "Mod.hpp"
#include <map>
#include <windows.h>

namespace Platform
{
    enum PlatformType
    {
        Steam,
        Epic
    };

    PlatformType DetectPlatform();
    void CreateEngineInfo(EngineInfo* engineInfo, PlatformType platform);
    void ApplyPatches(PlatformType platform);
    void GetObjectStaticVars(std::map<uint32, void**>* vars, PlatformType platform);
    void ResetObjects(PlatformType platform);
    HMODULE LoadGameLogicFile(PlatformType platform);
}