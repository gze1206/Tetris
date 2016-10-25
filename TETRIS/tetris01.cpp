#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>

#define IS_EMPTY 0
#define NOT_EMPTY 1

//�ش� ĭ�� ���¸� ���������� ����
enum State
{
	null = 0,
	is_moving,
	stuck,
	wall
};

//������ ��� ���� [���� ����] [���� ����] [���� ��ǥ]
char form[7][4][8] =
{
	{ { 1,0,1,1,1,2,1,3 },{ 0,1,1,1,2,1,3,1 },{ 1,0,1,1,1,2,1,3 },{ 0,1,1,1,2,1,3,1 } },
	{ { 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 } },
	{ { 2,1,0,2,1,2,1,1 },{ 1,0,1,1,2,1,2,2 },{ 2,1,0,2,1,2,1,1 },{ 1,0,1,1,2,1,2,2 } },
	{ { 0,1,1,1,1,2,2,2 },{ 2,0,2,1,1,1,1,2 },{ 0,1,1,1,1,2,2,2 },{ 2,0,2,1,1,1,1,2 } },
	{ { 1,0,1,1,1,2,2,2 },{ 0,1,1,1,2,1,0,2 },{ 1,0,2,0,2,1,2,2 },{ 2,1,2,2,1,2,0,2 } },
	{ { 2,0,2,1,2,2,1,2 },{ 0,1,0,2,1,2,2,2 },{ 1,0,2,0,1,1,1,2 },{ 0,1,1,1,2,1,2,2 } },
	{ { 0,1,1,1,2,1,1,2 },{ 2,0,2,1,2,2,1,1 },{ 1,1,1,2,0,2,2,2 },{ 1,0,1,1,1,2,2,1 } },
};

//0:���� 1:��Ʈ�� �� 2:���� 3:�ܺ�
char SpriteArray[4][3] = { "��", "��", "��", "��" };

//��Ʈ���� ������ �迭 0:���� 1:��Ʈ�� �� 2:���� 3:�ܺ�
int tetris[21][12];

//���� ����
int Score = 0;

//����� ������ ��Ÿ���� ����
int form_kind;
//����� ������ ��Ÿ���� ����
int form_kind_next;

//����� ȸ�� ���¸� ��Ÿ���� ����
int rotate_kind;

//ȭ�� �󿡼��� ��ǥ
int screen_x = 0, screen_y = 0;

//����ִ��� üũ
bool IS_ALIVE = true;

//Ŀ�� �����
void CursorOff()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

//Ŀ�� ���̱�
void CursorOn()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = TRUE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

//��ǻ� gotoXY(x,y)
void CurrentXY(int x, int y)
{
	COORD loc;
	loc.X = x;
	loc.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
}

//�ʱ�ȭ
void InitBoard()
{
	system("mode con:cols=83 lines=25");

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i == 20 || j == 0 || j == 11)
				tetris[i][j] = 3;
			else tetris[i][j] = 0;
		}
	}
	for (int i = 0; i < 21; i++)
	{
		CurrentXY(30, i);
		printf("%s", SpriteArray[State::wall]);
		CurrentXY(52, i);
		printf("%s", SpriteArray[State::wall]);
	}
	for (int i = 0; i < 12; i++)
	{
		CurrentXY(30+i*2, 20);
		printf("%s", SpriteArray[State::wall]);
	}
	CurrentXY(15, 9);
	printf("��SCORE��");
}

#pragma region ���/����
//���� �׸���
void Print_form()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2, screen_y + form[form_kind][rotate_kind][i * 2 + 1]);
		printf("%s", SpriteArray[State::is_moving]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]][screen_x + form[form_kind][rotate_kind][i * 2]] = State::is_moving;
	}
}

//���� ���� �׸���
void Print_next()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(56 + (form[form_kind_next][0][i * 2]) * 2, 5 + (form[form_kind_next][0][i * 2 + 1]));
		printf("%s", SpriteArray[State::is_moving]);
	}
}

//���� �����
void Delete_form()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2, screen_y + form[form_kind][rotate_kind][i * 2 + 1]);
		printf("%s", SpriteArray[State::null]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]][screen_x + form[form_kind][rotate_kind][i * 2]] = State::null;
	}
}

//���� ���� �����
void Delete_next()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(56 + (form[form_kind_next][0][i * 2]) * 2, 5 + (form[form_kind_next][0][i * 2 + 1]));
		printf("%s", SpriteArray[State::null]);
	}
}

void PrintAll()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 1; j < 11; j++)
		{
			CurrentXY(30 + j * 2, i);
			if (tetris[i][j] == State::stuck)
			{
				printf("%s", SpriteArray[State::stuck]);
			}
			else
			{
				printf("%s", SpriteArray[State::null]);
			}
		}
	}
}

void PrintScore()
{
	CurrentXY(15, 10);
	printf("%d", Score);
}
#pragma endregion

//(��) Ű �Է�
int GetKey()
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		Delete_form();
		Delete_form();
		rotate_kind++;
		rotate_kind %= 4;
		Print_form();
		Print_next();
		return 1;
	}
	else if (GetKeyState(VK_RETURN) & 0x8000)
	{
		screen_x = 4;
		screen_y = 0;
		form_kind = form_kind_next;
		form_kind_next = rand() % 7;
		rotate_kind = 0;
		return 2;
	}
}

//�ش� ��ǥ�� �� ĭ���� üũ
int Check_board(int x, int y)
{
	int temp = 0;

	for (int i = 0; i < 4; i++)
	{
		temp = tetris[y + form[form_kind][rotate_kind][i*2+1]][x + form[form_kind][rotate_kind][i*2]];
		if (temp > 1) return NOT_EMPTY;
	}

	return IS_EMPTY;
}

