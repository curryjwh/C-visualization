#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define Num 13 //有13种烟花
#define Width 1000 //画面宽度
#define Height 600 //画面高度

void picture();//准备每个烟花的图片
void Initialize(int i);//初始化数据
void choose_shoot(DWORD& t1);//选择并发射烟花弹
void baozha(DWORD* pM);//绘制烟花爆炸
void clear(DWORD* pM);//随机消除像素点，达到逐渐消失效果
void Style(DWORD& t_xin);//20s后播放心形烟花

struct yanhuadan {
	int x, y;//发射坐标
	int h;//发射的最高点
	bool is_shoot;//是否发射
	IMAGE dan;//存放烟花弹图片
	DWORD t1, t2, dt;//控制发射速度
}Dan[Num];

struct Yanhua {
	int rmax;//完整烟花半径
	int x, y;//爆炸点坐标
	int xc, yc;//图片中爆炸中心坐标
	int r;//当前烟花半径
	int width;
	int height;//烟花宽和高
	int fire[240][240];//储存烟花图片像素点
	bool is_draw;//是否画出烟花
	bool is_baozha;//烟花是否爆炸
	DWORD t1, t2, dt;//控制爆炸速度
}yanhua[Num];


//主函数
int main()
{
	initgraph(Width, Height);
	srand((unsigned int)time(NULL));//得到不同的随机数
	DWORD t1 = timeGetTime();       //选择烟花用的时间
	DWORD t_xin = timeGetTime();	//心形烟花计时
	DWORD* pM = GetImageBuffer();	//获取窗口显存指针，对每个像素点进行操作
	mciSendString("open daoshu.mp3 alias music ", 0, 0, 0);//播放背景音乐
	mciSendString("play music repeat", 0, 0, 0);
	for (int i = 0; i < Num; i++)
	{
		Initialize(i);				//初始化每个烟花和烟花弹
	}
	picture();						//准备加载烟花和烟花弹图片
	BeginBatchDraw();				//批量绘制
	while (1)
	{
		choose_shoot(t1);
		baozha(pM);
		Style(t_xin);
		settextcolor(WHITE);
		outtextxy(350, 200, TEXT("身后是昆一，各位必会光芒万丈"));
		clear(pM);
		Sleep(10);
		FlushBatchDraw();
	}
	return 0;
}




//准备图片
void picture()
{
	IMAGE onefireimg, fireimg;
	loadimage(&fireimg, "fireflower.jpg", 240 * 13, 240);//13个烟花，每个240*240
	for (int i = 0; i < Num; i++) {
		SetWorkingImage(&fireimg);//将整个烟花图片设为绘图区
		getimage(&onefireimg, i * 240, 0, 240, 240);//选取一个烟花
		SetWorkingImage(&onefireimg);//将该个烟花图片设为绘图区
		for (int a = 0; a < 240; a++) {
			for (int b = 0; b < 240; b++) {
				yanhua[i].fire[a][b] = getpixel(a, b);//获取每个点的颜色，并储存
			}
		}
	}/*烟花图片(上)*/
	/*烟花弹图片(下)*/
	IMAGE danimg;
	loadimage(&danimg, "yanhuadan.jpg", 100, 50);
	for (int i = 0; i < Num; i++) {
		SetWorkingImage(&danimg);//将烟花弹图片设为绘图区
		int x = rand() % 5;
		getimage(&Dan[i].dan, x * 20, 0, 20, 50);//随机获取一个烟花弹图片
	}
	SetWorkingImage(NULL);//重新将绘图区设为整个画面
}

