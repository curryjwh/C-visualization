#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>  // EasyX图形库
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
//#include <functional>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

// 全局变量定义
const int WINDOW_WIDTH = 600; //窗口x
const int WINDOW_HEIGHT = 700; //窗口y
const int BOARD_SIZE = 300;
const int BOARD_X = 150; //棋盘左上角横坐标
const int BOARD_Y = 100; //棋盘左上角纵坐标
const int TILE_SIZE = BOARD_SIZE / 3; //每一个数码格的横向宽度
const int STATS_Y = BOARD_Y + BOARD_SIZE + 50;
const int INSTRUCTIONS_Y = STATS_Y + 30;  // 指令说明的起始Y坐标
const int INSTRUCTIONS_LINE_HEIGHT = 30;  // 指令说明行高

// 移动方向：上、右、下、左
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

// 八数码问题的状态表示
struct State {
    int board[3][3];      // 3x3棋盘
    int zeroRow, zeroCol; // 空格(0)的位置
    int depth;            // 当前搜索深度
    int f;                // A*算法的估价函数f = g + h，g为深度，h为曼哈顿距离

    //不仅是找到最终解答状态，还需要知道从初始状态到目标状态的完整移动序列
    //找到解决方案时，通过反向遍历这些父节点指针，可以重建从初始状态到目标状态的完整路径，然后再将路径反转以获得正向的解决步骤序列。
    State* parent;        // 父节点指针，用于回溯路径


    // 默认构造函数
    State() : depth(0), f(0), parent(nullptr) {
        //初始化棋盘，将所有元素置0
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = 0;
            }
        }
        zeroRow = zeroCol = 0;
    }

    // 拷贝构造函数，用于复制一个状态
    State(const State& other) : depth(other.depth), f(other.f),
        parent(other.parent), zeroRow(other.zeroRow), zeroCol(other.zeroCol) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = other.board[i][j];
            }
        }
    }

    // 重载等于运算符
    bool operator==(const State& other) const {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] != other.board[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // 为优先队列添加比较运算符，用于A*算法
    bool operator<(const State& other) const {
        return f > other.f;  // 使f值小的有更高优先级
    }
};

// 哈希函数，用于unordered_set和unordered_map
namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State& s) const {
            size_t hash_val = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    hash_val = hash_val * 10 + s.board[i][j];
                }
            }
            return hash_val;
        }
    };
}

// 游戏状态变量
State goalState;
State initialState;
vector<State> solution; // 存储求解过程中的每一步
bool running = true; // 程序是否在开始执行阶段
bool solving = false; // 程序是否在求解状态
int solutionStep = 0; // 记录求解的步数
TCHAR methodName[100] = _T("");
double time_taken = 0.0; // 记录不同算法的执行时间

// 计算曼哈顿距离
int calculateManhattanDistance(const State& state) {
    int distance = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int value = state.board[i][j];
            if (value != 0) {
                // 计算当前数字到目标位置的曼哈顿距离
                int targetRow = (value - 1) / 3;
                int targetCol = (value - 1) % 3;
                distance += abs(i - targetRow) + abs(j - targetCol);
            }
        }
    }
    return distance;
}

// 检查状态是否可解
bool isSolvable(const State& state) {
    // 将二维数组转为一维数组，不包括0
    // 在计算逆序数时,只关心实际数字的相对顺序，空格移动不影响逆序数计算
    vector<int> reverseBoard;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state.board[i][j] != 0) {
                reverseBoard.push_back(state.board[i][j]);
            }
        }
    }

    // 计算逆序数(指这个排列中所有逆序对的数量)
    // 如果一个较大的数排在较小的数前面，那么这两个数构成一个逆序对
    int inversions = 0;
    for (int i = 0; i < reverseBoard.size(); ++i) {
        for (int j = i + 1; j < reverseBoard.size(); ++j) {
            if (reverseBoard[i] > reverseBoard[j]) {
                inversions++;
            }
        }
    }
    // 对于3x3的八数码问题，初始状态和目标状态的逆序数奇偶性相同时可解
    // 目标状态的逆序数为0
    return inversions % 2 == 0;
}

// 生成随机的初始状态
State generateRandomState() {
    State state;
    vector<int> numbers = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    srand(time(0));
    do {
        // 洗牌算法
        // 从数组的最后一个元素开始，逐步向前每次随机选择前面的一个元素(包括当前元素)进行交换
        for (int i = 8; i > 0; --i) {
            int j = rand() % (i + 1);  // 从[0,i]中随机选择一个位置
            swap(numbers[i], numbers[j]);  
        }
        // 填充初始棋盘
        int index = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                state.board[i][j] = numbers[index++];
                if (state.board[i][j] == 0) {
                    state.zeroRow = i;
                    state.zeroCol = j;
                }
            }
        }
    } while (!isSolvable(state)); // 确保生成的状态是可解的
    return state;
}

