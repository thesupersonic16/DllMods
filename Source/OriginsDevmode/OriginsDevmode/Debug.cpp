#include "pch.h"
#include <HiteModLoader.h>
#include <Helpers.h>
#include "SigScan.h"
#include "Data.hpp"
#include "Mod.hpp"
#include "Debug.hpp"
#include "DevFont.hpp"

int32 selection = 0;
int32 timer = 0;
int8 sceneState = 1;
int32 scrollPos;
int32 listPos;

int32 viewableVarCount;
ViewableVariable viewableVarList[0x100];

// Functions
auto DrawRectangle = (void(__fastcall*)(int32 x, int32 y, int32 width, int32 height, uint32 color, int32 alpha, int32 inkEffect, bool32 screenRelative))(SigDrawRectangle());
auto NotifyCallback = (void(__fastcall*)(int* args))(SigNotifyCallback());


void DrawDevString(const char* string, int32 x, int32 y, int32 align, uint32 color)
{
    ScreenInfo* currentScreen = *currentScreen_ptr;
    uint16 color16 = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    int32 charOffset = 0;
    bool32 linesRemain = true;
    while (linesRemain) {
        linesRemain = false;

        int32 lineSize = 0;
        char cur = string[charOffset];
        if (cur != '\n') {
            while (cur) {
                cur = string[++charOffset];
                lineSize++;
                if (cur == '\n') {
                    linesRemain = true;
                    break;
                }
            }
        }

        if (y >= 0 && y < currentScreen->size.y - 7) {
            int32 offset = 0;
            switch (align) {
            default:
            case ALIGN_LEFT: offset = 0; break;

            case ALIGN_CENTER: offset = 4 * lineSize; break;

            case ALIGN_RIGHT: offset = 8 * lineSize; break;
            }
            int32 drawX = x - offset;

            const char* curChar = &string[charOffset++ - lineSize];

            for (int32 c = 0; c < lineSize; ++c) {
                if (drawX >= 0 && drawX < currentScreen->size.x - 7) {
                    uint16* frameBuffer = &currentScreen->frameBuffer[drawX + y * currentScreen->pitch];

                    if ((*curChar < '\t' || *curChar > '\n') && *curChar != ' ') {
                        uint8* textStencilPtr = &devTextStencil[0x40 * *curChar];

                        for (int32 h = 0; h < 8; ++h) {
                            for (int32 w = 0; w < 8; ++w) {
                                if (*textStencilPtr)
                                    *frameBuffer = color16;

                                ++textStencilPtr;
                                ++frameBuffer;
                            }

                            frameBuffer += currentScreen->pitch - 8;
                        }
                    }

                    ++curChar;
                    drawX += 8;
                }
            }
        }

        y += 8;
        // TODO
    }
}

void DevMenu_OptionsMenu();
void DevMenu_CategorySelectMenu();
void DevMenu_SceneSelectMenu();
void DevMenu_DebugOptionsMenu();

