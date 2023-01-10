//��Χ��(NoGo) 
//�汾�ţ�5.2.2
//���ڣ�2021/1/17
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<iomanip>
#include<Windows.h>
#include<fstream>
#include<vector>
#pragma comment(lib,"Winmm.lib")
#define High 623
#define Width 600
#define inf 10000000
using namespace std;

//����ǩ��---------------------------------------------------------------------------------------------------------------

int startMenu();//�������ܣ���Ϸ��ʼ�����Լ��˵�����

void chessboard();//�������ܣ�����easyX��������

void updateWithInput();//�������ܣ�ʵ����꽻��

int findLocation(int x, int mod);//�������ܣ��ҵ�����ĸ��(mod==0��ʾ�Һ�����x��mod==1��ʾ��������y)

int startUp();//�������ܣ����ݵĳ�ʼ��

void rules();//�������ܣ����ܹ���

void show();//�������ܣ���ʾ����

void chooseDifficulty();//�������ܣ�ѡ����Ϸ�Ѷ�

bool inBoard(int x, int y);//�������ܣ��жϵ��Ƿ��������ڲ�

bool haveAir(int fx, int fy);//�������ܣ��ж��Ƿ��С�������true���С�����

void decideByComputer();//�������ܣ����Ծ��߲����ӣ��ͽ�����棩

int winOrLose();//�������ܣ��ж��Ƿ�ʤ��

void endGame(int mod);//�������ܣ���Ϸ�Ľ�������

void readRecordFile(int(*board)[10]);//�������ܣ���ȡ��Ϸ�����ļ��浵

void writeRecordFile(int n, int(*board)[10]);//�������ܣ��洢��Ϸ�����ļ��浵

//ȫ�ֱ���---------------------------------------------------------------------------------------------------------------

int cnt = 0, cnt_w = 0, cnt_b = 0;//���ڼ���
int gamemode = 0;//��Ϸģʽ
int state;//��Ϸ״̬
int difficulty = 0;//��Ϸ�Ѷ�
int locations_x[10][10] = { 0 };//���ڴ�����λ��x
int locations_y[10][10] = { 0 };//���ڴ�����λ��y
int sum = 0;//���ڼ��㡰����
int table[10][10] = { 0 };//���ڼ�¼���ӣ��Ա���������жϣ�0��ʾ�գ�1��ʾ���ӣ�-1��ʾ����
bool haveAir_visited[10][10] = { false };//���ڵݹ�
int air[10][10] = { 0 };//���ڴ��桰�����ļ���ֵ
int flag = 0, judge = 0;//�����ж�
IMAGE background;//���屳��ͼƬ
int nowx, nowy, nx, ny;//��������
int di[4] = { 0,0,1,-1 };//��������
int dj[4] = { 1,-1,0,0 };//��������

//�����ľ���ʵ��------------------------------------------------------------------------------------------------------------

int startMenu()
{
	//easyX����
	//��ʾ����
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(70, 0, _T("����"));
	outtextxy(Width * 0.2, High * 0.3, _T("��Χ�� NoGo"));
	settextstyle(44, 0, _T("����"));
	outtextxy(Width * 0.32, High * 0.55, _T("1.��ʼ����Ϸ"));
	outtextxy(Width * 0.32, High * 0.65, _T("2.������Ϸ"));
	outtextxy(Width * 0.32, High * 0.75, _T("3.�˳���Ϸ"));
	MOUSEMSG m;//���������Ϣ
	while (1)
	{
		m = GetMouseMsg();//��ȡ�����Ϣ
		if (m.uMsg == WM_LBUTTONDOWN)//����������
		{
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			if (m.x >= Width * 0.32 && m.y <= High * 0.61 && m.y >= High * 0.55 && m.x <= Width * 0.75)
			{
				rules();//��ʾ����
			}
			else if (m.y >= High * 0.65 && m.x >= Width * 0.32 && m.y <= High * 0.71 && m.x <= Width * 0.7)
			{
				readRecordFile(table);//����
				state = 1;
				if (winOrLose() != 0)
					state = 2;
				return state;
			}
			else if (m.y >= High * 0.75 && m.x >= Width * 0.32 && m.y <= High * 0.81 && m.x <= Width * 0.7)
				exit(0);
			else
				continue;
			return state;
		}
	}

}

