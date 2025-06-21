#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define Num 13 //��13���̻�
#define Width 1000 //������
#define Height 600 //����߶�

void picture();//׼��ÿ���̻���ͼƬ
void Initialize(int i);//��ʼ������
void choose_shoot(DWORD& t1);//ѡ�񲢷����̻���
void baozha(DWORD* pM);//�����̻���ը
void clear(DWORD* pM);//����������ص㣬�ﵽ����ʧЧ��
void Style(DWORD& t_xin);//20s�󲥷������̻�

struct yanhuadan {
	int x, y;//��������
	int h;//�������ߵ�
	bool is_shoot;//�Ƿ���
	IMAGE dan;//����̻���ͼƬ
	DWORD t1, t2, dt;//���Ʒ����ٶ�
}Dan[Num];

struct Yanhua {
	int rmax;//�����̻��뾶
	int x, y;//��ը������
	int xc, yc;//ͼƬ�б�ը��������
	int r;//��ǰ�̻��뾶
	int width;
	int height;//�̻���͸�
	int fire[240][240];//�����̻�ͼƬ���ص�
	bool is_draw;//�Ƿ񻭳��̻�
	bool is_baozha;//�̻��Ƿ�ը
	DWORD t1, t2, dt;//���Ʊ�ը�ٶ�
}yanhua[Num];


//������
int main()
{
	initgraph(Width, Height);
	srand((unsigned int)time(NULL));//�õ���ͬ�������
	DWORD t1 = timeGetTime();       //ѡ���̻��õ�ʱ��
	DWORD t_xin = timeGetTime();	//�����̻���ʱ
	DWORD* pM = GetImageBuffer();	//��ȡ�����Դ�ָ�룬��ÿ�����ص���в���
	mciSendString("open daoshu.mp3 alias music ", 0, 0, 0);//���ű�������
	mciSendString("play music repeat", 0, 0, 0);
	for (int i = 0; i < Num; i++)
	{
		Initialize(i);				//��ʼ��ÿ���̻����̻���
	}
	picture();						//׼�������̻����̻���ͼƬ
	BeginBatchDraw();				//��������
	while (1)
	{
		choose_shoot(t1);
		baozha(pM);
		Style(t_xin);
		settextcolor(WHITE);
		outtextxy(350, 200, TEXT("�������һ����λ�ػ��â����"));
		clear(pM);
		Sleep(10);
		FlushBatchDraw();
	}
	return 0;
}




//׼��ͼƬ
void picture()
{
	IMAGE onefireimg, fireimg;
	loadimage(&fireimg, "fireflower.jpg", 240 * 13, 240);//13���̻���ÿ��240*240
	for (int i = 0; i < Num; i++) {
		SetWorkingImage(&fireimg);//�������̻�ͼƬ��Ϊ��ͼ��
		getimage(&onefireimg, i * 240, 0, 240, 240);//ѡȡһ���̻�
		SetWorkingImage(&onefireimg);//���ø��̻�ͼƬ��Ϊ��ͼ��
		for (int a = 0; a < 240; a++) {
			for (int b = 0; b < 240; b++) {
				yanhua[i].fire[a][b] = getpixel(a, b);//��ȡÿ�������ɫ��������
			}
		}
	}/*�̻�ͼƬ(��)*/
	/*�̻���ͼƬ(��)*/
	IMAGE danimg;
	loadimage(&danimg, "yanhuadan.jpg", 100, 50);
	for (int i = 0; i < Num; i++) {
		SetWorkingImage(&danimg);//���̻���ͼƬ��Ϊ��ͼ��
		int x = rand() % 5;
		getimage(&Dan[i].dan, x * 20, 0, 20, 50);//�����ȡһ���̻���ͼƬ
	}
	SetWorkingImage(NULL);//���½���ͼ����Ϊ��������
}

