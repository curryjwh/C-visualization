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
int autosolvenum = 0; // 记录自动消灭方块的求解次数

// FloodFill搜索算法，向点击方块(ic,jc)的上下左右四个方向寻找
// clicedValue为当前被点击方块的颜色序号值
// searchedGrid[N][N]记录某一方块是否被检索过，0没有，1有且同色，-1有且异色
// matchNum为同色连通方块的个数
void floodfill(int ic, int jc, int clicedValue, int searchedGrid[N][N], int* matchNum)
{
    if (grid[ic][jc] == clicedValue) // 与被点击方块颜色相同
    {
        searchedGrid[ic][jc] = 1;  // 当前方块标记已检索过且同色
        (*matchNum)++; // 同色连通方块的个数加1
    }
    else // 与点击方块颜色不同
    {
        searchedGrid[ic][jc] = -1; // 当前方块标记已检索过且异色
        return; //  不用再找下去了
    }

    // 如果某一方向的位置没有超出范围，且没有被找过，就向这个方向递归寻找
    if (ic - 1 >= 0 && searchedGrid[ic - 1][jc] == 0)
        floodfill(ic - 1, jc, clicedValue, searchedGrid, matchNum); // 上

    if (ic + 1 < N && searchedGrid[ic + 1][jc] == 0)
        floodfill(ic + 1, jc, clicedValue, searchedGrid, matchNum); // 下

    if (jc - 1 >= 0 && searchedGrid[ic][jc - 1] == 0)
        floodfill(ic, jc - 1, clicedValue, searchedGrid, matchNum); // 左

    if (jc + 1 < N && searchedGrid[ic][jc + 1] == 0)
        floodfill(ic, jc + 1, clicedValue, searchedGrid, matchNum); // 右
}

// 方块下落，填满为0的黑色方块
void dropBlocks()
{
    // 让黑色方块上面的方块下落
    for (int j = 0; j < N; j++) // 对列遍历
    {
        int tempColumm[N] = { 0 }; // 新建一个临时列变量，存储去除0之后的元素
        int tcID = N - 1; // 用于拷贝的tempColumm元素序号

        for (int i = N - 1; i >= 0; i--) // 对行遍历
        {
            tempColumm[tcID] = grid[i][j]; // 先把这一数据赋给临时数组
            // 如果当前元素为0，tcID不处理，下次循环会覆盖掉0值元素
            // 如果当前元素不为0，则减少tcID，上一句复制的元素有效保留到临时数组中了		
            if (grid[i][j] != 0)
                tcID--;
        }

        for (int i = 0; i < N; i++) // 把临时数组的值赋回给二维数组的这一列
            grid[i][j] = tempColumm[i];
    }
}

// 某一列方块全消除了，则右边的方块依次向左移动
void moveBlocksLeft()
{
    // 如果有一列空了，右边的列向左移动
    for (int j = N - 1; j > 0; j--) // 对列遍历，从右向左遍历
    {
        int isColumEmpty = 1; // 这一列是否都是空白黑块，1是，0否
        for (int i = 0; i < N; i++) // 对行遍历
        {
            if (grid[i][j] != 0)
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
                    grid[n][m] = grid[n][m + 1];

            // 最右边一列设为空
            for (int n = 0; n < N; n++)
                grid[n][N - 1] = 0;
        }
    }
}

// 游戏模式选择界面
void selectMode() {
    int windowWidth = 1000;
    int windowHeight = 800;
    IMAGE imbackground;
    loadimage(&imbackground, _T("初始化.png"));
    initgraph(windowWidth, windowHeight);
    putimage(0, 0, &imbackground);
    settextcolor(BLACK);
    settextstyle(24, 0, _T("宋体")); // 设置文字字体为宋体，字号为24
    setbkmode(TRANSPARENT);
    /* outtextxy(400, 100, _T("请选择游戏模式："));
    outtextxy(400, 200, _T("1. 手动玩"));
    outtextxy(400, 300, _T("2. 电脑自动求解"));*/

    // 定义三个选项的矩形区域(左、上、右、下)
    RECT rect1 = { 400, 100, 600, 150 };
    RECT rect2 = { 300, 200, 475, 250 };
    RECT rect3 = { 500, 200, 700, 250 };

    // 绘制矩形区域
    setfillcolor(GREEN);
    fillrectangle(rect2.left, rect2.top, rect2.right, rect2.bottom);
    fillrectangle(rect3.left, rect3.top, rect3.right, rect3.bottom);

    // 在矩形区域内显示文字
    drawtext(_T("请选择游戏模式："), &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("自己玩"), &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T("电脑自动求解"), &rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    //鼠标消息：选择模式
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

void startup()  //  初始化函数
{
    score = 0; // 得分初始化为0，也就是消除掉的方块数目
    srand(time(0)); //  初始化随机种子
    int windowWidth = N * tileSize; // 屏幕宽度
    int windowHEIGHT = (N + 1.6) * tileSize; // 屏幕高度
    initgraph(windowWidth, windowHEIGHT);	// 新开窗口
    setbkcolor(BLACK);   // 设置背景颜色
    cleardevice();    // 以背景颜色清屏
    BeginBatchDraw(); // 开始批量绘制

    for (int i = 1; i < COLORNUM; i++) // 初始化所有种类的颜色
    {
        float h = i * (360 / COLORNUM); // 这种颜色的色调
        colors[i] = HSVtoRGB(h, 0.5, 0.9); //对应色调的颜色
    }
    colors[0] = BLACK; // 数组第0个元素为黑色

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // 二维数组元素设为随机数，取对应颜色种类数组的序号
            grid[i][j] = 1 + rand() % (COLORNUM - 1); // 值为1到COLORNUM-1
        }
    }

    //此时已初始化设定了show()函数中的文字格式
    setbkmode(TRANSPARENT); // 文字字体透明 
    settextcolor(RGB(250, 250, 250));// 设定文字颜色
    settextstyle(0.6 * tileSize, 0, _T("宋体")); // 设置文字大小、字体
}