// A*算法求解
vector<State> solveAStar(const State& initialState) {
    // 优先队列f值小的优先级较高
    priority_queue<State> openList;
    unordered_set<State> closedList; // 去重，防止重复扩展
    unordered_map<State, State*> stateMap; // 存储状态之间的父子关系，用于在找到目标后重建路径，键是子状态，值是指向父状态的指针

    State start = initialState;
    start.f = start.depth + calculateManhattanDistance(start);
    openList.push(start);

    while (!openList.empty()) {
        State current = openList.top();
        openList.pop();

        // 检查是否达到目标状态
        if (calculateManhattanDistance(current) == 0) {
            // 存储从初始状态到目标状态的完整路径
            vector<State> path;
            // 初始化指针，指向当前状态（即目标状态）
            State* currentPtr = &current; //指向 State 类型的指针
            while (currentPtr != nullptr) {
                path.push_back(*currentPtr);
                if (currentPtr->parent == nullptr) break;
                currentPtr = stateMap[*currentPtr];
            }
            reverse(path.begin(), path.end());
            return path; //得到的路径是从目标状态到初始状态，需要反转为从初始状态到目标状态
        }

        // 将当前状态加入已访问集合
        closedList.insert(current);

        // 尝试四个方向的移动
        for (int i = 0; i < 4; ++i) {
            int newRow = current.zeroRow + dx[i];
            int newCol = current.zeroCol + dy[i];

            // 检查是否越界
            if (newRow < 0 || newRow >= 3 || newCol < 0 || newCol >= 3) {
                continue;
            }

            // 创建新状态
            State next = current; //新状态next继承current的所有属性
            //模拟空格移动操作
            next.board[next.zeroRow][next.zeroCol] = next.board[newRow][newCol];
            next.board[newRow][newCol] = 0;
            next.zeroRow = newRow;
            next.zeroCol = newCol;
            next.depth = current.depth + 1;
            next.f = next.depth + calculateManhattanDistance(next);
            next.parent = &current;

            // 如果该状态已被访问，则跳过
            if (closedList.find(next) != closedList.end()) {
                continue;
            }

            // 保存父节点信息用于回溯
            stateMap[next] = new State(current); // 创建一个新的State对象作为current状态的副本，并且返回该对象的指针
            // 加入开放列表
            openList.push(next);
        }
    }
    // 如果无解，返回空路径
    return vector<State>();
}



// 深度受限搜索(DLS)
bool DLS(const State& state, int limit, vector<State>& path, unordered_set<State>& visited) {
    // 如果达到目标状态
    if (calculateManhattanDistance(state) == 0) {
        path.push_back(state);
        return true;
    }

    // 如果达到深度限制
    if (state.depth >= limit) {
        return false;
    }

    // 标记当前状态为已访问
    visited.insert(state);

    // 尝试四个方向的移动
    for (int i = 0; i < 4; ++i) {
        int newRow = state.zeroRow + dx[i];
        int newCol = state.zeroCol + dy[i];

        // 检查是否越界
        if (newRow < 0 || newRow >= 3 || newCol < 0 || newCol >= 3) {
            continue;
        }

        State next = state;
        next.board[next.zeroRow][next.zeroCol] = next.board[newRow][newCol];
        next.board[newRow][newCol] = 0;
        next.zeroRow = newRow;
        next.zeroCol = newCol;
        next.depth = state.depth + 1;
        next.parent = nullptr;

        // 如果该状态已被访问，则跳过
        if (visited.find(next) != visited.end()) {
            continue;
        }

        // 递归搜索
        // 从一条路径搜到底后要是最后能达到目标状态，又反向回溯path上的每一个state，然后逐一push_back
        if (DLS(next, limit, path, visited)) {
            path.push_back(state);
            return true;
        }
    }

    return false;
}
// 迭代加深搜索(IDS)
vector<State> solveIDS(const State& initialState, int maxDepth = 40) {
    vector<State> path;

    // 从深度1开始，逐渐增加深度限制
    for (int limit = 1; limit <= maxDepth; ++limit) {
        unordered_set<State> visited;
        if (DLS(initialState, limit, path, visited)) {
            reverse(path.begin(), path.end()); //将反向回溯收集到的路径改成正序
            return path;
        }
    }

    // 如果在最大深度内找不到解，返回空路径
    return vector<State>();
}