void Initialize(int i)//��ʼ���̻������̻�����
{
	/*��ʼ���̻�������*/
	Dan[i].x = rand() % Width;
	Dan[i].y = Height - rand() % 50;
	Dan[i].h = rand() % 300;
	Dan[i].is_shoot = false;
	Dan[i].t1 = GetTickCount();//���ϵͳʱ��
	Dan[i].dt = rand() % 5;//�ٶ�ʱ����
	/*��ʼ���̻�����*/
	int x[13] = { 120,120,110,117,110,93,102,102,110,105,100,108,110 };
	int y[13] = { 120,120,85,118,120,103,105,110,110,120,120,104,85 };
	/*��ÿ���̻�ͼƬ�б�ը��������Ͻ�����*/
	yanhua[i].rmax = 130;
	yanhua[i].r = 0;
	yanhua[i].x = Dan[i].x;
	yanhua[i].y = Dan[i].h;
	yanhua[i].xc = x[i];
	yanhua[i].yc = y[i];
	yanhua[i].width = 240;//ÿ���̻��Ŀ��
	yanhua[i].height = 240;
	yanhua[i].is_draw = false;
	yanhua[i].is_baozha = false;
	yanhua[i].t1 = timeGetTime();
	yanhua[i].dt = 5;
}
void choose_shoot(DWORD& t1)
{
	DWORD t2 = timeGetTime();//���ϵͳʱ��
	if (t2 - t1 > 150) {	 //ÿ��150ms����һ���̻���
		int n = rand() % 13;
		if (n<13 && Dan[n].is_shoot == false && yanhua[n].is_baozha == false)
		{
			Dan[n].is_shoot = true;
			Dan[n].x = rand() % Width;
			Dan[n].y = Height - rand() % 50;
			Dan[n].h = rand() % 300;
			putimage(Dan[n].x, Dan[n].y, &Dan[n].dan, SRCINVERT);
			char open[60], play[40], close[40];
			/*���ŷ��������*/
			sprintf_s(open, "open shoot.mp3 alias d%d", n);
			sprintf_s(play, "play d%d", n);
			sprintf_s(close, "close f%d", n);
			mciSendString(close, 0, 0, 0);//�رձ�ը����
			mciSendString(open, 0, 0, 0); //  ���ŷ�
			mciSendString(play, 0, 0, 0); //  ������
		}
		t1 = t2;//ȷ���´η���ʱ��������δΪ150ms
	}
	for (int i = 0; i < Num; i++)//�̻�������
	{
		Dan[i].t2 = timeGetTime();
		if (Dan[i].t2 - Dan[i].t1 >= Dan[i].dt && Dan[i].is_shoot == true) 
		{
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			//SRCINVERT������ʹͼƬ͸��
			if (Dan[i].y >= Dan[i].h)
			{
				Dan[i].y -= 5;
			}
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			if (Dan[i].y <= Dan[i].h) {
				/*�����̻���ը������*/
				char o1[50], p1[30], c1[30];
				sprintf_s(o1, "open bomb.mp3 alias f%d", i);
				sprintf_s(p1, "play f%d", i);
				sprintf_s(c1, "close d%d", i);

				mciSendString(c1, 0, 0, 0);//�رշ�������
				mciSendString(o1, 0, 0, 0); //  �����̻�
				mciSendString(p1, 0, 0, 0); //  ��ը����
				putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);//ֹͣ����
				yanhua[i].x = Dan[i].x + 10;
				yanhua[i].y = Dan[i].h;	//��ʼ�̻�xy����
				yanhua[i].is_baozha = true;
				Dan[i].is_shoot = false;
			}
		Dan[i].t1 = Dan[i].t2;
		//����һ���̻����ͷ���һ��
		}
	}
}


void baozha(DWORD* pM)
{
	int ddt[14] = { 5,5,5,5,25,25,25,25,25,55,55,55,55,55 };
	/*dt��ͬ��ȡֵ���ﵽ�̻���������Ч��*/
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
				Initialize(i);//���³�ʼ�����̻�
			}
			yanhua[i].t1 = yanhua[i].t2;
			/*���̻������ݰ뾶������ص�*/
			if (yanhua[i].is_draw = true)
			{
				for (double o = 0; o <= 6.28; o += 0.01)
				{
					int xx1 = (int)(yanhua[i].xc + yanhua[i].r * cos(o));
					int yy1 = (int)(yanhua[i].yc - yanhua[i].r * sin(o));
					/*���ص����̻�ͼƬ�������*/
					int xx2 = (int)(yanhua[i].x + yanhua[i].r * cos(o));
					int yy2 = (int)(yanhua[i].y - yanhua[i].r * sin(o));
					/*���ص����������ڵ�����*/
					if (xx1 > 0 && xx1 < yanhua[i].width && yy1>0 && yy1 < yanhua[i].height)
					{//ȷ��ֻ���ͼƬ������ص�
						int b = yanhua[i].fire[xx1][yy1] & 0xff;		//blue��ɫ��16����
						int g = (yanhua[i].fire[xx1][yy1] >> 8) & 0xff;	//green��ɫ��16����
						int r = (yanhua[i].fire[xx1][yy1] >> 16);		//red��ɫ��16����
						if (r > 0x20 && g > 0x20 && r > 0x20)
						{//�޳��ϰ������ص�
							if (xx2 > 0 && xx2 < Width && yy2>0 && yy2 < Height)
							{//ȷ����Χ����ֹԽ��
								pM[yy2 * Width + xx2] = BGR(yanhua[i].fire[xx1][yy1]);
								/*�Դ�����������Ӧ���ص����ɫ*/
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

	DWORD st2 = timeGetTime();//20s�󲥷������̻�
	if (st2 - t_xin > 20000)
	{
		
		// ��������
		double x[13] = { 45.4,45.7,34.0, 56.7,25.9,65.5,34.6,56.5,40.9,50.6,38.4,52.8, 64.8 };
		double y[13] = { 12.5, 35.9,5.0,5.0,13.8,13.8,28.4,28.4, 6.6,6.6,31.6,31.6,10.7 };
		for (int i = 0; i < Num; i++)
		{
			/*�̻��� */
			Dan[i].x = (int)(x[i] * 10);
			Dan[i].y = (int)((y[i] + 75) * 10);
			Dan[i].h = (int)(y[i] * 10);
			Dan[i].is_shoot = true;
			Dan[i].dt = 7;
			putimage(Dan[i].x, Dan[i].y, &Dan[i].dan, SRCINVERT);
			/* �̻�*/
			yanhua[i].x = (int)(Dan[i].x + 10);
			yanhua[i].y = (int)(Dan[i].h);
			yanhua[i].is_baozha = false;
			yanhua[i].r = 0;

			/*���ŷ�������*/
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
{/*Ŀ�꣺�������4000�����ص㣨1000*2*2��*/
	/*�����������ĳɺ�ɫ��������ɫ*/
	for (int i = 1; i <= 1000; i++)
	{
		for (int t = 1; t <= 2; t++)
		{
			int clx = rand() % Width;
			int cly = rand() % Height;
			pM[cly * Width + clx] =pM[cly * Width + clx + 1]= BLACK;//���������ɫ
		}
	}
	
}