void Initialize(int i)//初始化烟花弹和烟花数据
{
	/*初始化烟花弹数据*/
	Dan[i].x = rand() % Width;
	Dan[i].y = Height - rand() % 50;
	Dan[i].h = rand() % 300;
	Dan[i].is_shoot = false;
	Dan[i].t1 = GetTickCount();//获得系统时间
	Dan[i].dt = rand() % 5;//速度时间间隔
	/*初始化烟花数据*/
	int x[13] = { 120,120,110,117,110,93,102,102,110,105,100,108,110 };
	int y[13] = { 120,120,85,118,120,103,105,110,110,120,120,104,85 };
	/*在每个烟花图片中爆炸点距离左上角坐标*/
	yanhua[i].rmax = 130;
	yanhua[i].r = 0;
	yanhua[i].x = Dan[i].x;
	yanhua[i].y = Dan[i].h;
	yanhua[i].xc = x[i];
	yanhua[i].yc = y[i];
	yanhua[i].width = 240;//每个烟花的宽高
	yanhua[i].height = 240;
	yanhua[i].is_draw = false;
	yanhua[i].is_baozha = false;
	yanhua[i].t1 = timeGetTime();
	yanhua[i].dt = 5;
}
void choose_shoot(DWORD& t1)
{
	DWORD t2 = timeGetTime();//获得系统时间
	if (t2 - t1 > 150) {	 //每隔150ms发射一个烟花弹
		int n = rand() % 13;
		if (n<13 && Dan[n].is_shoot == false && yanhua[n].is_baozha == false)
		{
			Dan[n].is_shoot = true;
			Dan[n].x = rand() % Width;
			Dan[n].y = Height - rand() % 50;
			Dan[n].h = rand() % 300;
			putimage(Dan[n].x, Dan[n].y, &Dan[n].dan, SRCINVERT);
			char open[60], play[40], close[40];
			/*播放发射的音乐*/
			sprintf_s(open, "open shoot.mp3 alias d%d", n);
			sprintf_s(play, "play d%d", n);
			sprintf_s(close, "close f%d", n);
			mciSendString(close, 0, 0, 0);//关闭爆炸音乐
			mciSendString(open, 0, 0, 0); //  播放发
			mciSendString(play, 0, 0, 0); //  射音乐
		}
		t1 = t2;//确保下次发射时间间隔最少未为150ms
	}
	for (int i = 0; i < Num; i++)//烟花弹升空
	{
		Dan[i].t2 = timeGetTime();
		if (Dan[i].t2 - Dan[i].t1 >= Dan[i].dt && Dan[i].is_shoot == true) 
		{
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			//SRCINVERT作用是使图片透明
			if (Dan[i].y >= Dan[i].h)
			{
				Dan[i].y -= 5;
			}
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			if (Dan[i].y <= Dan[i].h) {
				/*播放烟花爆炸的音乐*/
				char o1[50], p1[30], c1[30];
				sprintf_s(o1, "open bomb.mp3 alias f%d", i);
				sprintf_s(p1, "play f%d", i);
				sprintf_s(c1, "close d%d", i);

				mciSendString(c1, 0, 0, 0);//关闭发射音乐
				mciSendString(o1, 0, 0, 0); //  播放烟花
				mciSendString(p1, 0, 0, 0); //  爆炸音乐
				putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);//停止发射
				yanhua[i].x = Dan[i].x + 10;
				yanhua[i].y = Dan[i].h;	//初始烟花xy坐标
				yanhua[i].is_baozha = true;
				Dan[i].is_shoot = false;
			}
		Dan[i].t1 = Dan[i].t2;
		//产生一个烟花弹就发射一个
		}
	}
}