void draw()
{
	//�ڶ���֮��ָ����̽���
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (table[i][j] == 1)
			{
				setfillcolor(BLACK);
				setlinecolor(BLACK);
				fillcircle(i * 60, j * 60, 27);
			}
			else if (table[i][j] == -1)
			{
				setfillcolor(WHITE);
				setlinecolor(WHITE);
				fillcircle(i * 60, j * 60, 27);
			}
		}
}

void chessboard()
{
	cleardevice();//����
	putimage(0, 0, &background);
	int step = 60;//���ڶ�λ
	setlinestyle(PS_SOLID, 2);//��ʵ�ߣ����Ϊ2������
	setcolor(RGB(0, 0, 0));//����Ϊ��ɫ

	for (int i = 1; i <= 9; i++)//�����ߡ�����
	{
		line(i * step, 1 * step, i * step, 9 * step);
		line(1 * step, i * step, 9 * step, i * step);
	}
	//��¼ÿһ���������
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			locations_x[i][j] = i * step;
			locations_y[i][j] = j * step;
			setfillcolor(BLACK);
			fillcircle(locations_x[i][j], locations_y[i][j], 2);

		}
	}
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(29, 0, _T("����"), 0, 0, 1000, 0, 0, 0);
	outtextxy(Width * 0.18, High * 0.94, _T("��Χ��"));
	settextstyle(29, 0, _T("����"), 0, 0, 1000, 0, 0, 0);
	outtextxy(Width * 0.38, High * 0.94, _T("���沢�˳�"));
	outtextxy(Width * 0.7, High * 0.94, _T("NoGo"));
}

void updateWithInput()
{
	MOUSEMSG m;//���������Ϣ
	while (1)
	{
		m = GetMouseMsg();//��ȡ�����Ϣ
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= Width * 0.35 && m.x <= Width * 0.65 && m.y >= High * 0.94)
			{
				writeRecordFile(cnt, table);//�浵
				exit(0);//�˳���Ϸ
			}
			else
			{
				if (cnt % 2 == 1)
				{
					//��������
					setfillcolor(WHITE);
					setlinecolor(WHITE);
					int mmx = findLocation(m.x, 0);
					int mmy = findLocation(m.y, 1);
					if (table[mmx / 60][mmy / 60] != 0)//λ�ò��Ϸ����ѱ�ռ�ã�
					{
						flag = 1;
						return;
					}
					//������Ч
					mciSendString(_T("close jpmusic"), NULL, 0, NULL);
					mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
					mciSendString(_T("play jpmusic"), NULL, 0, NULL);
					fillcircle(mmx, mmy, 27);
					table[mmx / 60][mmy / 60] = -1;
					nowx = mmx / 60; nowy = mmy / 60;
				}
				else
				{
					//ͬ��
					int mmx = findLocation(m.x, 0);
					int mmy = findLocation(m.y, 1);
					if (table[mmx / 60][mmy / 60] != 0)
					{
						flag = 1;
						return;
					}
					if (mmx == locations_x[5][5] && mmy == locations_y[5][5] && cnt == 0)
					{
						flag = 1;
						return;
					}
					mciSendString(_T("close jpmusic"), NULL, 0, NULL);
					mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
					mciSendString(_T("play jpmusic"), NULL, 0, NULL);

					if (gamemode == 1)
					{
						setfillcolor(BLACK);
						setlinecolor(BLACK); table[mmx / 60][mmy / 60] = 1;
					}
					else if (gamemode == 2)
					{
						setfillcolor(WHITE);
						setlinecolor(WHITE); table[mmx / 60][mmy / 60] = -1;
					}
					fillcircle(mmx, mmy, 27);
					nowx = mmx / 60; nowy = mmy / 60;
				}
			}
			return;
		}
	}
}

int findLocation(int x, int mod)
{
	//��λ
	int mini = 1000000;
	int mini_x = 0, mini_y = 0;
	if (mod == 0)
	{
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
			{
				if (abs(locations_x[i][j] - x) < mini)
				{
					//Ѱ������ĸ�㣨x���꣩
					mini = abs(locations_x[i][j] - x);
					mini_x = locations_x[i][j];
				}
			}
		return mini_x;
	}
	if (mod == 1)
	{
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
			{
				if (abs(locations_y[i][j] - x) < mini)
				{
					//Ѱ������ĸ�㣨y���꣩
					mini = abs(locations_y[i][j] - x);
					mini_y = locations_y[i][j];
				}
			}
		return mini_y;
	}
}