// 贪婪最佳优先搜索
vector<State> solveGreedyBestFirst(const State& initialState) {
    priority_queue<State> openList;
    unordered_set<State> closedList;
    unordered_map<State, State*> stateMap; // 去重、无序

    State start = initialState;
    start.f = calculateManhattanDistance(start); // 仅使用曼哈顿距离作为启发式函数
    openList.push(start);

    while (!openList.empty()) {
        State current = openList.top();
        openList.pop();

        if (calculateManhattanDistance(current) == 0) {
            vector<State> path;
            State* currentPtr = &current;
            while (currentPtr != nullptr) {
                path.push_back(*currentPtr);
                if (currentPtr->parent == nullptr) break;
                currentPtr = stateMap[*currentPtr];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        closedList.insert(current);

        for (int i = 0; i < 4; ++i) {
            int newRow = current.zeroRow + dx[i];
            int newCol = current.zeroCol + dy[i];

            if (newRow < 0 || newRow >= 3 || newCol < 0 || newCol >= 3) {
                continue;
            }

            State next = current;
            next.board[next.zeroRow][next.zeroCol] = next.board[newRow][newCol];
            next.board[newRow][newCol] = 0;
            next.zeroRow = newRow;
            next.zeroCol = newCol;
            next.depth = current.depth + 1;
            next.f = calculateManhattanDistance(next);
            next.parent = &current;

            if (closedList.find(next) != closedList.end()) {
                continue;
            }

            stateMap[next] = new State(current);
            openList.push(next);
        }
    }

    return vector<State>();
}
//虽然贪婪最佳优先搜索在执行时间上有优势，但不能保证找到最优解，求解步数较多是因为可能会陷入局部最优，错过更优的路径。
//使用深度受限搜索函数 DLS 来查找解找到目标状态时，路径是通过递归调用栈反向构建的，递归调用本身就形成了一个自然的回溯机制，而不需要像 A* 算法和贪婪最佳优先搜索那样维护一个完整的父节点映射


// 初始化函数
void startup()
{
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(WHITE);
    cleardevice();
    BeginBatchDraw();

    // 设置目标状态 (123/456/780)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            goalState.board[i][j] = i * 3 + j + 1;
        }
    }
    goalState.board[2][2] = 0;
    goalState.zeroRow = 2;
    goalState.zeroCol = 2;

    // 创建初始状态
    initialState = generateRandomState();
}