//���� �� �� �Ʒ��� ������
bool Go_Down()
{
	int chk1;

	chk1 = Check_board(screen_x, screen_y + 1);

	if (chk1 == NOT_EMPTY)
	{
		return false;
	}
	else
	{
		Delete_form();
		screen_y++;
		Print_form();
		return true;
	}
}

//Ű �Է�
int Select()
{
	int chk1, chk2;
	int prev_rotate, new_rotate;
	int ret;

	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('a'))
	{
		chk1 = Check_board(screen_x - 1, screen_y);
		if (chk1 == IS_EMPTY)
		{
			Delete_form();
			screen_x--;
			Print_form();
			return true;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('d'))
	{
		chk1 = Check_board(screen_x + 1, screen_y);
		if (chk1 == IS_EMPTY)
		{
			Delete_form();
			screen_x++;
			Print_form();
			return true;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S') || GetAsyncKeyState('s'))
	{
		Go_Down();
	}
	if (GetAsyncKeyState('z') || GetAsyncKeyState('Z'))
	{
		prev_rotate = rotate_kind;
		if (rotate_kind == 0) rotate_kind = 3;
		else rotate_kind--;
		new_rotate = rotate_kind;

		chk1 = Check_board(screen_x, screen_y);

		if (chk1 == IS_EMPTY)
		{
			rotate_kind = prev_rotate;
			Delete_form();
			rotate_kind = new_rotate;
			Print_form();
		}
		else
		{
			rotate_kind = prev_rotate;
		}
	}
	if (GetAsyncKeyState('x') || GetAsyncKeyState('X'))
	{
		prev_rotate = rotate_kind;
		if (rotate_kind == 3) rotate_kind = 0;
		else rotate_kind++;
		new_rotate = rotate_kind;

		chk1 = Check_board(screen_x, screen_y);

		if (chk1 == IS_EMPTY)
		{
			rotate_kind = prev_rotate;
			Delete_form();
			rotate_kind = new_rotate;
			Print_form();
		}
		else
		{
			rotate_kind = prev_rotate;
		}
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		while (Go_Down());
		return true;
	}

	return true;
}

//�ð� ����
void Timing(DWORD i = 200)
{
	DWORD thisTickCount;
	static DWORD lastTickCount = 0;
	DWORD delay = i;
	thisTickCount = GetTickCount();
	while (1)
	{
		if ((thisTickCount - lastTickCount) > delay)
		{
			lastTickCount = thisTickCount;
			return;
		}
		thisTickCount = GetTickCount();
	}
}

//���� �������� üũ�Ѵ�
void CheckGameOver()
{
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			if (tetris[j][i] == State::stuck)
			{
				IS_ALIVE = false;
				return;
			}
		}
	}
}

//������ �ű��
void MoveData(int row)
{
	for (int i = row - 1; i >= 0; i--)
		for (int j = 1; j < 11; j++)
		{
			tetris[i + 1][j] = tetris[i][j];
		}
}

//�ٿ� �� ĭ�� �ִ��� üũ�Ѵ�
int CheckLine(int row)
{
	for (int i = 1; i < 11; i++)
	{
		if (tetris[row][i] == 0) return IS_EMPTY;
	}
	return NOT_EMPTY;
}

//���� ���� �ִ��� üũ�Ѵ�
void CheckClearLine()
{
	int ret;
	int bonus_score = 0, temp = 1;

	for (int i = 0; i < 4; i++)
	{
		ret = screen_y + form[form_kind][rotate_kind][i * 2 + 1];
		if (CheckLine(ret) == NOT_EMPTY)
		{
			bonus_score += temp;
			temp += temp;
			Score += 1 + bonus_score;
			MoveData(ret);
			PrintScore();
			PrintAll();
		}
	}
}

void main()
{
	int chk1, chk2;
	bool IS_MOVING = true;
	CursorOff();

	InitBoard();

	srand((unsigned)time(NULL));
	form_kind_next = rand() % 7;

	while (IS_ALIVE)
	{
		Delete_next();

		screen_x = 4;
		screen_y = 0;
		form_kind = form_kind_next;
		form_kind_next = rand() % 7;
		rotate_kind = 0;

		Print_form();
		Print_next();
		PrintScore();

		chk1 = Check_board(screen_x, screen_y);
		chk2 = Check_board(screen_x, screen_y + 1);

		if (chk1 == NOT_EMPTY && chk2 == NOT_EMPTY) break;
		else if (chk1 == NOT_EMPTY && chk2 == IS_EMPTY) IS_MOVING = true;

		do
		{
			if (GetAsyncKeyState(VK_ESCAPE))
				return;
			IS_MOVING = Go_Down();
			Timing();
		} while (Select() && IS_MOVING);

		for (int i = 0; i < 3 || !Select(); i++)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
				return;
			Go_Down();
			Timing();
		}

		for (int i = 0; i < 4; i++)
		{
			Go_Down();
			CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2, screen_y + form[form_kind][rotate_kind][i * 2 + 1]);
			printf("%s", SpriteArray[State::stuck]);

			tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]][screen_x + form[form_kind][rotate_kind][i * 2]] = State::stuck;
			CheckGameOver();
			CheckClearLine();
		}
	}

	InitBoard();
	CurrentXY(37, 9);
	printf("GAME OVER!");
	CursorOn();

#if _DEBUG //����� ����� â�� �ٷ� ���� �ʴ´�
	system("pause>nul");
#endif
}