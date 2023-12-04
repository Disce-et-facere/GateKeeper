#ifndef GATEKEEPER_H
#define GATEKEEPER_H

#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include <ctype.h>
#include "randomPass.h"
#include "tagHandler.h"
#include "sendPassToArduino.h"

#define TIMER_ENABLE_BUTTONS 1
#define BUTTON_ENABLE_DELAY 100
#define IDC_EDIT_NAME_AT    1006
#define IDC_COMBO_ROLE      1007
#define IDC_RADIO_TRUE      1008
#define IDC_RADIO_FALSE     1009
#define AT_BUTTON_ADD       1010
#define AT_BUTTON_CANCEL    1011
#define CN_EDIT_NAME        1012
#define CN_BUTTON_SAVE      1013
#define CN_BUTTON_CANCEL    1014
#define CID_COMBO_ROLE      1015
#define CID_BUTTON_SAVE     1016
#define CID_BUTTON_CANCEL   1017
#define CA_RADIO_TRUE       1018
#define CA_RADIO_FALSE      1019
#define CA_BUTTON_SAVE      1020
#define CA_BUTTON_CANCEL    1021
#define RT_BUTTON_REMOVE    1022
#define RT_BUTTON_CANCEL    1023

int createMainContent();
void addPopup(int i);
int getHwndPos(int i);
void userALI();
void fileALI();
int checkInput(const char *text); // check if userinput contains non letter characters
int onPopupAtaTime(int popup);
void changedTag(int index, int subIndex, char change[255]);
int removeTag(int index);
void noScanner();

#endif