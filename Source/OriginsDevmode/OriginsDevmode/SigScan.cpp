#include "pch.h"
#include "SigScan.h"
#include <Psapi.h>
#define _countof(a) (sizeof(a)/sizeof(*(a)))

bool SigValid = true;

MODULEINFO moduleInfo;

const MODULEINFO& getModuleInfo()
{
    if (moduleInfo.SizeOfImage)
        return moduleInfo;

    ZeroMemory(&moduleInfo, sizeof(MODULEINFO));
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &moduleInfo, sizeof(MODULEINFO));

    return moduleInfo;
}

void* sigScan(const char* signature, const char* mask)
{
    const MODULEINFO& moduleInfo = getModuleInfo();
    const size_t length = strlen(mask);

    for (size_t i = 0; i < moduleInfo.SizeOfImage; i++)
    {
        char* memory = (char*)moduleInfo.lpBaseOfDll + i;

        size_t j;
        for (j = 0; j < length; j++)
            if (mask[j] != '?' && signature[j] != memory[j])
                break;

        if (j == length)
            return memory;
    }

    return nullptr;
}

#define SIG_SCAN(x, ...) \
    void* x##Addr; \
    void* x() \
    { \
        static const char* x##Data[] = { __VA_ARGS__ }; \
        if (!x##Addr) \
        { \
            for (int i = 0; i < _countof(x##Data); i += 2) \
            { \
                x##Addr = sigScan(x##Data[i], x##Data[i + 1]); \
                if (x##Addr) \
                    return x##Addr; \
                } \
            SigValid = false; \
        } \
        return x##Addr; \
    }

// Scans
SIG_SCAN(SigStartGameObjects      , "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x66\x0F\x6F\x05\x00\x00\x00\x00\x33\xC0\x66\x89\x05\x00\x00\x00\x00\x66\x0F\x6F\xC8\x88\x05\x00\x00", "xxxx?xxxxxx????xxxx????xxxxx????xxxxxx??");
SIG_SCAN(SigDrawRectangle         , "\x40\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x10\x8B\x6C\x24\x70\x33\xC0\x48\x8B\x35\x00\x00\x00\x00\x45\x8B\xF1\x45\x8B\xD0\x8B\xF9\x41\xBF\x00\x00\x00\x00\x83", "xxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxx????x");
SIG_SCAN(SigNotifyCallback        , "\x4C\x8B\xDC\x49\x89\x5B\x18\x55\x48\x8B\xEC\x48\x81\xEC\x00\x00\x00\x00\x8B\x01\x48\x8B\xD9\x3D\x00\x00\x00\x00\x0F\x8F\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxxxx????xx????xx????");
SIG_SCAN(SigLoadSceneFolder       , "\x40\x55\x53\x57\x41\x54\x41\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x4D\xC7\xE8\x00\x00\x00\x00\x48\x8D\x4D\x17\xE8\x00\x00\x00\x00\x45\x33", "xxxxxxxxxxxx?xxx????xxxxx????xxxxx????xx");
SIG_SCAN(SigInitObjects           , "\x48\x83\xEC\x28\x48\x89\x5C\x24\x00\x33\xDB\x48\x89\x7C\x24\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB7\x15\x00\x00\x00\x00\x0F\xB7", "xxxxxxxx?xxxxxx?xx????????x????xxx????xx");
SIG_SCAN(SigProcessObjectDrawLists, "\x4C\x8B\xDC\x48\x83\xEC\x58\xF6\x05\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x4D\x89\x63\xF0\x48\x8D\x05\x00\x00\x00\x00\x45\x33\xE4\x48\x89\x05\x00\x00\x00", "xxxxxxxxx?????xx????xxxxxxx????xxxxxx???");