void DevMenu_MainMenu()
{
    const ScreenInfo* currentScreen = *currentScreen_ptr;
    const int32 selectionCount = 5;
    uint32 selectionColors[] = { 0x808090, 0x808090, 0x808090, 0x808090, 0x808090 };
    const char* selectionNames[] = { "Resume", "Restart", "Stage Select", "Options", "Exit" };
    selectionColors[selection] = 0xF0F0F0;

    // Info Box
    int32 y = currentScreen->center.y - 80;
    DrawRectangle(currentScreen->center.x - 128, currentScreen->center.y - 84, 0x100, 0x30, 0x80, 0xFF, INK_NONE, true);
    DrawDevString("RETRO ENGINE v5U", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);

    y += 8;
    DrawDevString("Dev Menu", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);

    y += 8;
    y += 8;
    DrawDevString(gameTitle, currentScreen->center.x, y, ALIGN_CENTER, 0x808090);

    y += 8;
    DrawDevString(version, currentScreen->center.x, y, ALIGN_CENTER, 0x808090);

    // Selections Box
    y += 24;
    DrawRectangle(currentScreen->center.x - 128, y - 8, 0x100, 0x48, 0x80, 0xFF, INK_NONE, true);

    for (int32 i = 0; i < selectionCount; ++i)
    {
        DrawDevString(selectionNames[i], currentScreen->center.x, y, ALIGN_CENTER, selectionColors[i]);
        y += 12;
    }
    y += 20;

    // Storage box
    // Stage Storage
    int32 stgUsed = (int32)((sizeof(int32) * dataStorage[DATASET_STG].usedStorage) / (float)dataStorage[DATASET_STG].storageLimit * 126.0);
    DrawRectangle(currentScreen->center.x - 40, y, 0x80, 0x08, 0x80, 0xFF, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, stgUsed, 6, 0xF0F0F0, 0xFF, INK_NONE, true);
    DrawDevString("STG", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // Music Storage
    int32 musUsed = (int32)((sizeof(int32) * dataStorage[DATASET_MUS].usedStorage) / (float)dataStorage[DATASET_MUS].storageLimit * 126.0);
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 0x80, 0x08, 0x80, 0xFF5, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, musUsed, 6, 0xF0F0F0, 0xFF, INK_NONE, true);
    DrawDevString("MUS", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // SoundFX Storage
    int32 sfxUsed = (int32)((sizeof(int32) * dataStorage[DATASET_SFX].usedStorage) / (float)dataStorage[DATASET_SFX].storageLimit * 126.0);
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 0x80, 0x08, 0x80, 0xFF, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, sfxUsed, 6, 0xF0F0F0, 0xFF, INK_NONE, true);
    DrawDevString("SFX", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // String Storage
    int32 strUsed = (int32)((sizeof(int32) * dataStorage[DATASET_STR].usedStorage) / (float)dataStorage[DATASET_STR].storageLimit * 126.0);
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 0x80, 0x08, 0x80, 0xFF, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, strUsed, 6, 0xF0F0F0, 0xFF, INK_NONE, true);
    DrawDevString("STR", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // Temp Storage
    int32 tmpUsed = (int32)((sizeof(int32) * dataStorage[DATASET_TMP].usedStorage) / (float)dataStorage[DATASET_TMP].storageLimit * 126.0);
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 0x80, 0x08, 0x80, 0xFF, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, tmpUsed, 6, 0xF0F0F0, 0xFF, INK_NONE, true);
    DrawDevString("TMP", currentScreen->center.x - 64, y, 0, 0xF0F080);

    if (controller->keyUp.press)
    {
        selection--;
        timer = 1;

        if (selection < 0)
            selection += selectionCount;
    }
    else if (controller->keyUp.down)
    {
        if (timer)
            timer = ++timer & 7;
        else
        {
            selection--;
            timer = ++timer & 7;

            if (selection < 0)
                selection += selectionCount;
        }
    }

    if (controller->keyDown.press)
    {
        selection++;
        timer = 1;

        if (selection >= selectionCount)
            selection -= selectionCount;
    }
    else if (controller->keyDown.down)
    {
        if (timer)
            timer = ++timer & 7;
        else
        {
            selection++;
            timer = ++timer & 7;

            if (selection >= selectionCount)
                selection -= selectionCount;
        }
    }

    if (controller->keyStart.press || controller->keyA.press)
    {
        int args[3]{ 156, 1, 0 };
        switch (selection)
        {
        case 0: // Resume
            CloseDevMenu();
            break;
        case 1: // Restart
            switch (engineVersion)
            {
            default: break;
            case 5:
                sceneInfo.state = ENGINESTATE_LOAD;
                break;
            case 4:
            case 3:
                break;
            }
            break;
        case 2: // Stage Select
            devmenuState = DevMenu_CategorySelectMenu;
            selection = 0;
            timer = 1;
            scrollPos = 0;
            break;
        case 3: // Options
            devmenuState = DevMenu_OptionsMenu;
            selection = 0;
            timer = 1;
            break;
        case 4: // Exit
            NotifyCallback(args);
            break;
        default:
            break;
        }
    }
    else if (controller->keyB.press)
    {
        CloseDevMenu();
    }
}

