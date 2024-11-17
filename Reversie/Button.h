#ifndef _BUTTON
#define _BUTTON

#include<windows.h>

INPUT_RECORD RECORD;
DWORD        dwNOER;
typedef struct _Button {
	int x1;  //x pos of starting point
	int y1;  //y pos of starting point
	int x2;  //x pos of ending point
	int y2;  //y pos of ending point
	int type;//button type, 0 INVALID, 1 unpressed, 2 pressed
}Button;

//Get Starting point of Button
void SetStartofButton(Button* button, int point);
//Get Ending point of Button
void SetEndofButton(Button* button, int point);
//If button exist, return 1. Else, return 0. p is point of button
int findButton(int x, int y, int* p, Button* button, int numofbutton);
//Wait Until Click is Done. If left click done, clck is 1. If right lick done, clck is 2. If both click done, clck is 3.
void clickbutton_w(int* x, int* y, int* clck);
//Console WIndow Clear Function for Mouse Click
void clearConsoleWindow();

#endif
