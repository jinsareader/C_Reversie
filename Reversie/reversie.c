#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include "Button.h"
#include "Cursormove.h"

#define N 10

int menuprocess(int* set);
void mainprocess(int set);

void setwall(int* board);
void setboard(int* board);

void boardUpdate(int* board, int* prev_board, Button* button);
void printscore(int* board);
void printturn(int turn);
int canClickBlack(int* board);
int canClickWhite(int* board);

int clickactionBlack(int* board, Button* button);
int clickactionWhite(int* board, Button* button);

void heuristicScoreInitial(int* scoreboard, int* board);
void heuristicScoreBlack(int* scoreboard, int* board);
void heuristicScoreWhite(int* scoreboard, int* board);
int getHeuristic(int* scoreboard, int* board);
void comBlack(int* scoreboard, int* board);
void comWhite(int* scoreboard, int* board);
void predictBlack(int* scoreboard, int* board, int n);
void predictWhite(int* scoreboard, int* board, int n);

void changeActionBlack(int* board, int n);
void changeActionWhite(int* board, int n);

void eraseMark(int* board);

int getColor(WORD* ret);


int main()
{
	int set = 0;

	while (1)
	{
		if (menuprocess(&set))
			return 0;
		mainprocess(set);
		printf("\nPress Any Key to Continue");
		_getch();
		clearConsoleWindow();
	}
}

int menuprocess(int* set)
{
	char* str1 = "### Reversie ###";
	char input[16] = { 0 };

	MoveCursor(SetmiddleofX(strlen(str1)), 1);
	printf("%s", str1);
	printf("\n");
	printf("1. Play 1st - ¡Ü\n");
	printf("2. Play 2nd - ¡Û\n");
	printf("3. Player vs Player\n");
	printf("4. Computer vs Computer\n");
	printf("5. Computer vs Computer - Fast Mode\n");
	printf("0. Exit\n");
	printf("\n");
	while (1)
	{
		printf("Command : ");
		fgets(input, 16, stdin);
		switch (input[0])
		{
		case '1':
			*set = 1;
			break;
		case '2':
			*set = 2;
			break;
		case '3':
			*set = 3;
			break;
		case '4':
			*set = 4;
			break;
		case '5':
			*set = 5;
			break;
		case '0':
			return 1;
		default:
			printf("Wrong command, press again.\n");
			continue;
		}
		clearConsoleWindow();
		return 0;
	}
}
void mainprocess(int set)
{
	int board[100] = { 0 };
	int prev_board[100] = { 0 };
	int scoreboard[100] = { 0 };
	Button button[100];
	int turn = 1;
	int temp = 0;
	int pass = 0;

	setwall(board);
	setboard(board);
	for (int i = 0; i < N * N; i++)
		*(prev_board + i) = *(board + i);
	while (1)
	{
		boardUpdate(board, prev_board, button);
		printscore(board);
		printturn(turn);
		//black turn
		if (turn % 2)
		{
			while (1)
			{
				if (canClickBlack(board) == 0)
				{
					if (pass)
					{
						return;
					}
					pass = 1;
					break;
				}
				boardUpdate(board, prev_board, button);
				for (int i = 0; i < N * N; i++)
					*(prev_board + i) = *(board + i);
				pass = 0;
				if (set == 1 || set == 3)
				{
					while (temp == 0)
						temp = clickactionBlack(board, button);
					temp = 0;
				}
				else
				{
					int scoreboard[100] = { 0 };
					if (set != 5)
						Sleep(1000);
					heuristicScoreBlack(scoreboard, board);
					comBlack(scoreboard, board);
				}
				break;
			}
		}
		//white turn
		else
		{
			while (1)
			{
				if (canClickWhite(board) == 0)
				{

					if (pass)
					{
						return;
					}
					pass = 1;
					break;
				}
				boardUpdate(board, prev_board, button);
				for (int i = 0; i < N * N; i++)
					*(prev_board + i) = *(board + i);
				pass = 0;
				if (set == 2 || set == 3)
				{
					while (temp == 0)
						temp = clickactionWhite(board, button);
					temp = 0;
				}
				else
				{
					int scoreboard[100] = { 0 };
					if (set != 5)
						Sleep(1000);
					heuristicScoreWhite(scoreboard, board);
					comWhite(scoreboard, board);
				}
				break;
			}
		}
		turn++;
		eraseMark(board);
	}
}