void DevMenu_CategorySelectMenu()
{
    const ScreenInfo* currentScreen = *currentScreen_ptr;
    uint32 selectionColors[] = {
        0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090,
    };
    selectionColors[selection - scrollPos] = 0xF0F0F0;

    // Info Box
    int32 y = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, y - 84, 256, 0x30, 0x80, 0xFF, INK_NONE, true);

    y -= 68;
    DrawDevString("SELECT STAGE CATEGORY", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);

    // Selections Box
    y += 44;
    DrawRectangle(currentScreen->center.x - 128, y - 8, 0x100, 0x48, 0x80, 0xFF, INK_NONE, true);

    int32 y2 = y;
    for (int32 i = 0; i < 8; ++i)
    {
        if (scrollPos + i < sceneInfo.categoryCount)
        {
            DrawDevString(sceneInfo.listCategory[scrollPos + i].name, currentScreen->center.x - 64, y2, ALIGN_LEFT, selectionColors[i]);
            y2 += 8;
        }
    }
    y += 20;


    if (controller->keyUp.press)
    {
        if (--selection < 0)
            selection += sceneInfo.categoryCount;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyUp.down)
    {
        if (timer)
        {
            timer = (timer + 1) & 7;

            if (selection >= scrollPos) {
                if (selection > scrollPos + 7)
                    scrollPos = selection - 7;
            }
            else
            {
                scrollPos = selection;
            }
        }
        else
        {
            if (--selection < 0)
                selection += sceneInfo.categoryCount;

            timer = (timer + 1) & 7;

            if (selection >= scrollPos)
            {
                if (selection > scrollPos + 7)
                    scrollPos = selection - 7;
            }
            else
            {
                scrollPos = selection;
            }
        }
    }

    if (controller->keyDown.press)
    {
        if (++selection == sceneInfo.categoryCount)
            selection = 0;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyDown.down)
    {
        if (timer)
        {
            timer = (timer + 1) & 7;

            if (selection >= scrollPos)
            {
                if (selection > scrollPos + 7)
                    scrollPos = selection - 7;
            }
            else
            {
                scrollPos = selection;
            }
        }
        else
        {
            if (++selection == sceneInfo.categoryCount)
                selection = 0;

            timer = (timer + 1) & 7;

            if (selection >= scrollPos)
            {
                if (selection > scrollPos + 7)
                    scrollPos = selection - 7;
            }
            else
            {
                scrollPos = selection;
            }
        }
    }

    if (controller->keyStart.press || controller->keyA.press)
    {
        if (sceneInfo.listCategory[selection].sceneCount)
        {
            devmenuState = DevMenu_SceneSelectMenu;
            listPos = selection;
            scrollPos = 0;
            selection = 0;
        }
    }
    else if (controller->keyB.press)
    {
        devmenuState = DevMenu_MainMenu;
        listPos = 0;
        scrollPos = 0;
        selection = 2;
    }
}

void DevMenu_SceneSelectMenu()
{
    const ScreenInfo* currentScreen = *currentScreen_ptr;
    uint32 selectionColors[] = {
        0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090,
    };
    selectionColors[selection - scrollPos] = 0xF0F0F0;

    // Info Box
    int32 y = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, y - 84, 256, 0x30, 0x80, 0xFF, INK_NONE, true);

    y -= 68;
    DrawDevString("SELECT STAGE SCENE", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);

    // Selections Box
    y += 44;
    DrawRectangle(currentScreen->center.x - 128, y - 8, 0x100, 0x48, 0x80, 0xFF, INK_NONE, true);

    int32 y2 = y;
    auto list = &sceneInfo.listCategory[listPos];
    int32 start = list->sceneOffsetStart;
    for (int32 i = 0; i < 8; ++i)
    {
        if (scrollPos + i < list->sceneCount)
        {
            DrawDevString(sceneInfo.listData[start + (scrollPos + i)].name, currentScreen->center.x + 96, y2, ALIGN_RIGHT, selectionColors[i]);
            y2 += 8;
        }
    }
    y += 20;


    if (controller->keyUp.press)
    {
        if (start + --selection < list->sceneOffsetStart)
            selection = list->sceneCount - 1;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7) {
                scrollPos = selection - 7;
            }
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyUp.down)
    {
        if (!timer && start + --selection < list->sceneOffsetStart)
            selection = list->sceneCount - 1;

        timer = (timer + 1) & 7;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else {
            scrollPos = selection;
        }
    }

    if (controller->keyDown.press)
    {
        if (++selection >= list->sceneCount)
            selection = 0;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyDown.down)
    {
        if (!timer && ++selection >= list->sceneCount)
            selection = 0;

        timer = (timer + 1) & 7;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }
    }

    if (controller->keyStart.press || controller->keyA.press)
    {
        sceneInfo.activeCategory = listPos;
        sceneInfo.listPos = selection + list->sceneOffsetStart;

        switch (engineVersion)
        {
        case 5:
            sceneInfo.state = ENGINESTATE_LOAD;
            break;
        case 4:
        case 3:
            break;
        default:
            break;
        }
    }
    else if (controller->keyB.press)
    {
        devmenuState = DevMenu_CategorySelectMenu;
        scrollPos = 0;
        selection = 0;
        listPos = 0;
    }
}

