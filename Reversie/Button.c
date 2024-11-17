#include "Button.h"

void SetStartofButton(Button* button, int point)
{
	CONSOLE_SCREEN_BUFFER_INFO Cur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cur);
	(button + point)->x1 = Cur.dwCursorPosition.X;
	(button + point)->y1 = Cur.dwCursorPosition.Y;
}
void SetEndofButton(Button* button, int point)
{
	CONSOLE_SCREEN_BUFFER_INFO Cur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cur);
	(button + point)->x2 = Cur.dwCursorPosition.X - 1;
	(button + point)->y2 = Cur.dwCursorPosition.Y;
}
int findButton(int x, int y, int* p, Button* button, int numofbutton)
{
	for (int i = 0; i < numofbutton; i++)
		if (x >= (button + i)->x1 && x <= (button + i)->x2)
			if (y >= (button + i)->y1 && y <= (button + i)->y2)
			{
				*p = i;
				return 1;
			}
	return 0;
}
void clickbutton_w(int* x, int* y, int* clck)
{
	int mouse_x = 0;
	int mouse_y = 0;
	int CLICK = 0;

	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &RECORD, 1, &dwNOER); // 콘솔창 입력을 받아들임.
		if (RECORD.EventType == MOUSE_EVENT) // 마우스 이벤트일 경우
		{
			if (RECORD.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && CLICK == 0)
			{
				mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
				mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
				CLICK = 1;
			}
			if (RECORD.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED && CLICK == 0)
			{
				mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
				mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
				CLICK = 2;
			}
			if (CLICK == 1)
			{
				if (RECORD.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
					mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
				}
				else if (RECORD.Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))
				{
					mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
					mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
					CLICK = 3;
				}
				else
				{
					*x = mouse_x;
					*y = mouse_y;
					*clck = 1;
					CLICK = 0;
					return;
				}
			}
			else if (CLICK == 2)
			{
				if (RECORD.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				{
					mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
					mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
				}
				else if (RECORD.Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))
				{
					mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
					mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
					CLICK = 3;
				}
				else
				{
					*x = mouse_x;
					*y = mouse_y;
					*clck = 2;
					CLICK = 0;
					return;
				}
			}
			else if (CLICK == 3)
			{
				if (RECORD.Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))
				{
					mouse_x = RECORD.Event.MouseEvent.dwMousePosition.X;
					mouse_y = RECORD.Event.MouseEvent.dwMousePosition.Y;
				}
				else
				{
					*x = mouse_x;
					*y = mouse_y;
					*clck = 3;
					CLICK = 0;
					return;
				}
			}
		}
	}
}
void clearConsoleWindow()
{
	system("cls");
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwNOER) | ENABLE_MOUSE_INPUT | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
}