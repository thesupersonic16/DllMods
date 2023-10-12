#pragma once

extern char scriptErrorMessage[0x400];

extern void DevMenu_MainMenu();
extern void OpenDevMenu();
extern void CloseDevMenu();
extern void ToggleDevMenu();
extern void ClearViewableVariables();
extern void AddViewableVariable(const char* name, void* value, int32 type, int32 min, int32 max);
extern void DisplayScriptError();