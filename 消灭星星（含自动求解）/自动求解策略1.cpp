#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// �����������
#define N 10 
// ������ɫ��������
#define COLORNUM 6
const int tileSize = 50; //С���������εı߳�
COLORREF  colors[COLORNUM]; // �洢����������ɫ������
int grid[N][N]; // ��ά���飬��¼��Ϸ����������С�������ɫ���
int score; // �÷֣�Ҳ�����������ķ�����Ŀ
bool isAutoSolve = false; // ����Ƿ��Զ����
int autosolvenum = 0; // ��¼�Զ����𷽿��������

// FloodFill�����㷨����������(ic,jc)�����������ĸ�����Ѱ��
// clicedValueΪ��ǰ������������ɫ���ֵ
// searchedGrid[N][N]��¼ĳһ�����Ƿ񱻼�������0û�У�1����ͬɫ��-1������ɫ
// matchNumΪͬɫ��ͨ����ĸ���
void floodfill(int ic, int jc, int clicedValue, int searchedGrid[N][N], int* matchNum)
{
    if (grid[ic][jc] == clicedValue) // �뱻���������ɫ��ͬ
    {
        searchedGrid[ic][jc] = 1;  // ��ǰ�������Ѽ�������ͬɫ
        (*matchNum)++; // ͬɫ��ͨ����ĸ�����1
    }
    else // ����������ɫ��ͬ
    {
        searchedGrid[ic][jc] = -1; // ��ǰ�������Ѽ���������ɫ
        return; //  ����������ȥ��
    }

    // ���ĳһ�����λ��û�г�����Χ����û�б��ҹ��������������ݹ�Ѱ��
    if (ic - 1 >= 0 && searchedGrid[ic - 1][jc] == 0)
        floodfill(ic - 1, jc, clicedValue, searchedGrid, matchNum); // ��

    if (ic + 1 < N && searchedGrid[ic + 1][jc] == 0)
        floodfill(ic + 1, jc, clicedValue, searchedGrid, matchNum); // ��

    if (jc - 1 >= 0 && searchedGrid[ic][jc - 1] == 0)
        floodfill(ic, jc - 1, clicedValue, searchedGrid, matchNum); // ��

    if (jc + 1 < N && searchedGrid[ic][jc + 1] == 0)
        floodfill(ic, jc + 1, clicedValue, searchedGrid, matchNum); // ��
}

// �������䣬����Ϊ0�ĺ�ɫ����
void dropBlocks()
{
    // �ú�ɫ��������ķ�������
    for (int j = 0; j < N; j++) // ���б���
    {
        int tempColumm[N] = { 0 }; // �½�һ����ʱ�б������洢ȥ��0֮���Ԫ��
        int tcID = N - 1; // ���ڿ�����tempColummԪ�����

        for (int i = N - 1; i >= 0; i--) // ���б���
        {
            tempColumm[tcID] = grid[i][j]; // �Ȱ���һ���ݸ�����ʱ����
            // �����ǰԪ��Ϊ0��tcID�������´�ѭ���Ḳ�ǵ�0ֵԪ��
            // �����ǰԪ�ز�Ϊ0�������tcID����һ�临�Ƶ�Ԫ����Ч��������ʱ��������		
            if (grid[i][j] != 0)
                tcID--;
        }

        for (int i = 0; i < N; i++) // ����ʱ�����ֵ���ظ���ά�������һ��
            grid[i][j] = tempColumm[i];
    }
}

// ĳһ�з���ȫ�����ˣ����ұߵķ������������ƶ�
void moveBlocksLeft()
{
    // �����һ�п��ˣ��ұߵ��������ƶ�
    for (int j = N - 1; j > 0; j--) // ���б����������������
    {
        int isColumEmpty = 1; // ��һ���Ƿ��ǿհ׺ڿ飬1�ǣ�0��
        for (int i = 0; i < N; i++) // ���б���
        {
            if (grid[i][j] != 0)
            {
                isColumEmpty = 0;
                break; // ��һ����һ�����Ǻڿ飬������ѭ��
            }
        }

        // ��һ�ж��Ǻ�ɫ����ģ�����һ�п�ʼ���ң�ÿһ���ƶ������һ��
        if (isColumEmpty)
        {
            for (int m = j; m < N - 1; m++)
                for (int n = 0; n < N; n++)
                    grid[n][m] = grid[n][m + 1];

            // ���ұ�һ����Ϊ��
            for (int n = 0; n < N; n++)
                grid[n][N - 1] = 0;
        }
    }
}