// 绘制棋盘和游戏状态
void show()
{
    cleardevice();

    // 绘制棋盘背景
    setfillcolor(RGB(240, 240, 240));
    fillrectangle(BOARD_X, BOARD_Y, BOARD_X + BOARD_SIZE, BOARD_Y + BOARD_SIZE);

    // 设置字体
    settextstyle(TILE_SIZE / 3, 0, _T("微软雅黑")); //文字高度、自适应宽度
    settextcolor(BLACK);

    // 绘制标题
    TCHAR title[100];
    // 初始页面
    if (!solving) {
        wcscpy(title, _T("八数码问题 - 初始状态")); //把一个宽字符字符串复制到另一个宽字符数组
    }
    // 选择方法，开始求解后页面
    else {
        swprintf_s(title, _T("八数码问题 - %s"), methodName); //把格式化的宽字符数据写入到一个宽字符数组中
    }
    outtextxy(WINDOW_WIDTH / 2 - textwidth(title) / 2, 50, title);

    // 获取当前要显示的状态
    State currentState;
    if (solving && solutionStep < solution.size()) {
        currentState = solution[solutionStep];
    }
    else {
        currentState = initialState;
    }

    // 绘制每个方格
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = BOARD_X + j * TILE_SIZE;
            int y = BOARD_Y + i * TILE_SIZE;

            // 绘制方格背景和边框
            setlinecolor(BLACK);
            rectangle(x, y, x + TILE_SIZE, y + TILE_SIZE);

            if (currentState.board[i][j] != 0) {
                setfillcolor(RGB(100, 180, 255));
                // 使用透明方式绘制方块
                setbkmode(TRANSPARENT);
                fillrectangle(x, y, x + TILE_SIZE, y + TILE_SIZE);
                // 绘制数字
                TCHAR numStr[2];
                swprintf_s(numStr, _T("%d"), currentState.board[i][j]);
                int numWidth = textwidth(numStr);
                int numHeight = textheight(numStr);
                //数字居中处理
                outtextxy(x + TILE_SIZE / 2 - numWidth / 2, y + TILE_SIZE / 2 - numHeight / 2, numStr);
            }
        }
    }

    // 绘制步数信息
    if (solving) {
        TCHAR stepsInfo[50];
        swprintf_s(stepsInfo, _T("已完成步数: %d / %d (用时: %.2f秒)"),
            solutionStep, solution.size() - 1, time_taken);
        outtextxy(WINDOW_WIDTH / 2 - textwidth(stepsInfo) / 2, STATS_Y, stepsInfo);
    }

    // 如果是初始状态，显示操作说明
    if (!solving) {
        //每个元素都是一个指向常量 TCHAR 字符串的指针
        const TCHAR* instructions[10] = {
            _T("按键说明:"),
            _T("A - 使用A*算法求解"),
            _T("D - 使用深度受限+迭代加深搜索求解"),
            _T("S - 使用贪婪最佳优先搜索求解"),
            _T("R - 重新生成随机初始状态"),
            _T("ESC - 退出程序")
        };

        
        // 设置背景矩形
        setfillcolor(RGB(245, 245, 245));
        int maxWidth = 0;
        // 根据最长的文字设置宽度
        for (int i = 0; i < 6; ++i) {
            int width = textwidth(instructions[i]);
            if (width > maxWidth) {
                maxWidth = width;
            }
        }

        // 绘制说明背景
        int instructionsX = WINDOW_WIDTH / 2 - maxWidth / 2 - 20;
        int instructionsWidth = maxWidth + 40;
        fillrectangle(instructionsX, INSTRUCTIONS_Y - 10,
            instructionsX + instructionsWidth,
            INSTRUCTIONS_Y + 6 * INSTRUCTIONS_LINE_HEIGHT);

        // 绘制说明文字
        for (int i = 0; i < 6; ++i) {
            outtextxy(WINDOW_WIDTH / 2 - textwidth(instructions[i]) / 2,
                INSTRUCTIONS_Y + i * INSTRUCTIONS_LINE_HEIGHT,
                instructions[i]);
        }
    }

    // 刷新显示
    FlushBatchDraw();
}


// 与输入无关的更新
void updateWithoutInput()
{
    // 如果正在演示求解过程
    if (solving && !solution.empty()) {
        solutionStep++;
        Sleep(500);
        // 检查是否完成所有步骤
        if (solutionStep >= solution.size()) {
            // 演示完成，回到初始状态
            solving = false;
            solutionStep = 0;

            // 显示完成信息
            TCHAR completeMsg[100];
            swprintf_s(completeMsg, _T("求解完成！总步数: %d"), solution.size() - 1);
            MessageBox(GetHWnd(), completeMsg, _T("提示"), MB_OK | MB_ICONINFORMATION);
        }
    }
}


// 与输入有关的更新
void updateWithInput()
{
    // 如果不在求解过程中，处理用户输入
    if (!solving) {
        // 处理消息
        ExMessage msg;
        if (peekmessage(&msg, EM_KEY)) { // 只获取键盘相关的消息
            if (msg.message == WM_KEYDOWN) { // 如果键盘按键被按下
                switch (msg.vkcode) {
                case VK_ESCAPE: // ESC键退出
                    running = false;
                    break;

                case 'A': // A*算法
                {
                    clock_t start = clock();
                    solution = solveAStar(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC; //每个计时单位为一毫秒

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("A*算法"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("无法找到解决方案！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'D': // 迭代加深搜索
                {
                    clock_t start = clock();
                    solution = solveIDS(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC;

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("迭代加深搜索"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("无法找到解决方案！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'S': // 贪婪最佳优先搜索
                {
                    clock_t start = clock();
                    solution = solveGreedyBestFirst(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC;

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("贪婪最佳优先搜索"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("无法找到解决方案！"), _T("提示"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'R': // 重新生成随机状态
                    initialState = generateRandomState();
                    break;
                }
            }
        }
    }
    else {
        // 如果正在演示，检查是否按了ESC键停止演示
        ExMessage msg;
        if (peekmessage(&msg, EM_KEY)) {
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                solving = false;
                solutionStep = 0;
            }
        }
    }
}

// 主函数
int main()
{
    startup();  // 初始化函数，仅执行一次

    while (running)  // 一直循环
    {
        show();  // 进行绘制
        updateWithoutInput();  // 与输入无关的更新
        updateWithInput();  // 与输入有关的更新
    }

    // 关闭图形窗口
    EndBatchDraw();
    closegraph();
    return 0;
}