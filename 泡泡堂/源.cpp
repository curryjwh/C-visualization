#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<easyx.h>
#include <stdlib.h>
#include<math.h>
#include<time.h>
#include"EasyXPng.h"
#include <mmsystem.h>
#define B_SIZE 50//���Ӵ�С
#define B_NUMx 17//���
#define B_NUMy 15//�߶�
#pragma comment(lib,"Winmm.lib")
float distance(float x1, float y1, float x2, float y2)//����һ�����뺯�����ں��������ж�
{
	float distance;
	distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return distance;
}
//ȫ�ֱ�������
char level1[B_NUMy][B_NUMx + 1] = { "wwwwwwwwwwwwwwwww",
									"weebbbwwwwwbbbeew",
								  "wewwewbbbbbwewwew",
								  "wbwwbeewewbbewwbw",
								  "wbbebwwbbewwbbebw",
								  "wwbwbbbbwbbbbwbww",
								  "wwebwwbwwwbwwbbww",
								  "wwbwwbewwwebwweww",
								  "wwbbwwbwwwbwwbeww",
								  "wwbweebbwbbbeweww",
								  "wbbebwweeewwbbebw",
								  "wewwbeewbwbbbwwbw",
								  "wewwbwbebbewbwwew",
								  "weebbbwwwwwbbbeew",
								  "wwwwwwwwwwwwwwwww" };//��ͼ����1

char level2[B_NUMy][B_NUMx + 1] = { "wwwwwwwwwwwwwwwww",
									"weebewwbbbwwbeeew",
								  "wewebwbbwbewbbwew",
								  "wewebbbwwwbbbewbw",
								  "webbbwbbbbbwbbbbw",
								  "wwbwwwewwwbwwwbww",
								  "wwbwbbbeeebbbwbww",
								  "webebwwwwwwwbbebw",
								  "wwbwbbbwwwbbbwbww",
								  "wwbwwwbwwwewwwbww",
								  "webbbwbbebewbbbbw",
								  "wbwebbbwwwbbbewbw",
								  "wewebwbbwbewbbwew",
								  "weebewwbebwwbbeew",
								  "wwwwwwwwwwwwwwwww" };//��ͼ����2
char level3[B_NUMy][B_NUMx + 1] = { "wwwwwwwwwwwwwwwww",
									"weebbbbbbeebeeeew",
								  "wewwwwwwbwwwwwwew",
								  "wbbbbbbbbbbbbbebw",
								  "wwwewbwwwwwbwbwww",
								  "wbbbwbbbbbebwbebw",
								  "wewbwwbwwwbwwewbw",
								  "wbwbbbbwwwbbbbwbw",
								  "wbwbwwbwwwbwwbwbw",
								  "wbeewebbbbebweebw",
								  "wwwbwbwwwwwbwbwww",
								  "wbeebebbebeeebbbw",
								  "wewwwwwwbwwwwwwbw",
								  "weebbbebbbeebeeew",
								  "wwwwwwwwwwwwwwwww" };//��ͼ����3