int startUp()
{
	initgraph(Width, High);//��ʼ������
	mciSendString(_T("open ..\\music\\bgm.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
	//��ȡ���ھ��
	HWND hwnd = GetHWnd();
	//���ô��ڱ�������
	SetWindowText(hwnd, _T("NoGo ��Χ�� By ������"));
	loadimage(&background, _T("..\\picture\\chessboard.png"));//���뱳��
	putimage(0, 0, &background);
	int a = startMenu();
	if (a == 1)
	{
		cleardevice();
		putimage(0, 0, &background);
		chessboard();
		draw();
	}
	else if (a == 2)
	{
		cleardevice();
		putimage(0, 0, &background);
		chessboard();
		draw();
		MOUSEMSG s;
		while (1)
		{
			s = GetMouseMsg();
			if (s.uMsg == WM_LBUTTONDOWN)
			{
				if (cnt % 2 == 0 && haveAir(nx, ny) == false)
				{
					endGame(0);
					_getch();
					closegraph();
					return 1;
				}
				else
				{
					endGame(winOrLose());
					_getch();
					closegraph();
					return 1;
				}
			}
		}
	}
	return 0;
}

void rules()
{
	cleardevice();//����
	loadimage(&background, _T("..\\picture\\chessboard.png"));//���뱳��
	putimage(0, 0, &background);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("����"));
	outtextxy(Width * 0.26, High * 0.12, _T("��Ϸ������"));
	settextstyle(23, 0, _T("����"));
	outtextxy(Width * 0.05, High * 0.25, _T("1.�������֣�˫���������ӣ����Ӻ����Ӳ����ƶ���"));
	outtextxy(Width * 0.05, High * 0.30, _T("2.���ĵ�Ŀ�겻�ǳԵ��Է������ӣ�����ռ����̣�"));
	outtextxy(Width * 0.05, High * 0.35, _T("3.��һ�����Ӻ�Ե��˶Է������ӣ�����һ���и���"));
	outtextxy(Width * 0.05, High * 0.40, _T("4.���Ľ�ֹ��ɱ��������ɱһ���и���"));
	outtextxy(Width * 0.05, High * 0.45, _T("5.���Ľ�ֹ����(pass)������һ���и���"));
	outtextxy(Width * 0.05, High * 0.50, _T("6.���Ľ��ֻ��ʤ����û�к��塣"));
	outtextxy(Width * 0.40, High * 0.76, _T("�ر���л��"));
	outtextxy(Width * 0.25, High * 0.80, _T("������ѧ��Ϣ��ѧ����ѧԺ"));
	outtextxy(Width * 0.26, High * 0.84, _T("Microsoft Visual Studio"));
	outtextxy(Width * 0.37, High * 0.88, _T("CSDN���й�֪��"));
	settextstyle(50, 0, _T("����"));
	outtextxy(Width * 0.11, High * 0.60, _T("����˴��Կ�ʼ��Ϸ��"));
	settextstyle(23, 0, _T("����"));
	MOUSEMSG m;//���������Ϣ
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			if (m.x >= Width * 0.11 && m.x <= Width * 0.90 && m.y >= High * 0.60 && m.y <= High * 0.68)
			{
				show();
				break;
			}
		}
	}
	
}

void show()
{
	cleardevice();//����
	loadimage(&background, _T("..\\picture\\chessboard.png"));//���뱳��
	putimage(0, 0, &background);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	
	MOUSEMSG m;
	settextstyle(60, 0, _T("����"));
	outtextxy(Width * 0.09, High * 0.3, _T("��ѡ���Ⱥ���˳��:"));
	settextstyle(44, 0, _T("����"));
	outtextxy(Width * 0.32, High * 0.55, _T("1.�������"));
	outtextxy(Width * 0.32, High * 0.65, _T("2.��Һ���"));
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			if (m.x >= Width * 0.32 && m.y <= High * 0.61 && m.y >= High * 0.55 && m.x <= Width * 0.75)
			{
				cleardevice();
				putimage(0, 0, &background);
				cnt = 0; gamemode = 1;
				chooseDifficulty();
				
			}
			else if (m.y >= High * 0.65 && m.x >= Width * 0.32 && m.y <= High * 0.71 && m.x <= Width * 0.7)
			{
				cleardevice();
				putimage(0, 0, &background);
				cnt = 1; gamemode = 2;
				chooseDifficulty();
			}
			else
				continue;
			return;
		}
	}
}