void DevMenu_OptionsMenu()
{
    const ScreenInfo* currentScreen = *currentScreen_ptr;
    const int32 selectionCount = 2;
    uint32 selectionColors[] = { 0x808090, 0x808090 };
    const char* selectionNames[] = { "Debug Flags", "Back" };
    selectionColors[selection] = 0xF0F0F0;

    // Info Box
    int32 y = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, y - 84, 256, 0x30, 0x80, 0xFF, INK_NONE, true);

    y -= 68;
    DrawDevString("OPTIONS", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);

    // Selections Box
    y += 44;
    DrawRectangle(currentScreen->center.x - 128, y - 8, 0x100, 0x48, 0x80, 0xFF, INK_NONE, true);

    for (int32 i = 0; i < selectionCount; ++i)
    {
        DrawDevString(selectionNames[i], currentScreen->center.x, y, ALIGN_CENTER, selectionColors[i]);
        y += 12;
    }
    y += 20;


    if (controller->keyUp.press)
    {
        selection--;
        timer = 1;

        if (selection < 0)
            selection += selectionCount;
    }
    else if (controller->keyUp.down)
    {
        if (timer)
            timer = ++timer & 7;
        else
        {
            selection--;
            timer = ++timer & 7;

            if (selection < 0)
                selection += selectionCount;
        }
    }

    if (controller->keyDown.press)
    {
        selection++;
        timer = 1;

        if (selection >= selectionCount)
            selection -= selectionCount;
    }
    else if (controller->keyDown.down)
    {
        if (timer)
            timer = ++timer & 7;
        else
        {
            selection++;
            timer = ++timer & 7;

            if (selection >= selectionCount)
                selection -= selectionCount;
        }
    }

    if (controller->keyStart.press || controller->keyA.press)
    {
        switch (selection)
        {
        case 0: // Debug Flags
            devmenuState = DevMenu_DebugOptionsMenu;
            selection = 0;
            break;
        case 1: // Back
            devmenuState = DevMenu_MainMenu;
            selection = 0;
            break;
        default:
            break;
        }
    }
    else if (controller->keyB.press)
    {
        devmenuState = DevMenu_MainMenu;
        scrollPos = 0;
        selection = 0;
        listPos = 0;
    }
}

