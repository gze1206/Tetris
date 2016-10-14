#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>

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
int num = 0;

//����� ������ ��Ÿ���� ����
int form_kind;
//����� ������ ��Ÿ���� ����
int form_kind_next;

//����� ȸ�� ���¸� ��Ÿ���� ����
int rotate_kind;

//ȭ�� �󿡼��� ��ǥ
int screen_x = 0, screen_y = 0;

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
		printf("%s", SpriteArray[3]);
		CurrentXY(52, i);
		printf("%s", SpriteArray[3]);
	}
	for (int i = 0; i < 12; i++)
	{
		CurrentXY(30+i*2, 20);
		printf("%s", SpriteArray[3]);
	}
}

void Print_form()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2, screen_y + form[form_kind][rotate_kind][i * 2 + 1]);
		printf("%s", SpriteArray[1]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]][screen_x + form[form_kind][rotate_kind][i * 2]] = 1;
	}
}

void Print_next()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(56 + (form[form_kind_next][0][i * 2]) * 2, 5 + (form[form_kind_next][0][i * 2 + 1]));
		printf("%s", SpriteArray[1]);
	}
}

void Delete_form()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2, screen_y + form[form_kind][rotate_kind][i * 2 + 1]);
		printf("%s", SpriteArray[0]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]][screen_x + form[form_kind][rotate_kind][i * 2]] = 0;
	}
}

void Delete_next()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(56 + (form[form_kind_next][0][i * 2]) * 2, 5 + (form[form_kind_next][0][i * 2 + 1]));
		printf("%s", SpriteArray[0]);
	}
}

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

int Check_board(int x, int y)
{
	int temp = 0;

	for (int i = 0; i < 4; i++)
	{
		temp = tetris[y + form[form_kind][rotate_kind][i*2+1]][x + form[form_kind][rotate_kind][i*2]];
		if (temp > 1) return 1;
	}

	return 0;
}

int Select()
{
	if (GetAsyncKeyState(VK_LEFT))
	{

	}
	if (GetAsyncKeyState(VK_RIGHT))
	{

	}
	if (GetAsyncKeyState(VK_DOWN))
	{

	}
	if (GetAsyncKeyState(VK_UP))
	{

	}
	if (GetAsyncKeyState(VK_SPACE))
	{

	}

	return 1;
}

void main()
{
	int chk, chk2;
	CursorOff();

	InitBoard();

	srand((unsigned)time(NULL));
	form_kind_next = rand() % 7;

	while (true)
	{
		Delete_form();
		Delete_next();

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			return;

		screen_x = 4;
		screen_y = 0;
		form_kind = form_kind_next;
		form_kind_next = rand() % 7;
		rotate_kind = 0;

		Print_form();
		Print_next();

		chk = Check_board(screen_x, screen_y);
		chk2 = Check_board(screen_x, screen_y + 1);

		//�ϰ� �Ҵ�
		if (chk == 0 && chk2 == 1)
		{

		}
		else if (chk == 1 && chk2 == 1) break;

		do
		{

		} while (Select());
	}

#if _DEBUG //����� ����� â�� �ٷ� ���� �ʴ´�
	system("pause>nul");
#endif
}