// ��Ϸģʽѡ�����
void selectMode() {
    int windowWidth = 1000;
    int windowHeight = 800;
    IMAGE imbackground;
    loadimage(&imbackground, _T("��ʼ��.png"));
    initgraph(windowWidth, windowHeight);
    putimage(0, 0, &imbackground);
    settextcolor(BLACK);
    settextstyle(24, 0, _T("����")); // ������������Ϊ���壬�ֺ�Ϊ24
    setbkmode(TRANSPARENT);
    /* outtextxy(400, 100, _T("��ѡ����Ϸģʽ��"));
    outtextxy(400, 200, _T("1. �ֶ���"));
    outtextxy(400, 300, _T("2. �����Զ����"));*/

    // ��������ѡ��ľ�������(���ϡ��ҡ���)
    RECT rect1 = { 400, 100, 600, 150 };
    RECT rect2 = { 300, 200, 475, 250 };
    RECT rect3 = { 500, 200, 700, 250 };

    // ���ƾ�������
    setfillcolor(GREEN);
    fillrectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
    fillrectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);

    // �ھ�����������ʾ����
    drawtext(_T("��ѡ����Ϸģʽ��"), &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("�Լ���"), &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("�����Զ����"), &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //�����Ϣ��ѡ��ģʽ
    MOUSEMSG m;
    while (1) {
        if (MouseHit()) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                if (m.x >= rect2.left && m.x <= rect2.right && m.y >= rect2.top && m.y <= rect2.bottom) {
                    isAutoSolve = false;
                    break;
                }
                else if (m.x >= rect3.left && m.x <= rect3.right && m.y >= rect3.top && m.y <= rect3.bottom) {
                    isAutoSolve = true;
                    break;
                }
            }
        }
    }
    closegraph();
}

void startup()  //  ��ʼ������
{
    score = 0; // �÷ֳ�ʼ��Ϊ0��Ҳ�����������ķ�����Ŀ
    srand(time(0)); //  ��ʼ���������
    int windowWidth = N * tileSize; // ��Ļ���
    int windowHEIGHT = (N + 1.6) * tileSize; // ��Ļ�߶�
    initgraph(windowWidth, windowHEIGHT);	// �¿�����
    setbkcolor(BLACK);   // ���ñ�����ɫ
    cleardevice();    // �Ա�����ɫ����
    BeginBatchDraw(); // ��ʼ��������

    for (int i = 1; i < COLORNUM; i++) // ��ʼ�������������ɫ
    {
        float h = i * (360 / COLORNUM); // ������ɫ��ɫ��
        colors[i] = HSVtoRGB(h, 0.5, 0.9); //��Ӧɫ������ɫ
    }
    colors[0] = BLACK; // �����0��Ԫ��Ϊ��ɫ

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // ��ά����Ԫ����Ϊ�������ȡ��Ӧ��ɫ������������
            grid[i][j] = 1 + rand() % (COLORNUM - 1); // ֵΪ1��COLORNUM-1
        }
    }

    //��ʱ�ѳ�ʼ���趨��show()�����е����ָ�ʽ
    setbkmode(TRANSPARENT); // ��������͸�� 
    settextcolor(RGB(250, 250, 250));// �趨������ɫ
    settextstyle(0.6 * tileSize, 0, _T("����")); // �������ִ�С������
}