void chooseDifficulty()
{
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(60, 0, _T("����"));
	outtextxy(Width * 0.15, High * 0.3, _T("��ѡ����Ϸ�Ѷ�:"));
	settextstyle(44, 0, _T("����"));
	outtextxy(Width * 0.38, High * 0.55, _T("1.����"));
	outtextxy(Width * 0.38, High * 0.65, _T("2.�н�"));
	outtextxy(Width * 0.38, High * 0.75, _T("3.�߽�"));
	MOUSEMSG m;//���������Ϣ
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			if (m.x >= Width * 0.32 && m.y <= High * 0.61 && m.y >= High * 0.55 && m.x <= Width * 0.75)
			{
				difficulty = 1;
				chessboard();
			}
			else if (m.y >= High * 0.65 && m.x >= Width * 0.32 && m.y <= High * 0.71 && m.x <= Width * 0.7)
			{
				difficulty = 2;
				chessboard();
			}
			else if (m.y >= High * 0.75 && m.x >= Width * 0.32 && m.y <= High * 0.81 && m.x <= Width * 0.7)
			{
				difficulty = 3;
				chessboard();
			}
			else
				continue;
			return;
		}
	}
}

void decideByComputer()
{
	//�������
	srand((unsigned)time(NULL));
	int mmx = 0, mmy = 0;
	while (1)
	{
		mmx = rand() % 9 + 1;
		mmy = rand() % 9 + 1;
		if (table[mmx][mmy] == 0)
		{
			table[mmx][mmy] = (gamemode == 1) ? -1 : 1;
			if (haveAir(mmx, mmy) == 1)
			{
				table[mmx][mmy] = 0;
				break;
			}

		}
	}
	Sleep(400);
	mciSendString(_T("close jpmusic"), NULL, 0, NULL);
	mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
	mciSendString(_T("play jpmusic"), NULL, 0, NULL);
	if (gamemode == 1)
	{
		setfillcolor(WHITE);
		setlinecolor(WHITE); table[mmx][mmy] = -1;
	}
	else if (gamemode == 2)
	{
		setfillcolor(BLACK);
		setlinecolor(BLACK); table[mmx][mmy] = 1;
	}
	fillcircle(mmx * 60, mmy * 60, 27);
}