void setwall(int* board)
{
	for (int i = 0;i < N;i++)
	{
		*(board + i) = 100;
		*(board + (N-1)*N + i) = 100;
		*(board + i*N) = 100;
		*(board + i*N + N-1) = 100;
	}
}
void setboard(int* board)
{
	*(board + 44) = 2;
	*(board + 45) = 1;
	*(board + 54) = 1;
	*(board + 55) = 2;
}

void boardUpdate(int* board, int* prev_board, Button* button)
{
	for (int y = 1; y<=8; y++)
		for (int x = 1; x <= 8; x++)
		{
			int n = y * N + x;
			WORD color;

			if (y % 2)
			{
				if (x % 2)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_RED);
			}
			else
			{
				if (x % 2)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_RED);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			}
			if (*(prev_board + n) == 10 && (*(board + n) == 1 || *(board + n) == 2))
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
			getColor(&color);
			

			MoveCursor(SetmiddleofX((N - 2) * 4) + 4 * x, 3 * y);
			SetStartofButton(button, n);
			printf("    ");
			MoveCursor(SetmiddleofX((N - 2) * 4) + 4 * x, 3 * y + 1);
			switch (*(board + n))
			{
			case 0:
				printf("    ");
				break;
			case 1:
				if (*(prev_board + n) == 2)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | FOREGROUND_INTENSITY);
				printf(" ¡Ü ");
				break;
			case 2:
				if (*(prev_board + n) == 1)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | FOREGROUND_INTENSITY);
				printf(" ¡Û ");
				break;
			case 10 :
				printf(" ££ ");
				break;
			default:
				printf("ERRO");
				break;
			}
			MoveCursor(SetmiddleofX((N - 2) * 4) + 4 * x, 3 * y + 2);
			printf("    ");
			SetEndofButton(button, n);
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
void printscore(int* board)
{
	int black = 0;
	int white = 0;
	for (int i = 0; i < N * N; i++)
	{
		if (*(board + i) == 1)
			black++;
		if (*(board + i) == 2)
			white++;
	}
	MoveCursor(0, 28);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	printf(" ¡Ü : %2d \n", black);
	printf(" ¡Û : %2d \n", white);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
void printturn(int turn)
{
	MoveCursor(0, 30);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	if (turn % 2)
	{
		printf(" Turn : ¡Ü (%d) ", turn);
	}
	else
	{
		printf(" Turn : ¡Û (%d) ", turn);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
int canClickBlack(int* board)
{
	int result = 0;
	int i = 1;

	for (int y = 1; y <= 8; y++)
		for (int x = 1; x <= 8; x++)
		{
			int n = N * y + x;

			if (*(board + n) == 0)
			{
				if (*(board + n - (N + 1)) == 2)
				{
					for (i = 1; *(board + n - i * (N + 1)) == 2; i++);
					if (*(board + n - i * (N + 1)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n - (N)) == 2)
				{
					for (i = 1; *(board + n - i * (N)) == 2; i++);
					if (*(board + n - i * (N)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n - (N - 1)) == 2)
				{
					for (i = 1; *(board + n - i * (N - 1)) == 2; i++);
					if (*(board + n - i * (N - 1)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}

				if (*(board + n - (1)) == 2)
				{
					for (i = 1; *(board + n - i) == 2; i++);
					if (*(board + n - i) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (1)) == 2)
				{
					for (i = 1; *(board + n + i) == 2; i++);
					if (*(board + n + i) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}

				if (*(board + n + (N + 1)) == 2)
				{
					for (i = 1; *(board + n + i * (N + 1)) == 2; i++);
					if (*(board + n + i * (N + 1)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (N)) == 2)
				{
					for (i = 1; *(board + n + i * (N)) == 2; i++);
					if (*(board + n + i * (N)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (N - 1)) == 2)
				{
					for (i = 1; *(board + n + i * (N - 1)) == 2; i++);
					if (*(board + n + i * (N - 1)) == 1)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
			}
		}

	return result;
}
int canClickWhite(int* board)
{
	int result = 0;
	int i = 1;

	for (int y = 1; y <= 8; y++)
		for (int x = 1; x <= 8; x++)
		{
			int n = N * y + x;

			if (*(board + n) == 0)
			{
				if (*(board + n - (N + 1)) == 1)
				{
					for (i = 1; *(board + n - i * (N + 1)) == 1; i++);
					if (*(board + n - i * (N + 1)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n - (N)) == 1)
				{
					for (i = 1; *(board + n - i * (N)) == 1; i++);
					if (*(board + n - i * (N)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n - (N - 1)) == 1)
				{
					for (i = 1; *(board + n - i * (N - 1)) == 1; i++);
					if (*(board + n - i * (N - 1)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}

				if (*(board + n - (1)) == 1)
				{
					for (i = 1; *(board + n - i) == 1; i++);
					if (*(board + n - i) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (1)) == 1)
				{
					for (i = 1; *(board + n + i) == 1; i++);
					if (*(board + n + i) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}

				if (*(board + n + (N + 1)) == 1)
				{
					for (i = 1; *(board + n + i * (N + 1)) == 1; i++);
					if (*(board + n + i * (N + 1)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (N)) == 1)
				{
					for (i = 1; *(board + n + i * (N)) == 1; i++);
					if (*(board + n + i * (N)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
				if (*(board + n + (N - 1)) == 1)
				{
					for (i = 1; *(board + n + i * (N - 1)) == 1; i++);
					if (*(board + n + i * (N - 1)) == 2)
					{
						*(board + n) = 10;
						result += 1;
					}
				}
			}
		}

	return result;
}

int clickactionBlack(int* board, Button* button)
{
	int x = 0;
	int y = 0;
	int clck = 0;
	int p = 0;

	clickbutton_w(&x, &y, &clck);
	if (clck == 1)
		if (findButton(x, y, &p, button, N * N))
			if (*(board + p) == 10)
			{
				changeActionBlack(board, p);

				return 1;
			}
	return 0;
}
int clickactionWhite(int* board, Button* button)
{
	int x = 0;
	int y = 0;
	int clck = 0;
	int p = 0;
	int i = 1;

	clickbutton_w(&x, &y, &clck);
	if (clck == 1)
		if (findButton(x, y, &p, button, N * N))
			if (*(board+ p) == 10)
			{
				changeActionWhite(board, p);
				
				return 1;
			}
	return 0;
}

void heuristicScoreInitial(int* scoreboard, int* board)
{
	int i = 1;

	for (i = 1;i <= 8;i++)
	{
		if (*(board + 11) % 10 == 0 && *(board + 18) % 10 == 0)
			*(scoreboard + 10 + i) += 200;
		if (*(board + 81) % 10 == 0 && *(board + 88) % 10 == 0)
			*(scoreboard + 80 + i) += 200;
		if (*(board + 11) % 10 == 0 && *(board + 81) % 10 == 0)
			*(scoreboard + i * 10 + 1) += 200;
		if (*(board + 18) % 10 == 0 && *(board + 88) % 10 == 0)
			*(scoreboard + i * 10 + 8) += 200;
	}
	for (i = 2;i <= 7;i++)
	{
		*(scoreboard + 30 + i) = 50;
		*(scoreboard + 60 + i) = 50;
		*(scoreboard + i * 10 + 3) = 50;
		*(scoreboard + i * 10 + 6) = 50;
	}

	if (1)
	{
		*(scoreboard + 12) = -200;
		*(scoreboard + 21) = -200;
		*(scoreboard + 22) = -200;
	}
	if (1)
	{
		*(scoreboard + 17) = -200;
		*(scoreboard + 27) = -200;
		*(scoreboard + 28) = -200;
	}
	if (1)
	{
		*(scoreboard + 71) = -200;
		*(scoreboard + 72) = -200;
		*(scoreboard + 82) = -200;
	}
	if (1)
	{
		*(scoreboard + 77) = -200;
		*(scoreboard + 78) = -200;
		*(scoreboard + 87) = -200;
	}
}
void heuristicScoreBlack(int* scoreboard, int* board)
{
	int i = 1;

	heuristicScoreInitial(scoreboard, board);

	for (int y = 1; y <= 8; y++)
		for (int x = 1; x <= 8; x++)
		{
			int n = N * y + x;

			predictBlack(scoreboard, board, n);

			if (*(board + n - (N + 1)) == 2)
			{
				for (i = 1; *(board + n - i * (N + 1)) == 2; i++);
				if (*(board + n - i * (N + 1)) == 1)
				{
					for (i = 1; *(board + n - i * (N + 1)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n - (N)) == 2)
			{
				for (i = 1; *(board + n - i * (N)) == 2; i++);
				if (*(board + n - i * (N)) == 1)
				{
					for (i = 1; *(board + n - i * (N)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n - (N - 1)) == 2)
			{
				for (i = 1; *(board + n - i * (N - 1)) == 2; i++);
				if (*(board + n - i * (N - 1)) == 1)
				{
					for (i = 1; *(board + n - i * (N - 1)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}

			if (*(board + n - (1)) == 2)
			{
				for (i = 1; *(board + n - i) == 2; i++);
				if (*(board + n - i) == 1)
				{
					for (i = 1; *(board + n - i) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (1)) == 2)
			{
				for (i = 1; *(board + n + i) == 2; i++);
				if (*(board + n + i) == 1)
				{
					for (i = 1; *(board + n + i) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}

			if (*(board + n + (N + 1)) == 2)
			{
				for (i = 1; *(board + n + i * (N + 1)) == 2; i++);
				if (*(board + n + i * (N + 1)) == 1)
				{
					for (i = 1; *(board + n + i * (N + 1)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (N)) == 2)
			{
				for (i = 1; *(board + n + i * (N)) == 2; i++);
				if (*(board + n + i * (N)) == 1)
				{
					for (i = 1; *(board + n + i * (N)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (N - 1)) == 2)
			{
				for (i = 1; *(board + n + i * (N - 1)) == 2; i++);
				if (*(board + n + i * (N - 1)) == 1)
				{
					for (i = 1; *(board + n + i * (N - 1)) == 2; i++)
						*(scoreboard + n) += 1;
				}
			}
		}
}
void heuristicScoreWhite(int* scoreboard, int* board)
{
	int i = 1;

	heuristicScoreInitial(scoreboard, board);

	for (int y = 1; y <= 8; y++)
		for (int x = 1; x <= 8; x++)
		{
			int n = N * y + x;

			predictWhite(scoreboard, board, n);

			if (*(board + n - (N + 1)) == 1)
			{
				for (i = 1; *(board + n - i * (N + 1)) == 1; i++);
				if (*(board + n - i * (N + 1)) == 2)
				{
					for (i = 1; *(board + n - i * (N + 1)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n - (N)) == 1)
			{
				for (i = 1; *(board + n - i * (N)) == 1; i++);
				if (*(board + n - i * (N)) == 2)
				{
					for (i = 1; *(board + n - i * (N)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n - (N - 1)) == 1)
			{
				for (i = 1; *(board + n - i * (N - 1)) == 1; i++);
				if (*(board + n - i * (N - 1)) == 2)
				{
					for (i = 1; *(board + n - i * (N - 1)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}

			if (*(board + n - (1)) == 1)
			{
				for (i = 1; *(board + n - i) == 1; i++);
				if (*(board + n - i) == 2)
				{
					for (i = 1; *(board + n - i) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (1)) == 1)
			{
				for (i = 1; *(board + n + i) == 1; i++);
				if (*(board + n + i) == 2)
				{
					for (i = 1; *(board + n + i) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}

			if (*(board + n + (N + 1)) == 1)
			{
				for (i = 1; *(board + n + i * (N + 1)) == 1; i++);
				if (*(board + n + i * (N + 1)) == 2)
				{
					for (i = 1; *(board + n + i * (N + 1)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (N)) == 1)
			{
				for (i = 1; *(board + n + i * (N)) == 1; i++);
				if (*(board + n + i * (N)) == 2)
				{
					for (i = 1; *(board + n + i * (N)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
			if (*(board + n + (N - 1)) == 1)
			{
				for (i = 1; *(board + n + i * (N - 1)) == 1; i++);
				if (*(board + n + i * (N - 1)) == 2)
				{
					for (i = 1; *(board + n + i * (N - 1)) == 1; i++)
						*(scoreboard + n) += 1;
				}
			}
		}
}
int getHeuristic(int* scoreboard, int* board)
{
	srand(time(NULL));
	int score = -1000;
	int result[100] = { 0 };
	int size = 0;

	for (int i = 0; i < N * N; i++)
	{
		if (*(board + i) == 10)
			if (score < *(scoreboard + i))
			{
				size = 0;

				score = *(scoreboard + i);
				*(result + size) = i;
			}
			else if (score == *(scoreboard + i))
			{
				size++;

				*(result + size) = i;
			}
	}
	
	int n = rand() % (size + 1);
	return *(result + n);
}
void comBlack(int* scoreboard, int* board)
{
	int n = getHeuristic(scoreboard, board);
	int i = 1;

	changeActionBlack(board, n);
}
void comWhite(int* scoreboard, int* board)
{
	int n = getHeuristic(scoreboard, board);
	int i = 1;

	changeActionWhite(board, n);
}
void predictBlack(int* scoreboard, int* board, int n)
{
	int predict_board[100] = { 0 };
	int arr[12] = { 12,21,22,17,27,28,71,72,82,77,78,87 };

	for (int i = 0; i < 100; i++)
		*(predict_board + i) = *(board + i);
	if (*(predict_board + n) == 10)
	{
		changeActionBlack(predict_board, n);
		for (int i = 0;i < 12;i++)
			if (*(predict_board + *(arr + i)) == 1)
			{
				*(scoreboard + n) -= 100;
				break;
			}

		eraseMark(predict_board);
		if (canClickWhite(predict_board) == 0)
			*(scoreboard + n) += 500;
	}
}
void predictWhite(int* scoreboard, int* board, int n)
{
	int predict_board[100] = { 0 };
	int arr[12] = { 12,21,22,17,27,28,71,72,82,77,78,87 };

	for (int i = 0; i < 100; i++)
		*(predict_board + i) = *(board + i);
	if (*(predict_board + n) == 10)
	{
		changeActionWhite(predict_board, n);
		for (int i = 0;i < 12;i++)
			if (*(predict_board + *(arr + i)) == 2)
			{
				*(scoreboard + n) -= 100;
				break;
			}

		eraseMark(predict_board);
		if (canClickBlack(predict_board) == 0)
			*(scoreboard + n) += 500;
	}
}

void changeActionBlack(int* board, int n)
{
	int i = 1;

	*(board + n) = 1;
	if (*(board + n - (N + 1)) == 2)
	{
		for (i = 1; *(board + n - i * (N + 1)) == 2; i++);
		if (*(board + n - i * (N + 1)) == 1)
		{
			for (i = 1; *(board + n - i * (N + 1)) == 2; i++)
				*(board + n - i * (N + 1)) = 1;
		}
	}
	if (*(board + n - (N)) == 2)
	{
		for (i = 1; *(board + n - i * (N)) == 2; i++);
		if (*(board + n - i * (N)) == 1)
		{
			for (i = 1; *(board + n - i * (N)) == 2; i++)
				*(board + n - i * (N)) = 1;
		}
	}
	if (*(board + n - (N - 1)) == 2)
	{
		for (i = 1; *(board + n - i * (N - 1)) == 2; i++);
		if (*(board + n - i * (N - 1)) == 1)
		{
			for (i = 1; *(board + n - i * (N - 1)) == 2; i++)
				*(board + n - i * (N - 1)) = 1;
		}
	}

	if (*(board + n - (1)) == 2)
	{
		for (i = 1; *(board + n - i) == 2; i++);
		if (*(board + n - i) == 1)
		{
			for (i = 1; *(board + n - i) == 2; i++)
				*(board + n - i) = 1;
		}
	}
	if (*(board + n + (1)) == 2)
	{
		for (i = 1; *(board + n + i) == 2; i++);
		if (*(board + n + i) == 1)
		{
			for (i = 1; *(board + n + i) == 2; i++)
				*(board + n + i) = 1;
		}
	}

	if (*(board + n + (N + 1)) == 2)
	{
		for (i = 1; *(board + n + i * (N + 1)) == 2; i++);
		if (*(board + n + i * (N + 1)) == 1)
		{
			for (i = 1; *(board + n + i * (N + 1)) == 2; i++)
				*(board + n + i * (N + 1)) = 1;
		}
	}
	if (*(board + n + (N)) == 2)
	{
		for (i = 1; *(board + n + i * (N)) == 2; i++);
		if (*(board + n + i * (N)) == 1)
		{
			for (i = 1; *(board + n + i * (N)) == 2; i++)
				*(board + n + i * (N)) = 1;
		}
	}
	if (*(board + n + (N - 1)) == 2)
	{
		for (i = 1; *(board + n + i * (N - 1)) == 2; i++);
		if (*(board + n + i * (N - 1)) == 1)
		{
			for (i = 1; *(board + n + i * (N - 1)) == 2; i++)
				*(board + n + i * (N - 1)) = 1;
		}
	}
}
void changeActionWhite(int* board, int n)
{
	int i = 1;

	*(board + n) = 2;
	if (*(board + n - (N + 1)) == 1)
	{
		for (i = 1; *(board + n - i * (N + 1)) == 1; i++);
		if (*(board + n - i * (N + 1)) == 2)
		{
			for (i = 1; *(board + n - i * (N + 1)) == 1; i++)
				*(board + n - i * (N + 1)) = 2;
		}
	}
	if (*(board + n - (N)) == 1)
	{
		for (i = 1; *(board + n - i * (N)) == 1; i++);
		if (*(board + n - i * (N)) == 2)
		{
			for (i = 1; *(board + n - i * (N)) == 1; i++)
				*(board + n - i * (N)) = 2;
		}
	}
	if (*(board + n - (N - 1)) == 1)
	{
		for (i = 1; *(board + n - i * (N - 1)) == 1; i++);
		if (*(board + n - i * (N - 1)) == 2)
		{
			for (i = 1; *(board + n - i * (N - 1)) == 1; i++)
				*(board + n - i * (N - 1)) = 2;
		}
	}

	if (*(board + n - (1)) == 1)
	{
		for (i = 1; *(board + n - i) == 1; i++);
		if (*(board + n - i) == 2)
		{
			for (i = 1; *(board + n - i) == 1; i++)
				*(board + n - i) = 2;
		}
	}
	if (*(board + n + (1)) == 1)
	{
		for (i = 1; *(board + n + i) == 1; i++);
		if (*(board + n + i) == 2)
		{
			for (i = 1; *(board + n + i) == 1; i++)
				*(board + n + i) = 2;
		}
	}

	if (*(board + n + (N + 1)) == 1)
	{
		for (i = 1; *(board + n + i * (N + 1)) == 1; i++);
		if (*(board + n + i * (N + 1)) == 2)
		{
			for (i = 1; *(board + n + i * (N + 1)) == 1; i++)
				*(board + n + i * (N + 1)) = 2;
		}
	}
	if (*(board + n + (N)) == 1)
	{
		for (i = 1; *(board + n + i * (N)) == 1; i++);
		if (*(board + n + i * (N)) == 2)
		{
			for (i = 1; *(board + n + i * (N)) == 1; i++)
				*(board + n + i * (N)) = 2;
		}
	}
	if (*(board + n + (N - 1)) == 1)
	{
		for (i = 1; *(board + n + i * (N - 1)) == 1; i++);
		if (*(board + n + i * (N - 1)) == 2)
		{
			for (i = 1; *(board + n + i * (N - 1)) == 1; i++)
				*(board + n + i * (N - 1)) = 2;
		}
	}
}

void eraseMark(int* board)
{
	for (int i = 0;i < N * N;i++)
		if (*(board + i) == 10)
			*(board + i) = 0;
}

int getColor(WORD* ret) 
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return 0;
	*ret = info.wAttributes;
	return 1;
}