int playerradius = 25;//��Ұ뾶
struct player
{
	float x;//���x����
	float y;//���y����
	int life;//����Ƿ���
	int boom;//����Ƿ��ܷ���ը��
	int x_move;//�ж���ײ��x����
	int y_move;//�ж���ײ��y����
	int buff;//��ҳ������ש����ը����Χ
};
struct maps
{
	char type;//�ؿ�����
	int canbreak;//�ؿ��Ƿ��ܱ��ƻ�
	int exist;//�ؿ��Ƿ�·
	float x;//�ؿ�����x����
	float y;//�ؿ�����y����
	int x_left;//�ؿ����x����
	int x_right;//�ؿ��ұ�x����
	int y_up;//�ؿ��ϱ�y����
	int y_down;//�ؿ��±�y����
};
struct boom
{
	int x;//ը��x����
	int y;//ը��y����
	int exist;//ը���Ƿ����
	int boomtime;//ը����ը��ʱ
};
int time_type1, time_type2;//��ը��Ч��ʱ
int gamebegin;//�Ƿ�Ϊ�ս�����Ϸ�ĵ�һ������
int gamebegintime;//��һ�����߼�ʱ
int gameaftertime;//
boom boom1, boom2;
maps level[B_NUMy][B_NUMx + 1];//�����ͼ
float v1 = 0.45;
float v2 = 0.45;//�����ƶ��ٶ�
player player1, player2;//��ҽṹ��
//e:empty w:wall b:box p:player a:ash l:light
IMAGE menu_start, boom_png,//����ͼƬ����������ͼ
left_up_1, left_down_1, right_up_1, right_down_1, left_die_1, right_die_1,
left_up_2, left_down_2, right_up_2, right_down_2, left_die_2, right_die_2,
floor_b, floor_E, floor_w, floor_e, floor_d, light, win1, win2, draw;
int lr1, lr2, ud1, ud2;
int end = 0;
int map = 0;
void loadimage()//����ͼƬ��Դ
{
	loadimage(&menu_start, _T("����.png"));
	loadimage(&floor_w, _T("Ӳǽ.png"));
	loadimage(&floor_b, _T("��ǽ.png"));
	loadimage(&light, _T("��ը.png"));
	loadimage(&floor_E, _T("�յ�.png"));
	loadimage(&floor_e, _T("�յ�_����Ӱ.png"));
	loadimage(&floor_d, _T("����ǽ.png"));
	loadimage(&left_up_1, _T("����2.png"));
	loadimage(&left_down_1, _T("����2.png"));
	loadimage(&right_up_1, _T("����2.png"));
	loadimage(&right_down_1, _T("����2.png"));
	loadimage(&left_die_1, _T("����2.png"));
	loadimage(&right_die_1, _T("����2.png"));
	loadimage(&left_up_2, _T("����.png"));
	loadimage(&left_down_2, _T("����.png"));
	loadimage(&right_up_2, _T("����.png"));
	loadimage(&right_down_2, _T("����.png"));
	loadimage(&left_die_2, _T("����.png"));
	loadimage(&right_die_2, _T("����.png"));
	loadimage(&boom_png, _T("��.png"));
	loadimage(&win1, _T("ˮ����.png"));
	loadimage(&win2, _T("������.png"));
	loadimage(&draw, _T("ƽ��.png"));


}
void startup()
{
	if (end == 0)
	{
		mciSendString(_T("open game.mp3 alias bkmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
		initgraph(B_NUMx * B_SIZE, B_NUMy * B_SIZE);//�¿�һ������
	}
	end = 0;
	boom1.exist = 0;
	boom2.exist = 0;
	map++;
	if (map == 4)
	{
		map = 1;
	}

	while (1)
	{
		putimagePng(0, 0, &menu_start);
	begin:
		FlushBatchDraw();
		MOUSEMSG m;
		m = GetMouseMsg();

		if (m.x >= 297 && m.x <= 544 && m.y >= 381 && m.y <= 478) //�ж�����Ƿ��Ƶ���ť����
		{
			setlinecolor(RED);
			rectangle(297, 381, 544, 478);

			if (m.uMsg == WM_LBUTTONDOWN)//�ж��������Ƿ���
			{
				break;
			}
			goto begin;
		}
	}
	gamebegintime = 5000;
	gamebegin = 0;
	int i, j;//ѭ���ñ���
	srand(time(0));
	BeginBatchDraw();//��ʼ������ͼ
	player1 = { B_SIZE + B_SIZE / 2,B_SIZE + B_SIZE / 2,1,1,0 };//��ʼ�������Ϣ
	player2 = { (B_NUMx - 1) * B_SIZE - B_SIZE / 2,(B_NUMy - 1) * B_SIZE - B_SIZE / 2,1,1,0 };//��ʼ�������Ϣ
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)//�������еؿ�
		{
			if (map == 1)
			{
				level[i][j].type = level1[i][j];//Ϊ�ؿ鸳����ֵ
			}
			else if (map == 2)
			{
				level[i][j].type = level2[i][j];//Ϊ�ؿ鸳����ֵ
			}
			else if (map == 3)
			{
				level[i][j].type = level3[i][j];//Ϊ�ؿ鸳����ֵ
			}
			if (level[i][j].type == 'b')
				level[i][j].canbreak = 1;//��������Ϊ�ؿ鸳�Ƿ����ƻ�
			else
				level[i][j].canbreak = 0;//��������Ϊ�ؿ鸳�Ƿ����ƻ�
			if (level[i][j].type == 'b' || level[i][j].type == 'w' || level[i][j].type == 'a')
				level[i][j].exist = 1;//���������ж��Ƿ�·
			else
				level[i][j].exist = 0;//���������ж��Ƿ�·
			level[i][j].x = j * B_SIZE + B_SIZE / 2;//Ϊ�ؿ�������긳ֵ
			level[i][j].y = i * B_SIZE + B_SIZE / 2;
			level[i][j].x_left = j * B_SIZE;
			level[i][j].x_right = (j + 1) * B_SIZE;
			level[i][j].y_up = i * B_SIZE;
			level[i][j].y_down = (i + 1) * B_SIZE;
		}
	}
}
void show()
{
	int i, j;//ѭ���ñ���
	cleardevice();//�Ա�����ɫ�����Ļ
	//������ͼ����
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)
		{
			if (level[i][j].type == 'e')//�հ�����
			{
				if (level[i][j - 1].type == 'w' || level[i][j - 1].type == 'b')
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_E);//��������Ӱ����
				}
				else
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_e);//��������Ӱ����
				}

			}
			else if (level[i][j].type == 'w')//ǽ��
			{

				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_w);//����ǽ��
			}
			else if (level[i][j].type == 'b')//��ǽ
			{


				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_b);//������ǽ

			}
			//�ȸ���һ�������ͼ���ٸ��Ǳ�ը��Ч��ͼ
			else if (level[i][j].type == 'a')//��ը��Ч
			{
				if (level[i][j - 1].type == 'w' || level[i][j - 1].type == 'b')
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_E);//���Ƶ���
				}
				else
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_e);//���Ƶ���
				}

				putimagePng(j * B_SIZE, i * B_SIZE, &light);//��ը��Ч
			}
			else if (level[i][j].type == 'd')
				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_d);
		}
	}



	if (boom1.exist == 1)//���ը������
	{
		putimagePng(boom1.x - 25, boom1.y - 25, &boom_png);//��ʼ��ͼλ��ΪͼƬ���Ͻ�����
	}
	//��ʼʱ����Ϊ0������Ϊ1
	if (boom2.exist == 1)//���ը������
	{
		putimagePng(boom2.x - 25, boom2.y - 25, &boom_png);
	}
	if (player1.life != 0 && player2.life != 0)
	{
		if (ud1 == 0 && lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_down_1);
		}
		if (ud1 == 1 && lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_up_1);
		}
		if (ud1 == 0 && lr1 == 0)//��ʼʱ�Ľ�ɫͼ
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_down_1);
		}
		if (ud1 == 1 && lr1 == 0)
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_up_1);
		}
		if (ud2 == 0 && lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_down_2);
		}
		if (ud2 == 1 && lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_up_2);
		}
		if (ud2 == 0 && lr2 == 0)//��ʼʱ�Ľ�ɫͼ
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_down_2);
		}
		if (ud2 == 1 && lr2 == 0)
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_up_2);
		}

	}
	else if (player1.life == 0 && player2.life != 0)
	{
		if (lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_die_1);
		}
		else if (lr1 == 0)
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_die_1);
		}
		if (ud2 == 0 && lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_down_2);
		}
		if (ud2 == 1 && lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_up_2);
		}
		if (ud2 == 0 && lr2 == 0)
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_down_2);
		}
		if (ud2 == 1 && lr2 == 0)
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_up_2);
		}
		end = 1;
	}
	else if ((player1.life != 0 && player2.life == 0))
	{

		if (lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_die_2);
		}
		else if (lr2 == 0)
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_die_2);
		}
		if (ud1 == 0 && lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_down_1);
		}
		if (ud1 == 1 && lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_up_1);
		}
		if (ud1 == 0 && lr1 == 0)
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_down_1);
		}
		if (ud1 == 1 && lr1 == 0)
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_up_1);
		}
		end = 1;
	}
	else if (player1.life == 0 && player2.life == 0)
	{
		if (lr2 == 1)
		{
			putimagePng(player2.x - 25, player2.y - 50, &right_die_2);
		}
		else if (lr2 == 0)
		{
			putimagePng(player2.x - 25, player2.y - 50, &left_die_2);
		}
		if (lr1 == 1)
		{
			putimagePng(player1.x - 25, player1.y - 50, &right_die_1);
		}
		if (lr1 == 0)
		{
			putimagePng(player1.x - 25, player1.y - 50, &left_die_1);
		}
		end = 1;
	}

	FlushBatchDraw();
}
void updateWithInput()//����������
{
	if (_kbhit())
	{
		if (GetAsyncKeyState(VK_LEFT))
		{
			player2.x = player2.x - v2;
			lr2 = 0;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			player2.x = player2.x + v2;
			lr2 = 1;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			player2.y = player2.y - v2;
			ud2 = 0;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			ud2 = 1;
			player2.y = player2.y + v2;
		}
		if (GetAsyncKeyState('A'))
		{
			lr1 = 0;
			player1.x = player1.x - v1;
		}

		if (GetAsyncKeyState('D'))
		{
			lr1 = 1;
			player1.x = player1.x + v1;
		}

		if (GetAsyncKeyState('W'))
		{
			ud1 = 0;
			player1.y = player1.y - v1;
		}
		if (GetAsyncKeyState('S'))
		{
			ud1 = 1;
			player1.y = player1.y + v1;
		}
	}
}
void update()
{
	if (player1.buff != 0)
	{
		v1 = 0.7;
	}
	else if (player1.buff == 0)
	{
		v1 = 0.45;
	}
	if (player2.buff != 0)
	{
		v2 = 0.7;
	}
	else if (player2.buff == 0)
	{
		v2 = 0.45;
	}
	time_type1--;//��ըЧ������ʱ
	time_type2--;
	gamebegintime--;
	int r = rand() % B_NUMx;//����������������������ש��
	int c = rand() % B_NUMy + 1;
	int i, j;
	if (_kbhit())
	{
		if (GetAsyncKeyState(' ') && player1.boom == 1)//����ո������1�ɷ���ը��
		{

			boom1.x = player1.x_move;//ը��xy����
			boom1.y = player1.y_move;

			boom1.exist = 1;//ը������
			boom1.boomtime = 500;//ը����ʱ
			player1.boom = 0;//���1���ɷ���ը��
		}
	}
	if (_kbhit())
	{
		if (GetAsyncKeyState('\r') && player2.boom == 1)//����ո������2�ɷ���ը��
		{

			boom2.x = player2.x_move;
			boom2.y = player2.y_move;
			boom2.exist = 1;//ը������
			boom2.boomtime = 500;//ը����ʱ
			player2.boom = 0;//���1���ɷ���ը��
		}
	}
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)
		{
			if (time_type1 == 0 && level[i][j].type == 'a')//һֱ�ȵ���ըЧ���������ɫ�ſɷ���һ��ը��
			{
				level[i][j].type = 'e';
				player1.boom = 1;
			}
			if (time_type2 == 0 && level[i][j].type == 'a')
			{
				level[i][j].type = 'e';
				player2.boom = 1;
			}
			if (gamebegintime <= 0 && level[r][c].type == 'e' &&
				gamebegin == 0 &&
				(distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player1.x, player1.y) > 70.7 ||
					distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player2.x, player2.y) > 70.7))//��ֹ���ɵ����ש�����ɫ�������
			{
				level[r][c].type = 'd';
				gamebegin = 1;//�����ڳ�ʼ��ʱ���ɵ���ש�Ľ׶�
				gameaftertime = clock();//����ʱ����Ϊ������Ϸ�����ɵ���ש�ĳ�ʼʱ��
			}
			if ((clock() - gameaftertime > 15000) && level[r][c].type == 'e' &&
				(distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player1.x, player1.y) > 70.7 ||
					distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player2.x, player2.y) > 70.7) &&
				gamebegin == 1)
			{
				level[r][c].type = 'd';
				gameaftertime = clock();//���»�ȡ��һ�����ɵ���ש��ĳ�ʼ���ʱ��
			}

			player1.x_move = level[((int)(player1.y)) / 50][((int)(player1.x)) / 50].x;
			player1.y_move = level[((int)(player1.y)) / 50][((int)(player1.x)) / 50].y;
			if (level[((player1.y_move - 25) / 50)][((player1.x_move - 25) / 50) + 1].exist == 1 && player1.x > player1.x_move)
			{
				player1.x = player1.x - v1;//�������ƶ�
			}
			if (level[((player1.y_move - 25) / 50)][((player1.x_move - 25) / 50) - 1].exist == 1 && player1.x < player1.x_move)
			{
				player1.x = player1.x + v1;//�������ƶ�
			}
			if (level[((player1.y_move - 25) / 50) + 1][((player1.x_move - 25) / 50)].exist == 1 && player1.y > player1.y_move)
			{
				player1.y = player1.y - v1;//�������ƶ�
			}
			if (level[((player1.y_move - 25) / 50) - 1][((player1.x_move - 25) / 50)].exist == 1 && player1.y < player1.y_move)
			{
				player1.y = player1.y + v1;//�������ƶ�
			}

			player2.x_move = level[((int)(player2.y)) / 50][((int)(player2.x)) / 50].x;//ը��xy����
			player2.y_move = level[((int)(player2.y)) / 50][((int)(player2.x)) / 50].y;
			if (level[((player2.y_move - 25) / 50)][((player2.x_move - 25) / 50) + 1].exist == 1 && player2.x > player2.x_move)
			{
				player2.x = player2.x - v2;//�������ƶ�
			}
			if (level[((player2.y_move - 25) / 50)][((player2.x_move - 25) / 50) - 1].exist == 1 && player2.x < player2.x_move)
			{
				player2.x = player2.x + v2;//�������ƶ�
			}
			if (level[((player2.y_move - 25) / 50) + 1][((player2.x_move - 25) / 50)].exist == 1 && player2.y > player2.y_move)
			{
				player2.y = player2.y - v2;//�������ƶ�
			}
			if (level[((player2.y_move - 25) / 50) - 1][((player2.x_move - 25) / 50)].exist == 1 && player2.y < player2.y_move)
			{
				player2.y = player2.y + v2;//�������ƶ�
			}
		}
		if (level[(player1.y_move - 25) / 50][(player1.x_move - 25) / 50].type == 'd')//ʵ�ֽ�ɫ�ߵ�����שʰȡ���ܵ�Ч��
		{
			player1.buff = 3;
			level[(player1.y_move - 25) / 50][(player1.x_move - 25) / 50].type = 'e';
		}
		if (level[(player2.y_move - 25) / 50][(player2.x_move - 25) / 50].type == 'd')
		{
			player2.buff = 3;
			level[(player2.y_move - 25) / 50][(player2.x_move - 25) / 50].type = 'e';
		}
	}

	if (boom1.exist == 1)//���ը������
	{

		boom1.boomtime--;//��ʱ����

		if ((boom1.y - player1.y) * (boom1.y - player1.y) + (boom1.x - player1.x) * (boom1.x - player1.x) >= 50 * 50 - 20)//����뿪ը�����ڵؿ��
		{
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 1;//ը�����ڵؿ鲻��ͨ��
		}
		if (boom1.boomtime == 0 && player1.buff == 0)//�����ʱ������ը����ը��ΧΪһ��
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom1.exist = 0;//ը����ʧ
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 0;//ը�����ڵؿ����¿�ͨ��

			//�ж�ը����ը��Χ�ڸ������ש�����
			if (level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//����һ��Ϊ�յػ���ǽ
			{
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type = 'a';


			}
			if (level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type == 'e')//����һ��
			{
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].canbreak == 1 || level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type == 'e')//�ұ�һ��
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].canbreak == 1 || level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type = 'a';

			}

			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].type = 'a';
			time_type1 = 50;


		}
		else if (boom1.boomtime == 0 && player1.buff != 0)
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom1.exist = 0;//ը����ʧ
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 0;//ը�����ڵؿ����¿�ͨ��
			if (level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type == 'e' && level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//����Χ��Ϊ�յ�
			{
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type = 'a';


			}
			else if (level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type == 'b' && level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//��Χ�յأ���Χ��ǽ
			{
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type = 'a';


			}
			if (level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].type == 'e' && level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type == 'e')
			{
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].type = 'a';

			}
			else if (level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].type == 'b' && level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type == 'e')
			{
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) - 2][((boom1.x - 25) / 50)].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].type == 'e' && level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].type = 'a';

			}
			else if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].type == 'b' && level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 2].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].type == 'e' && level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].type = 'a';

			}
			else if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].type == 'b' && level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 2].type = 'a';

			}

			if (level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')
			{
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type = 'a';


			}
			if (level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type == 'e')
			{
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].canbreak == 1 || level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].canbreak == 1 || level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].canbreak = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].exist = 0;
				level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) - 1].type = 'a';

			}



			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].type = 'a';
			time_type1 = 50;
			player1.buff--;

		}

	}
	if (boom2.exist == 1)//���ը������
	{

		boom2.boomtime--;//��ʱ����

		if ((boom2.y - player2.y) * (boom2.y - player2.y) + (boom2.x - player2.x) * (boom2.x - player2.x) >= 50 * 50 - 20)//����뿪ը�����ڵؿ��
		{
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 1;//ը�����ڵؿ鲻��ͨ��
		}
		if (boom2.boomtime == 0 && player2.buff == 0)//�����ʱ����
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom2.exist = 0;//ը����ʧ
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 0;//ը�����ڵؿ����¿�ͨ��

			if (level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].canbreak == 1 || level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type = 'a';


			}
			if (level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].canbreak == 1 || level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].canbreak == 1 || level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].canbreak == 1 || level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type = 'a';

			}

			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].type = 'a';
			time_type2 = 50;



		}
		else if (boom2.boomtime == 0 && player2.buff != 0)
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom2.exist = 0;//ը����ʧ
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 0;//ը�����ڵؿ����¿�ͨ��
			if (level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].type == 'e' && level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].type = 'a';


			}
			else if (level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].type == 'b' && level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) + 2][((boom2.x - 25) / 50)].type = 'a';


			}
			if (level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].type == 'e' && level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].type = 'a';

			}
			else if (level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].type == 'b' && level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) - 2][((boom2.x - 25) / 50)].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].type == 'e' && level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].type = 'a';

			}
			else if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].type == 'b' && level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 2].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].type == 'e' && level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].type = 'a';

			}
			else if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].type == 'b' && level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 2].type = 'a';

			}

			if (level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].canbreak == 1 || level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) + 1][((boom2.x - 25) / 50)].type = 'a';


			}
			if (level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].canbreak == 1 || level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type == 'e')
			{
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].canbreak = 0;
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].exist = 0;
				level[((boom2.y - 25) / 50) - 1][((boom2.x - 25) / 50)].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].canbreak == 1 || level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) + 1].type = 'a';

			}
			if (level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].canbreak == 1 || level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type == 'e')
			{
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].canbreak = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].exist = 0;
				level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50) - 1].type = 'a';

			}
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].type = 'a';
			time_type2 = 50;
			player2.buff--;

		}
	}
	if (level[(player1.y_move - 25) / 50][(player1.x_move - 25) / 50].type == 'a')
		player1.life = 0;
	if (level[(player2.y_move - 25) / 50][(player2.x_move - 25) / 50].type == 'a')
		player2.life = 0;
}
void game_end()
{
	Sleep(1000);
	while (1)
	{

		if (player1.life == 0 && player2.life != 0)
		{
			putimagePng(0, 0, &win1);
		}
		else if (player2.life == 0 && player1.life != 0)
		{
			putimagePng(0, 0, &win2);
		}
		else if (player1.life == 0 && player2.life == 0)
		{
			putimagePng(0, 0, &draw);
		}
	begin:
		FlushBatchDraw();
		MOUSEMSG m;
		m = GetMouseMsg();

		if (m.x >= 450 && m.x <= 800 && m.y >= 560 && m.y <= 685) //�ж�����Ƿ��Ƶ���ť����
		{
			setlinecolor(RED);
			rectangle(450, 560, 796, 685);

			if (m.uMsg == WM_LBUTTONDOWN)//�ж��������Ƿ���
			{
				closegraph();//�رջ�ͼ����
				exit(0);//���������˳����Ҳ���ִ�к���Ĵ���
			}
			goto begin;
		}
		if (m.x >= 43 && m.x <= 370 && m.y >= 561 && m.y <= 685) //�ж�����Ƿ��Ƶ���ť����
		{
			setlinecolor(RED);
			rectangle(43, 561, 370, 685);

			if (m.uMsg == WM_LBUTTONDOWN)//�ж��������Ƿ���
			{
				break;//�˳���Ϸ�Ĵ�ѭ��
			}
			goto begin;
		}
	}

}
int main()
{
	loadimage();
	while (1)
	{

		startup();//��ʼ��
		while (1)
		{
			show();//����
			update();//����
			updateWithInput();//�ƶ�����
			if (end == 1)
				break;

		}
		game_end();

	}
	return 0;
}