struct decide_more
{
	//ģ������
	bool goStep(int x, int y, int c)
	{
		table[x][y] = c;
		if (getQi(x, y) == 0) return false;
		return true;
	}
	//������Ӻ��Ƿ���ӣ����ǲ�һ���������Ҳ�ͬ��ɫ���������û���˾ͱ����ˣ�
	bool checkQi(int x, int y)
	{
		for (int t = 0; t < 4; t++)
		{
			int tx = x + di[t], ty = y + dj[t];
			if (tx >= 1 && tx <= 9 && ty >= 1 && ty <= 9)
			{
				if (table[tx][ty] != 0 && getQi(tx, ty) == 0) return false;
			}
		}
		return true;
	}
	bool checkStep(int x, int y)
	{
		if (!inBoard(x, y)) return false;
		if (table[x][y] != 0) return false;
		if (x == 5 && y == 5 && cnt == 0) return false;
		return true;
	}
	//��������Ƿ��������
	bool testgoStep(int x, int y, int c)
	{
		if (!checkStep(x, y)) return false;
		bool flag = goStep(x, y, c) && checkQi(x, y);
		ungoStep(x, y);
		return flag;
	}
	void ungoStep(int x, int y)
	{
		table[x][y] = 0;
	}
	int evaluate1(int colour)
	{
		int ans = 0;
		int opp_colour = (colour == 1) ? -1 : 1;
		for (int x = 1; x < 10; x++)
		{
			for (int y = 1; y < 10; y++)
			{
				if (table[x][y] != 0)
					continue;
				else
				{
					if (testgoStep(x, y, colour) == 1)
					{
						ans++;
						ungoStep(x, y);
					}
					if (testgoStep(x, y, opp_colour) == 1)
					{
						ans--;
						ungoStep(x, y);
					}
				}
			}
		}
		return ans;
	}
	int chk[15][15] = { 0 };
	int dfs(int x, int y, int colour)
	{
		if (chk[x][y] == -1)
			return 0;
		chk[x][y] = -1;
		int res = 0;
		for (int i = 0; i < 4; i++)
		{
			int tx = x + di[i], ty = y + dj[i];
			if (inBoard(tx, ty) == 1)
			{
				if (table[tx][ty] == 0 && !chk[tx][ty])
				{
					res++;
					chk[tx][ty] = 1;
				}
				if (table[tx][ty] == colour)
				{
					res += dfs(tx, ty, colour);
				}
			}
		}
		return res;
	}
	int getQi(int x, int y)
	{
		memset(chk, 0, sizeof(chk));
		return dfs(x, y, table[x][y]);
	}
	int decideByComputer_more()
	{
		int decideList_x[81], decideList_y[81];
		int valueList[10][10];
		int col;
		int mmx, mmy;
		int maxValue = -1000000;
		int maxNum = 0;
		if (gamemode == 1)col = -1;
		else col = 1;
		int Threshold = 18;
		for (int j = 1; j < 10; j++)
			for (int i = 1; i < 10; i++)
			{
				if (testgoStep(i, j, col) == 1)
				{
					table[i][j] = col;
					int qi = -getQi(i, j);
					if (cnt <= 20)
						valueList[i][j] = 3 * evaluate1(col) + qi;
					else
					{
						valueList[i][j] = evaluate1(col);
					}
					if (valueList[i][j] >= maxValue)
					{
						maxValue = valueList[i][j];
					}
					table[i][j] = 0;
				}
			}
		for (int i = 1; i < 10; i++)
			for (int j = 1; j < 10; j++)
			{
				if (valueList[i][j] == maxValue)
				{
					decideList_x[maxNum] = i;
					decideList_y[maxNum] = j;
					maxNum++;
				}
			}
		if (maxNum == 0)
			return 0;
		srand((unsigned)time(NULL));
		int position = rand() % maxNum;
		mmx = decideList_x[position];
		mmy = decideList_y[position];
		Sleep(300);

		mciSendString(_T("close jpmusic"), NULL, 0, NULL);
		mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
		mciSendString(_T("play jpmusic"), NULL, 0, NULL);
		if (gamemode == 1)
		{
			setfillcolor(WHITE);
			setlinecolor(WHITE); table[mmx][mmy] = -1;
		}
		else if (gamemode == 2)
		{
			setfillcolor(BLACK);
			setlinecolor(BLACK); table[mmx][mmy] = 1;
		}
		fillcircle(mmx * 60, mmy * 60, 27);
		return 1;
	}
}S;