void baozha(DWORD* pM)
{
	int ddt[14] = { 5,5,5,5,25,25,25,25,25,55,55,55,55,55 };
	/*dt不同的取值，达到烟花变速绽放效果*/
	for (int i = 0; i < Num; i++)
	{
		yanhua[i].t2 = timeGetTime();
		if (yanhua[i].t2 - yanhua[i].t1 >= yanhua[i].dt && yanhua[i].is_baozha == true)
		{
			if (yanhua[i].r < yanhua[i].rmax)
			{
				yanhua[i].r++;
				yanhua[i].dt = ddt[yanhua[i].r / 10];
				yanhua[i].is_draw = true;
			}
			if (yanhua[i].r >= yanhua[i].rmax)
			{
				yanhua[i].is_draw = false;
				Initialize(i);//重新初始化该烟花
			}
			yanhua[i].t1 = yanhua[i].t2;
			/*画烟花，根据半径输出像素点*/
			if (yanhua[i].is_draw = true)
			{
				for (double o = 0; o <= 6.28; o += 0.01)
				{
					int xx1 = (int)(yanhua[i].xc + yanhua[i].r * cos(o));
					int yy1 = (int)(yanhua[i].yc - yanhua[i].r * sin(o));
					/*像素点在烟花图片里的坐标*/
					int xx2 = (int)(yanhua[i].x + yanhua[i].r * cos(o));
					int yy2 = (int)(yanhua[i].y - yanhua[i].r * sin(o));
					/*像素点在整个窗口的坐标*/
					if (xx1 > 0 && xx1 < yanhua[i].width && yy1>0 && yy1 < yanhua[i].height)
					{//确保只输出图片里的像素点
						int b = yanhua[i].fire[xx1][yy1] & 0xff;		//blue颜色的16进制
						int g = (yanhua[i].fire[xx1][yy1] >> 8) & 0xff;	//green颜色的16进制
						int r = (yanhua[i].fire[xx1][yy1] >> 16);		//red颜色的16进制
						if (r > 0x20 && g > 0x20 && r > 0x20)
						{//剔除较暗的像素点
							if (xx2 > 0 && xx2 < Width && yy2>0 && yy2 < Height)
							{//确定范围，防止越界
								pM[yy2 * Width + xx2] = BGR(yanhua[i].fire[xx1][yy1]);
								/*显存操作，输出对应像素点的颜色*/
							}
						}
					}
				}
				yanhua[i].is_draw = false;
			}
		}
	}
}

void Style(DWORD& t_xin)
{

	DWORD st2 = timeGetTime();//20s后播放心形烟花
	if (st2 - t_xin > 20000)
	{
		
		// 心形坐标
		double x[13] = { 45.4,45.7,34.0, 56.7,25.9,65.5,34.6,56.5,40.9,50.6,38.4,52.8, 64.8 };
		double y[13] = { 12.5, 35.9,5.0,5.0,13.8,13.8,28.4,28.4, 6.6,6.6,31.6,31.6,10.7 };
		for (int i = 0; i < Num; i++)
		{
			/*烟花弹 */
			Dan[i].x = (int)(x[i] * 10);
			Dan[i].y = (int)((y[i] + 75) * 10);
			Dan[i].h = (int)(y[i] * 10);
			Dan[i].is_shoot = true;
			Dan[i].dt = 7;
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			/* 烟花*/
			yanhua[i].x = (int)(Dan[i].x + 10);
			yanhua[i].y = (int)(Dan[i].h);
			yanhua[i].is_baozha = false;
			yanhua[i].r = 0;

			/*播放发射声音*/
			char o1[50], p2[30], c3[30];
			sprintf_s(o1, "open shoot.mp3 alias d%d", i);
			sprintf_s(p2, "play d%d", i);
			sprintf_s(c3, "close f%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(o1, 0, 0, 0);
			mciSendString(p2, 0, 0, 0);
		}
		t_xin = st2;
	}
}
void clear(DWORD* pM)
{/*目标：随机消除4000个像素点（1000*2*2）*/
	/*消除方法：改成黑色，即背景色*/
	for (int i = 1; i <= 1000; i++)
	{
		for (int t = 1; t <= 2; t++)
		{
			int clx = rand() % Width;
			int cly = rand() % Height;
			pM[cly * Width + clx] =pM[cly * Width + clx + 1]= BLACK;//将两点的颜色
		}
	}
	
}