void show()  // ���ƺ���
{
    cleardevice(); // ����
    // �������е�С����
    for (int i = 0; i < N; i++) // ���б���
    {
        for (int j = 0; j < N; j++) // ���б���
        {
            // ��ά����Ԫ��Ϊ��Ӧ��ɫ������������
            setfillcolor(colors[grid[i][j]]);
            if (grid[i][j] == 0)
                setlinecolor(BLACK); // ��ɫ����ı߽�����Ϊ��ɫ
            else
                setlinecolor(WHITE); // ��ɫ����ı߽�����Ϊ��ɫ

            // ���Ƶ�ǰ�������
            fillrectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
        }
    }
    // ��ʾ�÷���Ϣ
    TCHAR s[20], s1[20]; // �����ַ�������
    swprintf_s(s, _T("�������飺%d"), score); // ������ת��Ϊ�ַ���
    // ������ʾ�ľ��������ڳ��������Ϸ�
    RECT r1 = { 0, N * tileSize, N * tileSize, (N + 1) * tileSize };//���ϡ��ҡ��±߽�
    // ����������ʾ�������֣�ˮƽ���С���ֱ���С���drawtext���������Ը��õ����þ��У���outtextxy������
    drawtext(s, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    //�����Զ����
    if (isAutoSolve) {
        TCHAR s1[20];
        swprintf_s(s1, _T("�Զ���������%d"), autosolvenum);
        RECT r3 = { 0, (N + 0.8) * tileSize, N * tileSize, (N + 1.6) * tileSize };
        drawtext(s1, &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    //����ֶ����
    if (!isAutoSolve) {
        RECT r2 = { 0, (N + 0.8) * tileSize, N * tileSize, (N + 1.6) * tileSize };
        drawtext(_T("�������ͬ��ɫ���飬�Ҽ����¿�ʼ"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    FlushBatchDraw(); // ��������
}

// �Զ���⺯��(��ǰ�����µ����Ž⣬�����������)
void autoSolve() {
    int maxMatchNum = 0; //��¼�ڵ�ǰ�����£�ͨ���������зǺ�ɫ���飬�ҵ���ͬɫ��ͨ������������
    int realnum = 0; //���ҵ����ͬɫ��ͨ�����λ�ú��ٴε��� floodfill ������ȷ��������ͬɫ��ͨ�����ڵķ�������
    int bestIC = -1, bestJC = -1; //�ֱ����ڼ�¼��������λ�õ��к���

    //���ñ���ö�ٶ�������ÿ���Ǻ�ɫ���鶼���м����ͨ������ 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0) {
                int searchedGrid[N][N] = { 0 };
                int matchNum=0; // ÿ��ѭ�������ʱ���������ڼ�¼ÿ�˱����ķ������ͨ����������maxMatchNum�Ƚϣ�����maxMatchNum
                floodfill(i, j, grid[i][j], searchedGrid, &matchNum);
                if (matchNum > maxMatchNum) {
                    maxMatchNum = matchNum;
                    bestIC = i;
                    bestJC = j;
                }
            }
        }
    }

    if (maxMatchNum >= 2) {
        int searchedGrid[N][N] = { 0 };
        floodfill(bestIC, bestJC, grid[bestIC][bestJC], searchedGrid, &realnum);
        // ���ƺ�ɫ�߿�����ʾ��ǰѡ��Ч��
        setlinecolor(RED);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    rectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
                }
            }
        }
        FlushBatchDraw();
        Sleep(700); // ��ʾһ��ʱ��
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    grid[i][j] = 0;
                }
            }
        }
        score += realnum;
        dropBlocks();
        moveBlocksLeft();
    }
    else {
        //��Ϣ������ʾ���ı����ݡ���Ϣ��ı���
        //ָ����Ϣ������ʾ�İ�ť���Ϊ ��OK�� ��ť
        MessageBox(GetHWnd(), _T("�޷�����������"), _T("��ʾ"), MB_OK);
        _getch();
    }
}

void update()  // ����
{
    //�����Զ����
    if (isAutoSolve) {
        autoSolve();
        autosolvenum++; // �Զ���������һ
        Sleep(1000); // ÿһ���Զ����һ��
    }
    //�ֶ����
    else {
        MOUSEMSG m;		 // ���������Ϣ
        if (MouseHit())   // ����������Ϣ
        {
            m = GetMouseMsg();  // ��������Ϣ
            if (m.uMsg == WM_LBUTTONDOWN) // �������������
            {
                // ���Ȼ���û�����Ķ�Ӧ���̸���кš��к�
                int iClicked = m.y / tileSize;
                int jClicked = m.x / tileSize;
                if (grid[iClicked][jClicked] > 0) // �Ǻ�ɫ����Ž�һ������
                {
                    // ��ά���飬���ڼ�¼floodfill�㷨�Ƿ�������ĳһԪ��
                    // û�б���������Ԫ��Ϊ0��������������ɫһ�£�Ԫ��Ϊ1��������������ɫ��һ�£�Ԫ��Ϊ-1
                    int searchedGrid[N][N] = { 0 };
                    int matchNum = 0; // �ҵ�ͬɫ����ĸ���

                    // ����FloodFill�㷨����
                    floodfill(iClicked, jClicked, grid[iClicked][jClicked], searchedGrid, &matchNum);

                    if (matchNum >= 2) // �����ͨͬ��ɫ������Ŀ���ڵ���2
                    {
                        // ���ƺ�ɫ�߿�                    
                        setlinecolor(RED);
                        for (int i = 0; i < N; i++)
                        {
                            for (int j = 0; j < N; j++)
                            {
                                if (searchedGrid[i][j] == 1)
                                {
                                    rectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
                                }
                            }
                        }
                        FlushBatchDraw();
                        Sleep(700); // ��ʾһ��ʱ��
                        for (int i = 0; i < N; i++) // ���б���
                            for (int j = 0; j < N; j++) // ���б���
                                if (searchedGrid[i][j] == 1)
                                    grid[i][j] = 0;  // �Ѷ�Ӧ�ķ����ɺ�ɫ

                        score += matchNum; // �÷�����
                        dropBlocks(); // �������䣬����Ϊ0�ĺ�ɫ����					
                        moveBlocksLeft(); // ĳһ��ȫΪ��ɫ���飬�ұ߷������������ƶ�
                    }
                }
            }
            else if (m.uMsg == WM_RBUTTONDOWN) // �����������Ҽ�
                startup(); // ��������һ��
        }
    }
}

int main() //  ������
{
    selectMode();
    startup();
    while (1)   // һֱѭ��
    {
        show();  // ���л���
        update();  // ����
    }
    return 0;
}