struct decide_most
{
	int chk[15][15] = { 0 };
	int col[15][15], colcnt;
	//����ǰλ�û��ߵ�ǰλ������ͬ����ɫ�����ġ�������
	int dfs(int x, int y, int colour)
	{
		if (chk[x][y] == -1)
			return 0;
		chk[x][y] = -1;
		int res = 0;
		for (int i = 0; i < 4; i++)
		{
			int tx = x + di[i], ty = y + dj[i];
			if (inBoard(tx, ty) == 1)
			{
				if (table[tx][ty] == 0 && !chk[tx][ty])
				{
					res++;
					chk[tx][ty] = 1;
				}
				if (table[tx][ty] == colour)
				{
					res += dfs(tx, ty, colour);
				}
			}
		}
		return res;
	}
	int getQi(int x, int y)
	{
		memset(chk, 0, sizeof(chk));
		return dfs(x, y, table[x][y]);
	}
	//ģ������
	bool goStep(int x, int y, int c) 
	{
		table[x][y] = c;
		if (getQi(x, y) == 0) return false;
		return true;
	}
    //ȡ��ģ�����ӣ��ָ�״̬
	void ungoStep(int x, int y) 
	{
		table[x][y] = 0;
	}
	//������Ӻ��Ƿ���ӣ����ǲ�һ���������Ҳ�ͬ��ɫ���������û���˾ͱ����ˣ�
	bool checkQi(int x, int y) 
	{ 
		for (int t = 0; t < 4; t++) 
		{
			int tx = x + di[t], ty = y + dj[t];
			if (tx >= 1 && tx <= 9 && ty >= 1 && ty <= 9) 
			{
				if (table[tx][ty] != 0 && getQi(tx, ty) == 0) return false;
			}
		}
		return true;
	}
	bool checkStep(int x, int y) 
	{
		if (!inBoard(x,y)) return false;
		if (table[x][y] != 0) return false;
		if (x == 5 && y == 5 && cnt == 0) return false;
		return true;
	}
	//��������Ƿ��������
	bool testgoStep(int x, int y, int c) 
	{
		if (!checkStep(x, y)) return false;
		bool flag = goStep(x, y, c) && checkQi(x, y);
		ungoStep(x, y);
		return flag;
	}
	void color(int x, int y) 
	{
		if (col[x][y]) return;
		col[x][y] = colcnt;
		for (int i = 0; i < 4; i++) 
		{
			int tx = x + di[i], ty = y + dj[i];
			if (tx >= 1 && tx <= 9 && ty >= 1 && ty <= 9 && table[tx][ty] == table[x][y]) 
				color(tx, ty);
		}
	}
	//ͳ�������ͬ����ɫ��������ɫ�����
	int calc(int c)
	{
		colcnt = 0;
		memset(col, 0, sizeof(col));
		for (int i = 1; i <= 9; i++) 
		{
			for (int j = 1; j <= 9; j++) 
			{
				if (table[i][j] == c && col[i][j] == 0) 
				{
					++colcnt;
					color(i, j);
				}
			}
		}
		return colcnt;
	}
	//�������棬�󡰾�����������
	int evaluate(int c) 
	{
		int res = 0;
		for (int j = 1; j <= 9; j++) 
		{
			for (int i = 1; i <= 9; i++) 
			{
				if (testgoStep(i, j, c *(-1))) --res;
				else if (testgoStep(i, j, c)) ++res;
			}
		}
		if (c == 1) res = -res;//��ִ����ͷ�תһ�£������ں�������������
		return res;
	}
	//�������棬����һ��������£����ǲ���ת
	int evaluate1(int c) 
	{
		int res = 0;
		for (int j = 1; j <= 9; j++)
		{
			for (int i = 1; i <= 9; i++)
			{
				if (testgoStep(i, j, c*(-1))) --res;
				if (testgoStep(i, j, c)) ++res;
			}
		}
		return res;
	}
	vector<pair<int, int> > vec, vec1;
	int bx, by, prevtime;
	//�о���������
	int canItWin(int c, int dep) 
	{
		int th = evaluate(c);  //��ʱ��Evaluate�Ǽ��㵱ǰδ���¾������������
		int num = 0;
		if (clock() - prevtime > 1000) return -1;
		for (int j = 1; j <= 9; j++) 
		{
			for (int i = 1; i <= 9; i++) 
			{
				if (!checkStep(i, j)) continue;
				if (goStep(i, j, c) && checkQi(i, j)) 
				{
					if ((c == -1 && evaluate(c*(-1)) >= th) || (c == 1 && evaluate(c*(-1)) <= th)) 
					{
						num++;
						int r = canItWin(c *(-1), dep + 1);
						if (r == 0) 
						{
							if (dep != 0) ungoStep(i, j);
							return 1;
						}
						if (r == -1) 
						{
							ungoStep(i, j); return -1;
						}
					}
				}
				ungoStep(i, j);
				if (num > 7) return 0;
			}
		}
		return 0;
	}
	int my; int qi[100] = { 0 };
	//ѡ����һ��
	bool chooseNextStep() 
	{
		vec.clear(); vec1.clear();
		for (int j = 1; j <= 9; j++) 
		{
			for (int i = 1; i <= 9; i++) 
			{
				if (!checkStep(i, j)) continue;
				if (goStep(i, j, my)) 
				{
					if (checkQi(i, j)) vec.push_back(make_pair(i, j));
				}
				ungoStep(i, j);
			}
		}
		if (vec.size() == 0) return false;
		for (int t = 0; t < vec.size(); ++t) 
		{
			goStep(vec[t].first, vec[t].second, my);
			qi[t] = (cnt <= 20 ? -getQi(vec[t].first, vec[t].second) : calc(my));
			ungoStep(vec[t].first, vec[t].second);
		}
		int ch = 0; int mm = 0; int now = -inf; int ddx, ddy;
		for (int t = 0; t < vec.size(); ++t) 
		{
			if (qi[t] > qi[ch]) 
			{
				vec1.clear(); ch = t;
				vec1.push_back(make_pair(vec[t].first, vec[t].second));
			}
			else if (qi[t] == qi[ch])
			{
				vec1.push_back(make_pair(vec[t].first, vec[t].second));
			}
		}
		int p = rand() % vec1.size();
		if (cnt <= 55) 
		{
			for (int t = 0; t < vec1.size(); t++)
			{
				goStep(vec1[t].first, vec1[t].second, my);
				if (evaluate1(my) > now)
				{
					now = evaluate1(my);
					ddx = vec1[t].first; ddy = vec1[t].second;
				}
				ungoStep(vec1[t].first, vec1[t].second);
			}
			goStep(ddx, ddy, my);
			Sleep(500);
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			if (gamemode == 1)
			{
				setfillcolor(WHITE);
				setlinecolor(WHITE); table[ddx][ddy] = -1;
			}
			else if (gamemode == 2)
			{
				setfillcolor(BLACK);
				setlinecolor(BLACK); table[ddx][ddy] = 1;
			}
			fillcircle(ddx * 60, ddy * 60, 27);
		}
		else {
			prevtime = clock();
			if (canItWin(my, 0) != 1) 
			{
				goStep(vec1[p].first, vec1[p].second, my);
				mciSendString(_T("close jpmusic"), NULL, 0, NULL);
				mciSendString(_T("open ..\\music\\����.wav alias jpmusic"), NULL, 0, NULL);
				mciSendString(_T("play jpmusic"), NULL, 0, NULL);
				if (gamemode == 1)
				{
					setfillcolor(WHITE);
					setlinecolor(WHITE); table[vec1[p].first][vec1[p].second] = -1;
				}
				else if (gamemode == 2)
				{
					setfillcolor(BLACK);
					setlinecolor(BLACK); table[vec1[p].first][vec1[p].second] = 1;
				}
				fillcircle(vec1[p].first * 60, vec1[p].second * 60, 27);
			}
		}
		return true;
	}
}SS;

