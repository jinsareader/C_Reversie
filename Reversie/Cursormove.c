#include "Cursormove.h"

void MoveCursor(int x, int y)
{
	COORD coord = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int SetmiddleofX(int n)
{
	return (MAX_X - n) / 2;
}