void show()  // 绘制函数
{
    cleardevice(); // 清屏
    // 绘制所有的小方块
    for (int i = 0; i < N; i++) // 对行遍历
    {
        for (int j = 0; j < N; j++) // 对列遍历
        {
            // 二维数组元素为对应颜色种类数组的序号
            setfillcolor(colors[grid[i][j]]);
            if (grid[i][j] == 0)
                setlinecolor(BLACK); // 黑色方块的边界线设为黑色
            else
                setlinecolor(WHITE); // 彩色方块的边界线设为白色

            // 绘制当前方块矩形
            fillrectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
        }
    }
    // 显示得分信息
    TCHAR s[20], s1[20]; // 定义字符串数组
    swprintf_s(s, _T("消除方块：%d"), score); // 将数字转换为字符串
    // 文字显示的矩形区域，在长方形正上方
    RECT r1 = { 0, N * tileSize, N * tileSize, (N + 1) * tileSize };//左、上、右、下边界
    // 在区域内显示数字文字，水平居中、竖直居中。用drawtext函数，可以更好的设置居中，比outtextxy更方便
    drawtext(s, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    //电脑自动求解
    if (isAutoSolve) {
        TCHAR s1[20];
        swprintf_s(s1, _T("自动求解次数：%d"), autosolvenum);
        RECT r3 = { 0, (N + 0.8) * tileSize, N * tileSize, (N + 1.6) * tileSize };
        drawtext(s1, &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    //玩家手动求解
    if (!isAutoSolve) {
        RECT r2 = { 0, (N + 0.8) * tileSize, N * tileSize, (N + 1.6) * tileSize };
        drawtext(_T("左键消除同颜色方块，右键重新开始"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    FlushBatchDraw(); // 批量绘制
}

// 自动求解函数(当前局面下的最优解，并非随机消除)
void autoSolve() {
    int maxMatchNum = 0; //记录在当前局面下，通过遍历所有非黑色方块，找到的同色连通方块的最大数量
    int realnum = 0; //在找到最大同色连通方块的位置后，再次调用 floodfill 函数来确定这个最大同色连通区域内的方块数量
    int bestIC = -1, bestJC = -1; //分别用于记录最优消除位置的行和列

    //采用暴力枚举对网格里每个非黑色方块都进行检查联通方块数 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] > 0) {
                int searchedGrid[N][N] = { 0 };
                int matchNum=0; // 每次循环里的临时变量，用于记录每此遍历的方格块连通数量，并与maxMatchNum比较，更新maxMatchNum
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
        // 绘制红色边框以显示当前选中效果
        setlinecolor(RED);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (searchedGrid[i][j] == 1) {
                    rectangle(j * tileSize, i * tileSize, (j + 1) * tileSize, (i + 1) * tileSize);
                }
            }
        }
        FlushBatchDraw();
        Sleep(700); // 显示一段时间
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
        //消息框中显示的文本内容、消息框的标题
        //指定消息框中显示的按钮风格为 “OK” 按钮
        MessageBox(GetHWnd(), _T("无法继续消除！"), _T("提示"), MB_OK);
        _getch();
    }
}

void update()  // 更新
{
    //电脑自动求解
    if (isAutoSolve) {
        autoSolve();
        autosolvenum++; // 自动求解次数加一
        Sleep(1000); // 每一秒自动求解一次
    }
    //手动求解
    else {
        MOUSEMSG m;		 // 定义鼠标消息
        if (MouseHit())   // 如果有鼠标消息
        {
            m = GetMouseMsg();  // 获得鼠标消息
            if (m.uMsg == WM_LBUTTONDOWN) // 如果按下鼠标左键
            {
                // 首先获得用户点击的对应棋盘格的行号、列号
                int iClicked = m.y / tileSize;
                int jClicked = m.x / tileSize;
                if (grid[iClicked][jClicked] > 0) // 非黑色方块才进一步处理
                {
                    // 二维数组，用于记录floodfill算法是否搜索过某一元素
                    // 没有被搜索过，元素为0；被搜索过且颜色一致，元素为1；被搜索过且颜色不一致，元素为-1
                    int searchedGrid[N][N] = { 0 };
                    int matchNum = 0; // 找到同色方块的个数

                    // 调用FloodFill算法查找
                    floodfill(iClicked, jClicked, grid[iClicked][jClicked], searchedGrid, &matchNum);

                    if (matchNum >= 2) // 如果连通同颜色方块数目大于等于2
                    {
                        // 绘制红色边框                    
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
                        Sleep(700); // 显示一段时间
                        for (int i = 0; i < N; i++) // 对行遍历
                            for (int j = 0; j < N; j++) // 对列遍历
                                if (searchedGrid[i][j] == 1)
                                    grid[i][j] = 0;  // 把对应的方块变成黑色

                        score += matchNum; // 得分增加
                        dropBlocks(); // 方块下落，填满为0的黑色方块					
                        moveBlocksLeft(); // 某一列全为黑色方块，右边方块依次向左移动
                    }
                }
            }
            else if (m.uMsg == WM_RBUTTONDOWN) // 如果按下鼠标右键
                startup(); // 重新再来一局
        }
    }
}

int main() //  主函数
{
    selectMode();
    startup();
    while (1)   // 一直循环
    {
        show();  // 进行绘制
        update();  // 更新
    }
    return 0;
}