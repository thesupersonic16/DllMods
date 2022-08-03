#include <windows.h>
#include <ManiaModLoader.h>
#include <SonicMania.h>

#define AUTOCONFIG

extern "C"
{
    __declspec(dllexport) const HelperFunctions* MML_HelperFunctions = nullptr;

    bool TailsEnabled = false;
    bool TailsTailsEnabled = false;
    bool KnuxEnabled = false;
    bool MightyEnabled = false;
    bool RayEnabled = false;

    short GetSuperAnimation(SonicMania::EntityPlayer* player)
    {
        switch (player->Character)
        {
        case SonicMania::Character_Sonic:
            return SonicMania::GetSpritePointer(0xAC6838, 0xA0A);
        case SonicMania::Character_Tails:
            if (TailsEnabled)
                return SonicMania::LoadAnimation("Players/SuperTails.bin", SonicMania::Scope::Scope_Stage);
                if (TailsTailsEnabled)
                    return SonicMania::LoadAnimation("Players/SuperTailSprite.bin", SonicMania::Scope::Scope_Stage);
            break;
        case SonicMania::Character_Knux:
            if (KnuxEnabled)
                return SonicMania::LoadAnimation("Players/SuperKnux.bin", SonicMania::Scope::Scope_Stage);
            break;
        case SonicMania::Character_Mighty:
            if (MightyEnabled)
                return SonicMania::LoadAnimation("Players/SuperMighty.bin", SonicMania::Scope::Scope_Stage);
            break;
        case SonicMania::Character_Ray:
            if (RayEnabled)
                return SonicMania::LoadAnimation("Players/SuperRay.bin", SonicMania::Scope::Scope_Stage);
            break;
        default:
            break;
        }
        return -1;
    }

    void SetupSuperSprites(SonicMania::EntityPlayer* player)
    {
        if (!player->Transforming)
        {
            switch (player->Character)
            {
            case SonicMania::Character_Sonic:
                player->SpriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA0A);
                break;
            case SonicMania::Character_Tails:
                if (TailsEnabled)
                    player->SpriteIndex = SonicMania::LoadAnimation("Players/SuperTails.bin", SonicMania::Scope::Scope_Stage);
                    if (TailsTailsEnabled)
                        player->SpriteIndexTails = SonicMania::LoadAnimation("Players/SuperTailSprite.bin", SonicMania::Scope::Scope_Stage);
                break;
            case SonicMania::Character_Knux:
                if (KnuxEnabled)
                    player->SpriteIndex = SonicMania::LoadAnimation("Players/SuperKnux.bin", SonicMania::Scope::Scope_Stage);
                break;
            case SonicMania::Character_Mighty:
                if (MightyEnabled)
                    player->SpriteIndex = SonicMania::LoadAnimation("Players/SuperMighty.bin", SonicMania::Scope::Scope_Stage);
                break;
            case SonicMania::Character_Ray:
                if (RayEnabled)
                    player->SpriteIndex = SonicMania::LoadAnimation("Players/SuperRay.bin", SonicMania::Scope::Scope_Stage);
                break;
            default:
                break;
            }
        }
    }

    void SetupSuperSpriteRecovery(SonicMania::EntityPlayer* player)
    {
        if (!player->Transforming)
        {
            short spriteIndex = 0;
            switch (player->Character)
            {
            case SonicMania::Character_Sonic:
                spriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA08);
                break;
            case SonicMania::Character_Tails:
                if (TailsEnabled)
                    spriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA0C);
                if (TailsTailsEnabled)
                    player->SpriteIndexTails = SonicMania::LoadAnimation("Players/TailSprite.bin", SonicMania::Scope::Scope_Stage);
                break;
            case SonicMania::Character_Knux:
                if (KnuxEnabled)
                    spriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA10);
                break;
            case SonicMania::Character_Mighty:
                if (MightyEnabled)
                    spriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA12);
                break;
            case SonicMania::Character_Ray:
                if (RayEnabled)
                    spriteIndex = SonicMania::GetSpritePointer(0xAC6838, 0xA14);
                break;
            default:
                break;
            }
            if (spriteIndex)
            {
                player->SpriteIndex = spriteIndex;
                if (!player->Animation.CurrentAnimation)
                    player->Animation.CurrentFrame = 0;
                SonicMania::SetSpriteAnimation(player->SpriteIndex, player->Animation.CurrentAnimation, &player->Animation, 1, player->Animation.CurrentFrame);
            }
        }
    }

    FunctionPointer(int, sub_4CAD80, (), 0x000CAD80);
    DataPointer(SonicMania::EntityPlayer**, CurrentPlayer, 0x00AA7634);

    int EnforceSuperAnimations()
    {
        auto player = *CurrentPlayer;
        //printf("EnforceSuperAnimations: IsSuper? %s\n", player->SuperState == SonicMania::SuperState_Active ? "Yes" : "No");
        if (player->SuperState == SonicMania::SuperState_Active)
        {
            short superSpriteIndex = GetSuperAnimation(player);
            //printf("EnforceSuperAnimations: %d\n", superSpriteIndex);
            if (superSpriteIndex != -1)
                player->SpriteIndex = superSpriteIndex;
        }
        return sub_4CAD80();
    }

    __declspec(dllexport) void PostInit(const char* path)
    {
#ifdef AUTOCONFIG
        if (MML_HelperFunctions)
        {
            TailsEnabled = MML_HelperFunctions->CheckFile("Data/Sprites/Players/SuperTails.bin");
            TailsTailsEnabled = MML_HelperFunctions->CheckFile("Data/Sprites/Players/SuperTailSprite.bin");
            KnuxEnabled = MML_HelperFunctions->CheckFile("Data/Sprites/Players/SuperKnux.bin");
            MightyEnabled = MML_HelperFunctions->CheckFile("Data/Sprites/Players/SuperMighty.bin");
            RayEnabled = MML_HelperFunctions->CheckFile("Data/Sprites/Players/SuperRay.bin");
        }
        else
        {
            MessageBoxA(NULL, "SuperSpritesV2 was compiled with the AUTOCONFIG macro\n which requires a later version of the modloader\n Please update the modloader before continuing.", "SuperSpritesV2 Error", MB_ICONERROR);
        }
#endif // AUTOCONFIG
    }

    __declspec(dllexport) void Init(const char* path)
    {
        // Setup
        BYTE patch[] { 0x83, 0xC4, 0x04, 0xEB, 0x1A };
        WriteData((BYTE*)(baseAddress + 0x000C84D0), (BYTE)0x56);        // push esi - player
        WriteCall((void*)(baseAddress + 0x000C84D1), SetupSuperSprites); // call SetupSuperSprites
        WriteData((BYTE*)(baseAddress + 0x000C84D6), patch);             // add esi, 4
                                                                         // jmp SonicMania.exe+000C84F5
        WriteData<5>((BYTE*)(baseAddress + 0x000C84DB), (BYTE)0x90);     // nop - clean up

        // Recovery
        BYTE patch2[]{ 0x83, 0xC4, 0x04, 0xEB, 0x45 };
        WriteData((BYTE*)(baseAddress + 0x000C7825), (BYTE)0x56);               // push esi - player
        WriteCall((void*)(baseAddress + 0x000C7826), SetupSuperSpriteRecovery); // call SetupSuperSpriteRecovery
        WriteData((BYTE*)(baseAddress + 0x000C782B), patch2);                   // add esi, 4
                                                                                // jmp SonicMania.exe+000C7875
        WriteData<5>((BYTE*)(baseAddress + 0x000C7830), (BYTE)0x90);            // nop - clean up

        WriteJump((void*)(baseAddress + 0x000CEC0D), EnforceSuperAnimations);
    }

    __declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };

}
