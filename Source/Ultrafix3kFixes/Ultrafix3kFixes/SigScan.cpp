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
SIG_SCAN(SigPlayer_State_KnuxGlideLeft,  "\x48\x8B\xC4\x53\x57\x48\x83\xEC\x68\x48\x89\x68\x20\x48\x8B\xD9\x48\x89\x70\xE8\x4C\x89\x60\xE0\x4C\x89\x68\xD8\x4C\x89\x70\xD0\x4C\x89\x78\xC8\x48\x8D\x05\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?");
SIG_SCAN(Sigsub_1401EA5E0             ,  "\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\xE8\x00\x00\x00\x00\x83\xBB\x00\x00\x00\x00\x00\x74\x41\x80\xBB\x00\x00\x00\x00\x00\x74\x66\xFF\x83\x00\x00\x00\x00\x83\xBB", "xxxxxxxxxx????xx?????xxxx?????xxxx????xx");
SIG_SCAN(SigCamera_ShakeScreen        ,  "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x41\x8B\xE9\x41\x8B\xF8\x8B\xF2\x8B\xD9\x48\x85\xC0", "xxxx?xxxx?xxxx?xxxxxxxx????xxxxxxxxxxxxx");
SIG_SCAN(Sigsub_1403A2550             ,  "\x33\xC0\x83\xF9\x02\x0F\x42\xC1\x48\x8D\x0D\x00\x00\x00\x00\x48\x6B\xC0\x64\x48\x03\xC1\xC3", "xxxxxxxxxxx????xxxxxxxx");
SIG_SCAN(SigIsMirrorMode              ,  "\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x74\x0A\x48\x8B\x01\x48\xFF\xA0\x98\x00\x00\x00", "xxx????xxxxxxxxxxxxxxx");
SIG_SCAN(SigSpecialClear_State_FigureOutWhatToDoNext_D0, "\x83\xF9\x02\x0F\x84\x00\x00\x00\x00\xBD\x00\x00\x00\x00\x83\xF9\x08\x0F\x84\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x45\x33\xC9\xC7\x44\x24\x00\x00\x00\x00", "xxxxx????x????xxxxx????xxx????xxxxxx????");
SIG_SCAN(SigPlayer_Create_266         ,  "\xC7\x86\x00\x00\x00\x00\x00\x00\x02\x00\x48\x8B\x05\x00\x00\x00\x00\x83\x38\x03\x75\x09\xF6\x80\x00\x00\x00\x00\x00\x74\x16\x48\x8D\x8E\x00\x00\x00\x00\x45\x33", "xx??????xxxxx????xxxxxxx?????xxxxx????xx");
SIG_SCAN(SigActClear_Create           ,  "\x40\x53\x48\x83\xEC\x30\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xD9\x83\x78\x2C\x01\x75\x0D\xC6\x81\x00\x00\x00\x00\x00\x48\x83\xC4\x30\x5B\xC3\x45\x33\xC0\x80\xB9", "xxxxxxxxx????xxxxxxxxxxx?????xxxxxxxxxxx");