void DevMenu_DebugOptionsMenu()
{
    const ScreenInfo* currentScreen = *currentScreen_ptr;
    uint32 selectionColors[] = { 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090 };
    selectionColors[selection - scrollPos] = 0xF0F0F0;

    int32 dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 0x100, 0x30, 0x80, 0xFF, INK_NONE, true);

    dy -= 68;
    DrawDevString("CONFIGURE DEBUG FLAGS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);

    dy += 40;
    DrawRectangle(currentScreen->center.x - 128, dy - 4, 0x100, 0x48, 0x80, 0xFF, INK_NONE, true);

    for (int32 i = 0; i < 8; ++i)
    {
        if (scrollPos + i < viewableVarCount)
        {
            ViewableVariable* value = &viewableVarList[scrollPos + i];
            DrawDevString(value->name, currentScreen->center.x - 96, dy, ALIGN_LEFT, selectionColors[i]);

            if (!value->value)
            {
                DrawDevString("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080);
            }
            else
            {
                char valueStr[0x10];
                strcpy_s(valueStr, "--------");

                switch (value->size)
                {
                    case sizeof(uint8):
                    {
                        uint8* v = (uint8*)value->value;

                        switch (value->type)
                        {
                        case VIEWVAR_DISPLAY_BOOL:
                            valueStr[0] = *v ? 'Y' : 'N';
                            valueStr[1] = 0;
                            break;
                        default:
                        case VIEWVAR_DISPLAY_UNSIGNED:
                            valueStr[0] = ' ';
                            break;
                        case VIEWVAR_DISPLAY_SIGNED:
                            valueStr[0] = *v > 0x7F ? '-' : ' ';
                            *v &= 0x7F;
                            break;
                        }
                        break;
                    }
                    case sizeof(int16):
                    {
                        uint16* v = (uint16*)value->value;

                        switch (value->type)
                        {
                        case VIEWVAR_DISPLAY_BOOL:
                            valueStr[0] = *v ? 'Y' : 'N';
                            valueStr[1] = 0;
                            break;
                        default:
                        case VIEWVAR_DISPLAY_UNSIGNED:
                            valueStr[0] = ' ';
                            break;
                        case VIEWVAR_DISPLAY_SIGNED:
                            valueStr[0] = *v > 0x7FFF ? '-' : ' ';
                            *v &= 0x7FFF;
                            break;
                        }
                        break;
                    }
                    case sizeof(int32):
                    {
                        uint32* v = (uint32*)value->value;

                        switch (value->type)
                        {
                        case VIEWVAR_DISPLAY_BOOL:
                            valueStr[0] = *v ? 'Y' : 'N';
                            valueStr[1] = 0;
                            break;
                        default:
                        case VIEWVAR_DISPLAY_UNSIGNED:
                            valueStr[0] = ' ';
                            break;
                        case VIEWVAR_DISPLAY_SIGNED:
                            valueStr[0] = *v > 0x7FFFFFFF ? '-' : ' ';
                            *v &= 0x7FFFFFFF;
                            break;
                        }
                        break;
                    }
                    default:
                        DrawDevString("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080);
                        break;
                }

                if (value->type != VIEWVAR_DISPLAY_BOOL)
                {
                    if (2 * value->size)
                    {
                        char* curChar = &valueStr[2 * value->size];
                        valueStr[(2 * value->size) + 1] = 0;

                        switch (value->size)
                        {
                            case sizeof(uint8):
                            {
                                uint8* valuePtr = (uint8*)value->value;
                                for (int32 v = 0; v < 2 * value->size; ++v)
                                    *curChar-- = ((v & 0xF) > 9 ? '7' : '0') + ((*valuePtr >> 4 * v) & 0xF);
                                break;
                            }
                            case sizeof(int16):
                            {
                                uint16* valuePtr = (uint16*)value->value;

                                for (int32 v = 0; v < 2 * value->size; ++v)
                                    *curChar-- = ((v & 0xF) > 9 ? '7' : '0') + ((*valuePtr >> 4 * v) & 0xF);
                                break;
                            }
                            case sizeof(int32):
                            {
                                uint32* valuePtr = (uint32*)value->value;

                                for (int32 v = 0; v < 2 * value->size; ++v)
                                    *curChar-- = ((v & 0xF) > 9 ? '7' : '0') + ((*valuePtr >> 4 * v) & 0xF);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }

                DrawDevString(valueStr, currentScreen->center.x + 96, dy, ALIGN_CENTER, 0xF0F080);
            }
            dy += 8;
        }
        else {
            DrawDevString("Back", currentScreen->center.x, dy, ALIGN_CENTER, selectionColors[i]);
        }
    }

    if (controller->keyUp.press)
    {
        if (--selection < 0)
            selection = viewableVarCount;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyUp.down)
    {
        if (!timer && --selection < 0)
            selection = viewableVarCount;

        timer = (timer + 1) & 7;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }
    }

    if (controller->keyDown.press)
    {
        if (++selection > viewableVarCount)
            selection = 0;

        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }

        timer = 1;
    }
    else if (controller->keyDown.down)
    {
        if (!timer && ++selection >= viewableVarCount)
            selection = 0;

        timer = (timer + 1) & 7;
        if (selection >= scrollPos)
        {
            if (selection > scrollPos + 7)
                scrollPos = selection - 7;
        }
        else
        {
            scrollPos = selection;
        }
    }

    if (selection < viewableVarCount)
    {
        ViewableVariable* var = &viewableVarList[selection];

        switch (var->size)
        {
        case sizeof(int8):
        {
            int8* value = (int8*)var->value;

            if (controller->keyLeft.press)
            {
                if (var->type == VIEWVAR_DISPLAY_BOOL)
                    *value ^= 1;
                else if (*value - 1 >= var->min)
                    *value = *value - 1;
            }

            if (controller->keyRight.press)
            {
                if (var->type == VIEWVAR_DISPLAY_BOOL)
                    *value ^= 1;
                else if (*value + 1 <= var->max)
                    *value = *value + 1;
            }
            break;
        }
        case sizeof(int16):
        {
            int16* value = (int16*)var->value;

            if (controller->keyLeft.press)
            {
                if (var->type == VIEWVAR_DISPLAY_BOOL)
                    *value ^= 1;
                else if (*value - 1 >= var->min)
                    *value = *value - 1;
            }

            if (controller->keyRight.press)
            {
                if (var->type == VIEWVAR_DISPLAY_BOOL)
                    *value ^= 1;
                else if (*value + 1 <= var->max)
                    *value = *value + 1;
            }
            break;
        }
        case sizeof(int32):
        {
            int32* value = (int32*)var->value;

            if (controller->keyLeft.press)
            {
                if (var->type == VIEWVAR_DISPLAY_BOOL)
                    *value ^= 1;
                else if (*value - 1 >= var->min)
                    *value = *value - 1;
            }

            if (controller->keyRight.press)
            {
                if (!var->type)
                    *value ^= 1;
                else if (*value + 1 <= var->max)
                    *value = *value + 1;
            }
            break;
        }
        default:
            DrawDevString("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080); break;
        }
    }
    else {
        if (controller->keyStart.press || controller->keyA.press)
        {
            devmenuState = DevMenu_OptionsMenu;
            selection = 0;
        }
    }

    if (controller->keyB.press)
    {
        devmenuState = DevMenu_OptionsMenu;
        selection = 0;
    }
}

void OpenDevMenu()
{
    switch (engineVersion)
    {
    default: break;
    case 5:
        sceneState = sceneInfo.state;
        sceneInfo.state = ENGINESTATE_DEVMENU;
        devmenuState = DevMenu_MainMenu;
        selection = 0;
        timer = 1;
        scrollPos = 0;
        ((void(__fastcall*)())(FunctionTable[FunctionTable_PauseChannel]))();
        break;
    case 4:
    case 3:
        break;
    }
}

void CloseDevMenu()
{
    switch (engineVersion)
    {
    default: break;
    case 5:
        sceneInfo.state = sceneState;
        ((void(__fastcall*)())(FunctionTable[FunctionTable_ResumeChannel]))();
        break;
    case 4:
    case 3:
        break;
    }
}

void ToggleDevMenu()
{
    if (sceneInfo.state == ENGINESTATE_DEVMENU)
        CloseDevMenu();
    else
        OpenDevMenu();
}

void ClearViewableVariables()
{
    viewableVarCount = 0;
}

void AddViewableVariable(const char* name, void* value, int32 type, int32 min, int32 max)
{
    if (viewableVarCount < 0x100)
    {
        ViewableVariable* viewVar = &viewableVarList[viewableVarCount++];

        strncpy_s(viewVar->name, name, 0x10);
        viewVar->value = value;

        switch (type)
        {
        case VIEWVAR_BOOL:
            viewVar->type = VIEWVAR_DISPLAY_BOOL;
            viewVar->size = sizeof(bool32);
            break;
        case VIEWVAR_UINT8:
            viewVar->type = VIEWVAR_DISPLAY_UNSIGNED;
            viewVar->size = sizeof(uint8);
            break;
        case VIEWVAR_UINT16:
            viewVar->type = VIEWVAR_DISPLAY_UNSIGNED;
            viewVar->size = sizeof(uint16);
            break;
        case VIEWVAR_UINT32:
            viewVar->type = VIEWVAR_DISPLAY_UNSIGNED;
            viewVar->size = sizeof(uint32);
            break;
        case VIEWVAR_INT8:
            viewVar->type = VIEWVAR_DISPLAY_SIGNED;
            viewVar->size = sizeof(int8);
            break;
        case VIEWVAR_INT16:
            viewVar->type = VIEWVAR_DISPLAY_SIGNED;
            viewVar->size = sizeof(int16);
            break;
        case VIEWVAR_INT32:
            viewVar->type = VIEWVAR_DISPLAY_SIGNED;
            viewVar->size = sizeof(int32);
            break;
        default:
            break;
        }

        viewVar->min = min;
        viewVar->max = max;
    }
}