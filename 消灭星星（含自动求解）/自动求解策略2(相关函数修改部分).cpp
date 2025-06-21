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
int num = 0; // ��¼�Զ����𷽿��������


// �������䣬����Ϊ0�ĺ�ɫ����
void dropBlocks(int tempGrid[N][N]) {
    // �ú�ɫ��������ķ�������
    for (int j = 0; j < N; j++) // ���б���
    {
        int tempColumm[N] = { 0 }; // �½�һ����ʱ�б������洢ȥ��0֮���Ԫ��
        int tcID = N - 1; // ���ڿ�����tempColummԪ�����

        for (int i = N - 1; i >= 0; i--) // ���б���
        {
            tempColumm[tcID] = tempGrid[i][j]; // �Ȱ���һ���ݸ�����ʱ����
            // �����ǰԪ��Ϊ0��tcID�������´�ѭ���Ḳ�ǵ�0ֵԪ��
            // �����ǰԪ�ز�Ϊ0�������tcID����һ�临�Ƶ�Ԫ����Ч��������ʱ��������		
            if (tempGrid[i][j] != 0)
                tcID--;
        }

        for (int i = 0; i < N; i++) // ����ʱ�����ֵ���ظ���ά�������һ��
            tempGrid[i][j] = tempColumm[i];
    }
}

// ĳһ�з���ȫ�����ˣ����ұߵķ������������ƶ�
void moveBlocksLeft(int tempGrid[N][N]) {
    // �����һ�п��ˣ��ұߵ��������ƶ�
    for (int j = N - 1; j > 0; j--) // ���б����������������
    {
        int isColumEmpty = 1; // ��һ���Ƿ��ǿհ׺ڿ飬1�ǣ�0��
        for (int i = 0; i < N; i++) // ���б���
        {
            if (tempGrid[i][j] != 0)
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
                    tempGrid[n][m] = tempGrid[n][m + 1];

            // ���ұ�һ����Ϊ��
            for (int n = 0; n < N; n++)
                tempGrid[n][N - 1] = 0;
        }
    }
}


// �Զ���⺯�����������ǵ�ǰ״̬��Ҳ�������������һ�η����״̬��ȡ��ѣ�
void autoSolve1() {
    const int simulation = 100; // ģ����������Ը�����Ҫ����
    int bestTotalMatchNum = 0; // ��¼��ѷ����������������������ĳ�ʼ��
    int bestIC = -1, bestJC = -1; // ��¼��ѷ�������ʼ��������

    srand(time(0)); // ��ʼ�����������

    for (int sim = 0; sim < simulation; ++sim) {
        int availableBlocks[100][2]; // �洢���÷�������꣬�������100�����÷���,�ڶ�ά��0��1�ֱ�洢�С�������
        int availableCount = 0;

        // �ҳ����п��õķ���
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] > 0) {
                    availableBlocks[availableCount][0] = i;
                    availableBlocks[availableCount][1] = j;
                    availableCount++;
                }
            }
        }

        if (availableCount == 0) {
            // ���û�п��÷��飬��ʾ�޷���������
            MessageBox(GetHWnd(), _T("�޷�����������"), _T("��ʾ"), MB_OK);
            _getch();
            return;
        }

        // ���ѡ��һ�����÷���
        int randomIndex = rand() % availableCount; //����һ���� 0 �� availableCount - 1 ��Χ�ڵ��������
        int i = availableBlocks[randomIndex][0];
        int j = availableBlocks[randomIndex][1];

        int tempGrid[N][N];
        // ���ݵ�ǰ����״̬
        for (int m = 0; m < N; m++) {
            for (int n = 0; n < N; n++) {
                tempGrid[m][n] = grid[m][n]; //Ϊ����ģ����������ʱ����Ӱ��ԭʼ����Ϸ����״̬
            }
        }

        int firstSearchedGrid[N][N] = { 0 };
        int firstMatchNum = 0;
        floodfill(i, j, grid[i][j], firstSearchedGrid, &firstMatchNum);

        if (firstMatchNum >= 2) {
            // ģ��������ǰ��ͨ����
            for (int m = 0; m < N; m++) {
                for (int n = 0; n < N; n++) {
                    if (firstSearchedGrid[m][n] == 1) {
                        tempGrid[m][n] = 0;
                    }
                }
            }

            // ģ�ⷽ�����������
            dropBlocks(tempGrid);
            moveBlocksLeft(tempGrid);

            // ������״̬�¿������������ͨ���򣨷�����������1��ֻ���ǵ�ǰ������״̬�����ͬ��
            int secondMaxMatchNum = 0;
            for (int p = 0; p < N; p++) {
                for (int q = 0; q < N; q++) {
                    if (tempGrid[p][q] > 0) {
                        int secondSearchedGrid[N][N] = { 0 };
                        int secondMatchNum = 0;
                        floodfill(p, q, tempGrid[p][q], secondSearchedGrid, &secondMatchNum);
                        if (secondMatchNum > secondMaxMatchNum) {
                            secondMaxMatchNum = secondMatchNum;
                        }
                    }
                }
            }

            //��Ӧ�ʼ��ѭ��ģ������õ����յ�bestTotalMatchNum��bestIC��bestJC
            int totalMatchNum = firstMatchNum + secondMaxMatchNum;
            if (totalMatchNum > bestTotalMatchNum) {
                bestTotalMatchNum = totalMatchNum;
                bestIC = i;
                bestJC = j;
            }
        }
    }

    //˵���ҵ������ŷ���
    if (bestIC != -1) {
        int searchedGrid[N][N] = { 0 };
        int realnum = 0;
        floodfill(bestIC, bestJC, grid[bestIC][bestJC], searchedGrid, &realnum);

        // ���ƺ�ɫ�߿�
        setlinecolor(RED);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    rectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
                }
            }
        }
        FlushBatchDraw();
        Sleep(500); // ��ʾһ��ʱ��

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    grid[i][j] = 0;
                }
            }
        }
        score += realnum;
        dropBlocks(grid);
        moveBlocksLeft(grid);
    }
    else {
        MessageBox(GetHWnd(), _T("�޷�����������"), _T("��ʾ"), MB_OK);
        _getch();
    }
}