bool inBoard(int x, int y)
{
	if (x >= 1 && y >= 1 && x <= 9 && y <= 9)//������֮��
		return true;
	return false;
}

bool haveAir(int fx, int fy)
{
	haveAir_visited[fx][fy] = true;
	bool flag = false;
	for (int dir = 0; dir < 4; dir++)
	{
		//�ݹ�
		int dx = fx + di[dir], dy = fy + dj[dir];
		if (inBoard(dx, dy) == true)
		{
			if (table[dx][dy] == 0)
				flag = true;
			if (table[dx][dy] == table[fx][fy] && haveAir_visited[dx][dy] == false)
				if (haveAir(dx, dy) == true)
					flag = true;
		}
	}
	return flag;
}

int winOrLose()
{
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			memset(haveAir_visited, false, sizeof(haveAir_visited));
			if (haveAir(i, j) == false)//�����һ��λ��û�������������Ϸ
			{
				if (table[i][j] == 1)
				{
					return 1;
				}
				else if (table[i][j] == -1)
				{
					return -1;
				}
			}
		}
	}
	return 0;
}

void endGame(int mod)
{
	writeRecordFile(cnt, table);//�浵
	if (mod == 1)
	{
		cleardevice();
		putimage(0, 0, &background);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(70, 0, _T("������κ"), 0, 0, 0, 0, 0, 0);
		outtextxy(Width * 0.25, High * 0.3, _T("��Ϸ������"));
		if (gamemode == 1)
			outtextxy(Width * 0.25, High * 0.4, _T("��ʤ���ˣ�"));
		else if (gamemode == 2)
			outtextxy(Width * 0.13, High * 0.4, _T("���ӣ������ˣ�"));
		settextstyle(30, 0, _T("����"));
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);
		if (gamemode == 1)
		{
			mciSendString(_T("close winmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\win.wav alias winmusic"), NULL, 0, NULL);
			mciSendString(_T("play winmusic"), NULL, 0, NULL);
		}
		else if (gamemode == 2)
		{
			mciSendString(_T("close losemusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\lose.wav alias losemusic"), NULL, 0, NULL);
			mciSendString(_T("play losemusic"), NULL, 0, NULL);
		}
		outtextxy(Width * 0.33, High * 0.8, _T("����������˳�"));
	}
	else if (mod == -1)
	{
		cleardevice();
		putimage(0, 0, &background);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(70, 0, _T("������κ"), 0, 0, 0, 0, 0, 0);
		outtextxy(Width * 0.25, High * 0.3, _T("��Ϸ������"));
		if (gamemode == 1)
			outtextxy(Width * 0.13, High * 0.4, _T("���ӣ������ˣ�"));
		else if (gamemode == 2)
			outtextxy(Width * 0.25, High * 0.4, _T("��ʤ���ˣ�"));
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);
		if (gamemode == 1)
		{
			mciSendString(_T("close losemusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\lose.wav alias losemusic"), NULL, 0, NULL);
			mciSendString(_T("play losemusic"), NULL, 0, NULL);
		}
		else if (gamemode == 2)
		{
			mciSendString(_T("close winmusic"), NULL, 0, NULL);
			mciSendString(_T("open ..\\music\\win.wav alias winmusic"), NULL, 0, NULL);
			mciSendString(_T("play winmusic"), NULL, 0, NULL);

		}
		settextstyle(30, 0, _T("����"));
		outtextxy(Width * 0.33, High * 0.8, _T("����������˳�"));
	}
	else if (mod == 0)
	{
		cleardevice();
		putimage(0, 0, &background);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(70, 0, _T("������κ"), 0, 0, 0, 0, 0, 0);
		outtextxy(Width * 0.25, High * 0.3, _T("��Ϸ������"));
		settextstyle(60, 0, _T("������κ"));
		outtextxy(Width * 0.08, High * 0.44, _T("������ɱ�������ˣ�"));
		settextstyle(30, 0, _T("����"));
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);
		mciSendString(_T("close losemusic"), NULL, 0, NULL);
		mciSendString(_T("open ..\\music\\lose.wav alias losemusic"), NULL, 0, NULL);
		mciSendString(_T("play losemusic"), NULL, 0, NULL);

		outtextxy(Width * 0.33, High * 0.8, _T("����������˳�"));
	}
}

