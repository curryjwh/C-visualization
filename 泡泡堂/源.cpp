#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<easyx.h>
#include <stdlib.h>
#include<math.h>
#include<time.h>
#include"EasyXPng.h"
#include <mmsystem.h>
#define B_SIZE 50//格子大小
#define B_NUMx 17//宽度
#define B_NUMy 15//高度
#pragma comment(lib,"Winmm.lib")
float distance(float x1, float y1, float x2, float y2)//定义一个距离函数用于后续坐标判断
{
	float distance;
	distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return distance;
}
//全局变量定义
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
								  "wwwwwwwwwwwwwwwww" };//地图数据1

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
								  "wwwwwwwwwwwwwwwww" };//地图数据2
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
								  "wwwwwwwwwwwwwwwww" };//地图数据3
int playerradius = 25;//玩家半径
struct player
{
	float x;//玩家x坐标
	float y;//玩家y坐标
	int life;//玩家是否存活
	int boom;//玩家是否能放置炸弹
	int x_move;//判断碰撞用x坐标
	int y_move;//判断碰撞用y坐标
	int buff;//玩家吃完道具砖增加炸弹范围
};
struct maps
{
	char type;//地块类型
	int canbreak;//地块是否能被破坏
	int exist;//地块是否挡路
	float x;//地块中心x坐标
	float y;//地块中心y坐标
	int x_left;//地块左边x坐标
	int x_right;//地块右边x坐标
	int y_up;//地块上边y坐标
	int y_down;//地块下边y坐标
};
struct boom
{
	int x;//炸弹x坐标
	int y;//炸弹y坐标
	int exist;//炸弹是否存在
	int boomtime;//炸弹爆炸计时
};
int time_type1, time_type2;//爆炸特效计时
int gamebegin;//是否为刚进入游戏的第一个道具
int gamebegintime;//第一个道具计时
int gameaftertime;//
boom boom1, boom2;
maps level[B_NUMy][B_NUMx + 1];//定义地图
float v1 = 0.45;
float v2 = 0.45;//人物移动速度
player player1, player2;//玩家结构体
//e:empty w:wall b:box p:player a:ash l:light
IMAGE menu_start, boom_png,//定义图片变量用于贴图
left_up_1, left_down_1, right_up_1, right_down_1, left_die_1, right_die_1,
left_up_2, left_down_2, right_up_2, right_down_2, left_die_2, right_die_2,
floor_b, floor_E, floor_w, floor_e, floor_d, light, win1, win2, draw;
int lr1, lr2, ud1, ud2;
int end = 0;
int map = 0;
void loadimage()//加载图片资源
{
	loadimage(&menu_start, _T("启动.png"));
	loadimage(&floor_w, _T("硬墙.png"));
	loadimage(&floor_b, _T("软墙.png"));
	loadimage(&light, _T("爆炸.png"));
	loadimage(&floor_E, _T("空地.png"));
	loadimage(&floor_e, _T("空地_无阴影.png"));
	loadimage(&floor_d, _T("道具墙.png"));
	loadimage(&left_up_1, _T("正左2.png"));
	loadimage(&left_down_1, _T("背左2.png"));
	loadimage(&right_up_1, _T("正右2.png"));
	loadimage(&right_down_1, _T("背右2.png"));
	loadimage(&left_die_1, _T("死左2.png"));
	loadimage(&right_die_1, _T("死右2.png"));
	loadimage(&left_up_2, _T("正左.png"));
	loadimage(&left_down_2, _T("背左.png"));
	loadimage(&right_up_2, _T("正右.png"));
	loadimage(&right_down_2, _T("背右.png"));
	loadimage(&left_die_2, _T("死左.png"));
	loadimage(&right_die_2, _T("死右.png"));
	loadimage(&boom_png, _T("雷.png"));
	loadimage(&win1, _T("水月输.png"));
	loadimage(&win2, _T("澄闪输.png"));
	loadimage(&draw, _T("平局.png"));


}
void startup()
{
	if (end == 0)
	{
		mciSendString(_T("open game.mp3 alias bkmusic"), NULL, 0, NULL);
		mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
		initgraph(B_NUMx * B_SIZE, B_NUMy * B_SIZE);//新开一个画面
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

		if (m.x >= 297 && m.x <= 544 && m.y >= 381 && m.y <= 478) //判断鼠标是否移到按钮区域
		{
			setlinecolor(RED);
			rectangle(297, 381, 544, 478);

			if (m.uMsg == WM_LBUTTONDOWN)//判断鼠标左键是否按下
			{
				break;
			}
			goto begin;
		}
	}
	gamebegintime = 5000;
	gamebegin = 0;
	int i, j;//循环用变量
	srand(time(0));
	BeginBatchDraw();//开始批量绘图
	player1 = { B_SIZE + B_SIZE / 2,B_SIZE + B_SIZE / 2,1,1,0 };//初始化玩家信息
	player2 = { (B_NUMx - 1) * B_SIZE - B_SIZE / 2,(B_NUMy - 1) * B_SIZE - B_SIZE / 2,1,1,0 };//初始化玩家信息
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)//遍历所有地块
		{
			if (map == 1)
			{
				level[i][j].type = level1[i][j];//为地块赋类型值
			}
			else if (map == 2)
			{
				level[i][j].type = level2[i][j];//为地块赋类型值
			}
			else if (map == 3)
			{
				level[i][j].type = level3[i][j];//为地块赋类型值
			}
			if (level[i][j].type == 'b')
				level[i][j].canbreak = 1;//根据类型为地块赋是否能破坏
			else
				level[i][j].canbreak = 0;//根据类型为地块赋是否能破坏
			if (level[i][j].type == 'b' || level[i][j].type == 'w' || level[i][j].type == 'a')
				level[i][j].exist = 1;//根据类型判断是否挡路
			else
				level[i][j].exist = 0;//根据类型判断是否挡路
			level[i][j].x = j * B_SIZE + B_SIZE / 2;//为地块各个坐标赋值
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
	int i, j;//循环用变量
	cleardevice();//以背景颜色清空屏幕
	//遍历地图数据
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)
		{
			if (level[i][j].type == 'e')//空白区域
			{
				if (level[i][j - 1].type == 'w' || level[i][j - 1].type == 'b')
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_E);//绘制有阴影地面
				}
				else
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_e);//绘制无阴影地面
				}

			}
			else if (level[i][j].type == 'w')//墙体
			{

				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_w);//绘制墙体
			}
			else if (level[i][j].type == 'b')//软墙
			{


				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_b);//绘制软墙

			}
			//先覆盖一层地面贴图，再覆盖爆炸特效贴图
			else if (level[i][j].type == 'a')//爆炸特效
			{
				if (level[i][j - 1].type == 'w' || level[i][j - 1].type == 'b')
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_E);//绘制地面
				}
				else
				{

					putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_e);//绘制地面
				}

				putimagePng(j * B_SIZE, i * B_SIZE, &light);//爆炸特效
			}
			else if (level[i][j].type == 'd')
				putimagePng(j * B_SIZE, i * B_SIZE - 5, &floor_d);
		}
	}



	if (boom1.exist == 1)//如果炸弹存在
	{
		putimagePng(boom1.x - 25, boom1.y - 25, &boom_png);//初始贴图位置为图片左上角坐标
	}
	//初始时左上为0，右下为1
	if (boom2.exist == 1)//如果炸弹存在
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
		if (ud1 == 0 && lr1 == 0)//初始时的角色图
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
		if (ud2 == 0 && lr2 == 0)//初始时的角色图
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
void updateWithInput()//按键检测更新
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
	time_type1--;//爆炸效果的延时
	time_type2--;
	gamebegintime--;
	int r = rand() % B_NUMx;//随机函数用来生成随机道具砖块
	int c = rand() % B_NUMy + 1;
	int i, j;
	if (_kbhit())
	{
		if (GetAsyncKeyState(' ') && player1.boom == 1)//如果空格且玩家1可放置炸弹
		{

			boom1.x = player1.x_move;//炸弹xy坐标
			boom1.y = player1.y_move;

			boom1.exist = 1;//炸弹存在
			boom1.boomtime = 500;//炸弹计时
			player1.boom = 0;//玩家1不可放置炸弹
		}
	}
	if (_kbhit())
	{
		if (GetAsyncKeyState('\r') && player2.boom == 1)//如果空格且玩家2可放置炸弹
		{

			boom2.x = player2.x_move;
			boom2.y = player2.y_move;
			boom2.exist = 1;//炸弹存在
			boom2.boomtime = 500;//炸弹计时
			player2.boom = 0;//玩家1不可放置炸弹
		}
	}
	for (i = 0; i < B_NUMy; i++)
	{
		for (j = 0; j < B_NUMx; j++)
		{
			if (time_type1 == 0 && level[i][j].type == 'a')//一直等到爆炸效果结束后角色才可放下一次炸弹
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
					distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player2.x, player2.y) > 70.7))//防止生成的随机砖块离角色距离过近
			{
				level[r][c].type = 'd';
				gamebegin = 1;//跳出在初始化时生成道具砖的阶段
				gameaftertime = clock();//将此时间设为进入游戏后生成道具砖的初始时间
			}
			if ((clock() - gameaftertime > 15000) && level[r][c].type == 'e' &&
				(distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player1.x, player1.y) > 70.7 ||
					distance((c + 0.5) * B_SIZE, (r + 0.5) * B_SIZE, player2.x, player2.y) > 70.7) &&
				gamebegin == 1)
			{
				level[r][c].type = 'd';
				gameaftertime = clock();//重新获取下一次生成道具砖块的初始间隔时间
			}

			player1.x_move = level[((int)(player1.y)) / 50][((int)(player1.x)) / 50].x;
			player1.y_move = level[((int)(player1.y)) / 50][((int)(player1.x)) / 50].y;
			if (level[((player1.y_move - 25) / 50)][((player1.x_move - 25) / 50) + 1].exist == 1 && player1.x > player1.x_move)
			{
				player1.x = player1.x - v1;//撤销右移动
			}
			if (level[((player1.y_move - 25) / 50)][((player1.x_move - 25) / 50) - 1].exist == 1 && player1.x < player1.x_move)
			{
				player1.x = player1.x + v1;//撤销右移动
			}
			if (level[((player1.y_move - 25) / 50) + 1][((player1.x_move - 25) / 50)].exist == 1 && player1.y > player1.y_move)
			{
				player1.y = player1.y - v1;//撤销右移动
			}
			if (level[((player1.y_move - 25) / 50) - 1][((player1.x_move - 25) / 50)].exist == 1 && player1.y < player1.y_move)
			{
				player1.y = player1.y + v1;//撤销右移动
			}

			player2.x_move = level[((int)(player2.y)) / 50][((int)(player2.x)) / 50].x;//炸弹xy坐标
			player2.y_move = level[((int)(player2.y)) / 50][((int)(player2.x)) / 50].y;
			if (level[((player2.y_move - 25) / 50)][((player2.x_move - 25) / 50) + 1].exist == 1 && player2.x > player2.x_move)
			{
				player2.x = player2.x - v2;//撤销右移动
			}
			if (level[((player2.y_move - 25) / 50)][((player2.x_move - 25) / 50) - 1].exist == 1 && player2.x < player2.x_move)
			{
				player2.x = player2.x + v2;//撤销右移动
			}
			if (level[((player2.y_move - 25) / 50) + 1][((player2.x_move - 25) / 50)].exist == 1 && player2.y > player2.y_move)
			{
				player2.y = player2.y - v2;//撤销右移动
			}
			if (level[((player2.y_move - 25) / 50) - 1][((player2.x_move - 25) / 50)].exist == 1 && player2.y < player2.y_move)
			{
				player2.y = player2.y + v2;//撤销右移动
			}
		}
		if (level[(player1.y_move - 25) / 50][(player1.x_move - 25) / 50].type == 'd')//实现角色走到道具砖拾取技能的效果
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

	if (boom1.exist == 1)//如果炸弹存在
	{

		boom1.boomtime--;//计时减少

		if ((boom1.y - player1.y) * (boom1.y - player1.y) + (boom1.x - player1.x) * (boom1.x - player1.x) >= 50 * 50 - 20)//玩家离开炸弹所在地块后
		{
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 1;//炸弹所在地块不可通行
		}
		if (boom1.boomtime == 0 && player1.buff == 0)//如果计时归零且炸弹爆炸范围为一格
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom1.exist = 0;//炸弹消失
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 0;//炸弹所在地块重新可通行

			//判断炸弹爆炸范围内各方向的砖块情况
			if (level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//下面一块为空地或软墙
			{
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type = 'a';


			}
			if (level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak == 1 || level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type == 'e')//上面一块
			{
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) - 1][((boom1.x - 25) / 50)].type = 'a';

			}
			if (level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].canbreak == 1 || level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50) + 1].type == 'e')//右边一块
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
			boom1.exist = 0;//炸弹消失
			level[((boom1.y - 25) / 50)][((boom1.x - 25) / 50)].exist = 0;//炸弹所在地块重新可通行
			if (level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type == 'e' && level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//内外围都为空地
			{
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].canbreak = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].exist = 0;
				level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type = 'a';


			}
			else if (level[((boom1.y - 25) / 50) + 2][((boom1.x - 25) / 50)].type == 'b' && level[((boom1.y - 25) / 50) + 1][((boom1.x - 25) / 50)].type == 'e')//内围空地，外围软墙
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
	if (boom2.exist == 1)//如果炸弹存在
	{

		boom2.boomtime--;//计时减少

		if ((boom2.y - player2.y) * (boom2.y - player2.y) + (boom2.x - player2.x) * (boom2.x - player2.x) >= 50 * 50 - 20)//玩家离开炸弹所在地块后
		{
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 1;//炸弹所在地块不可通行
		}
		if (boom2.boomtime == 0 && player2.buff == 0)//如果计时归零
		{
			PlaySound(_T("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
			boom2.exist = 0;//炸弹消失
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 0;//炸弹所在地块重新可通行

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
			boom2.exist = 0;//炸弹消失
			level[((boom2.y - 25) / 50)][((boom2.x - 25) / 50)].exist = 0;//炸弹所在地块重新可通行
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

		if (m.x >= 450 && m.x <= 800 && m.y >= 560 && m.y <= 685) //判断鼠标是否移到按钮区域
		{
			setlinecolor(RED);
			rectangle(450, 560, 796, 685);

			if (m.uMsg == WM_LBUTTONDOWN)//判断鼠标左键是否按下
			{
				closegraph();//关闭绘图窗口
				exit(0);//程序立即退出，且不再执行后面的代码
			}
			goto begin;
		}
		if (m.x >= 43 && m.x <= 370 && m.y >= 561 && m.y <= 685) //判断鼠标是否移到按钮区域
		{
			setlinecolor(RED);
			rectangle(43, 561, 370, 685);

			if (m.uMsg == WM_LBUTTONDOWN)//判断鼠标左键是否按下
			{
				break;//退出游戏的大循环
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

		startup();//初始化
		while (1)
		{
			show();//绘制
			update();//更新
			updateWithInput();//移动更新
			if (end == 1)
				break;

		}
		game_end();

	}
	return 0;
}