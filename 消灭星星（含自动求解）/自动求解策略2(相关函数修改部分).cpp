#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 方块的行列数
#define N 10 
// 方块颜色的种类数
#define COLORNUM 6
const int tileSize = 50; //小方块正方形的边长
COLORREF  colors[COLORNUM]; // 存储方块所有颜色的数组
int grid[N][N]; // 二维数组，记录游戏画面中所有小方块的颜色编号
int score; // 得分，也就是消除掉的方块数目
bool isAutoSolve = false; // 标记是否自动求解
int num = 0; // 记录自动消灭方块的求解次数


// 方块下落，填满为0的黑色方块
void dropBlocks(int tempGrid[N][N]) {
    // 让黑色方块上面的方块下落
    for (int j = 0; j < N; j++) // 对列遍历
    {
        int tempColumm[N] = { 0 }; // 新建一个临时列变量，存储去除0之后的元素
        int tcID = N - 1; // 用于拷贝的tempColumm元素序号

        for (int i = N - 1; i >= 0; i--) // 对行遍历
        {
            tempColumm[tcID] = tempGrid[i][j]; // 先把这一数据赋给临时数组
            // 如果当前元素为0，tcID不处理，下次循环会覆盖掉0值元素
            // 如果当前元素不为0，则减少tcID，上一句复制的元素有效保留到临时数组中了		
            if (tempGrid[i][j] != 0)
                tcID--;
        }

        for (int i = 0; i < N; i++) // 把临时数组的值赋回给二维数组的这一列
            tempGrid[i][j] = tempColumm[i];
    }
}

// 某一列方块全消除了，则右边的方块依次向左移动
void moveBlocksLeft(int tempGrid[N][N]) {
    // 如果有一列空了，右边的列向左移动
    for (int j = N - 1; j > 0; j--) // 对列遍历，从右向左遍历
    {
        int isColumEmpty = 1; // 这一列是否都是空白黑块，1是，0否
        for (int i = 0; i < N; i++) // 对行遍历
        {
            if (tempGrid[i][j] != 0)
            {
                isColumEmpty = 0;
                break; // 这一列有一个不是黑块，就跳出循环
            }
        }

        // 这一列都是黑色方块的，从这一列开始向右，每一列移动到左边一列
        if (isColumEmpty)
        {
            for (int m = j; m < N - 1; m++)
                for (int n = 0; n < N; n++)
                    tempGrid[n][m] = tempGrid[n][m + 1];

            // 最右边一列设为空
            for (int n = 0; n < N; n++)
                tempGrid[n][N - 1] = 0;
        }
    }
}


// 自动求解函数（不仅考虑当前状态，也考虑消除后的下一次方格块状态来取最佳）
void autoSolve1() {
    const int simulation = 100; // 模拟次数，可以根据需要调整
    int bestTotalMatchNum = 0; // 记录最佳方案的两次消除方块总数的初始化
    int bestIC = -1, bestJC = -1; // 记录最佳方案的起始方块坐标

    srand(time(0)); // 初始化随机数种子

    for (int sim = 0; sim < simulation; ++sim) {
        int availableBlocks[100][2]; // 存储可用方块的坐标，假设最多100个可用方块,第二维的0、1分别存储行、列坐标
        int availableCount = 0;

        // 找出所有可用的方块
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
            // 如果没有可用方块，提示无法继续消除
            MessageBox(GetHWnd(), _T("无法继续消除！"), _T("提示"), MB_OK);
            _getch();
            return;
        }

        // 随机选择一个可用方块
        int randomIndex = rand() % availableCount; //生成一个在 0 到 availableCount - 1 范围内的随机整数
        int i = availableBlocks[randomIndex][0];
        int j = availableBlocks[randomIndex][1];

        int tempGrid[N][N];
        // 备份当前网格状态
        for (int m = 0; m < N; m++) {
            for (int n = 0; n < N; n++) {
                tempGrid[m][n] = grid[m][n]; //为了在模拟消除操作时，不影响原始的游戏网格状态
            }
        }

        int firstSearchedGrid[N][N] = { 0 };
        int firstMatchNum = 0;
        floodfill(i, j, grid[i][j], firstSearchedGrid, &firstMatchNum);

        if (firstMatchNum >= 2) {
            // 模拟消除当前连通区域
            for (int m = 0; m < N; m++) {
                for (int n = 0; n < N; n++) {
                    if (firstSearchedGrid[m][n] == 1) {
                        tempGrid[m][n] = 0;
                    }
                }
            }

            // 模拟方块下落和左移
            dropBlocks(tempGrid);
            moveBlocksLeft(tempGrid);

            // 计算新状态下可消除的最大连通区域（方法与求解策略1中只考虑当前方格盘状态代码大同）
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

            //对应最开始的循环模拟次数得到最终的bestTotalMatchNum、bestIC、bestJC
            int totalMatchNum = firstMatchNum + secondMaxMatchNum;
            if (totalMatchNum > bestTotalMatchNum) {
                bestTotalMatchNum = totalMatchNum;
                bestIC = i;
                bestJC = j;
            }
        }
    }

    //说明找到了最优方案
    if (bestIC != -1) {
        int searchedGrid[N][N] = { 0 };
        int realnum = 0;
        floodfill(bestIC, bestJC, grid[bestIC][bestJC], searchedGrid, &realnum);

        // 绘制红色边框
        setlinecolor(RED);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    rectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
                }
            }
        }
        FlushBatchDraw();
        Sleep(500); // 显示一段时间

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
        MessageBox(GetHWnd(), _T("无法继续消除！"), _T("提示"), MB_OK);
        _getch();
    }
}