void writeRecordFile(int n, int(*board)[10])
{
	ofstream outfile;//���������ļ�
	outfile.open("..\\data\\data.txt");
	outfile << n << ' ';
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			outfile << board[i][j] << ' ';//д���������
		}
	}
	outfile << nowx << ' ' << nowy;//д�뵱ǰ�������
	outfile << ' ' << gamemode;//д����Ϸ�Ѷ�
	outfile << ' ' << difficulty;
	outfile.close();
}

void readRecordFile(int(*board)[10])
{
	int n;
	ifstream infile("..\\data\\data.txt");
	infile >> n;
	cnt = n;//�غ���
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			infile >> board[i][j];//�����������
	infile >> nx >> ny;//���뵱ǰ�������
	infile >> gamemode;//������Ϸ�Ѷ�
	infile >> difficulty;
}

//main����---------------------------------------------------------------------------------------------------------------

int main()
{
	int mode = startUp();
	if (mode == 1)
		return 0;
	while (true)
	{
		if (cnt % 2 == 0)
			updateWithInput();
		else
		{
			int result = -1;
			if (difficulty == 1)
				decideByComputer();
			else if (difficulty == 2)
				result = S.decideByComputer_more();
			else if (difficulty == 3)
			{
				if (gamemode == 1)SS.my = -1;
				else SS.my = 1;
				result = SS.chooseNextStep();
			}
			if (result == 0)
			{
				Sleep(500);
				gamemode = 1;
				endGame(1);
				_getch();
				closegraph();
				break;
			}
		}
		if (flag == 1)
		{
			flag = 0;
			continue;
		}
		if (cnt % 2 == 0 && haveAir(nowx, nowy) == false)
		{
			Sleep(500);
			endGame(0);
			_getch();
			closegraph();
			break;
		}
		else if (winOrLose() == 0)
		{
			cnt++;
			continue;
		}
		else
		{
			Sleep(500);
			endGame(winOrLose());
			_getch();
			closegraph();
			break;
		}
	}
	return 0;
}