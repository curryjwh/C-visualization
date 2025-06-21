#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>  // EasyXͼ�ο�
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

// ȫ�ֱ�������
const int WINDOW_WIDTH = 600; //����x
const int WINDOW_HEIGHT = 800; //����y
const int BOARD_SIZE = 300;
const int BOARD_X = 150; //�������ϽǺ�����
const int BOARD_Y = 100; //�������Ͻ�������
const int TILE_SIZE = BOARD_SIZE / 3; //ÿһ�������ĺ�����
const int STATS_Y = BOARD_Y + BOARD_SIZE + 50;
const int INSTRUCTIONS_Y = STATS_Y + 30;  // ָ��˵������ʼY����
const int INSTRUCTIONS_LINE_HEIGHT = 30;  // ָ��˵���и�

// �ƶ������ϡ��ҡ��¡���
const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

// �����������״̬��ʾ
struct State {
    int board[3][3];      // 3x3����
    int zeroRow, zeroCol; // �ո�(0)��λ��
    int depth;            // ��ǰ�������
    int f;                // A*�㷨�Ĺ��ۺ���f = g + h��gΪ��ȣ�hΪ�����پ���

    //�������ҵ����ս��״̬������Ҫ֪���ӳ�ʼ״̬��Ŀ��״̬�������ƶ�����
    //�ҵ��������ʱ��ͨ�����������Щ���ڵ�ָ�룬�����ؽ��ӳ�ʼ״̬��Ŀ��״̬������·����Ȼ���ٽ�·����ת�Ի������Ľ���������С�
    State* parent;        // ���ڵ�ָ�룬���ڻ���·��


    // Ĭ�Ϲ��캯��
    State() : depth(0), f(0), parent(nullptr) {
        //��ʼ�����̣�������Ԫ����0
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = 0;
            }
        }
        zeroRow = zeroCol = 0;
    }

    // �������캯�������ڸ���һ��״̬
    State(const State& other) : depth(other.depth), f(other.f),
        parent(other.parent), zeroRow(other.zeroRow), zeroCol(other.zeroCol) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = other.board[i][j];
            }
        }
    }

    // ���ص��������
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

    // Ϊ���ȶ�����ӱȽ������������A*�㷨
    bool operator<(const State& other) const {
        return f > other.f;  // ʹfֵС���и������ȼ�
    }
};

// ��ϣ����������unordered_set��unordered_map
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

// ��Ϸ״̬����
State goalState;
State initialState;
vector<State> solution; // �洢�������е�ÿһ��
bool running = true; // �����Ƿ��ڿ�ʼִ�н׶�
bool solving = false; // �����Ƿ������״̬
int solutionStep = 0; // ��¼���Ĳ���
TCHAR methodName[100] = _T("");
double time_taken = 0.0; // ��¼��ͬ�㷨��ִ��ʱ��

// ���������پ���
int calculateManhattanDistance(const State& state) {
    int distance = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int value = state.board[i][j];
            if (value != 0) {
                // ���㵱ǰ���ֵ�Ŀ��λ�õ������پ���
                int targetRow = (value - 1) / 3;
                int targetCol = (value - 1) % 3;
                distance += abs(i - targetRow) + abs(j - targetCol);
            }
        }
    }
    return distance;
}

// ���״̬�Ƿ�ɽ�
bool isSolvable(const State& state) {
    // ����ά����תΪһά���飬������0
    // �ڼ���������ʱ,ֻ����ʵ�����ֵ����˳�򣬿ո��ƶ���Ӱ������������
    vector<int> reverseBoard;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state.board[i][j] != 0) {
                reverseBoard.push_back(state.board[i][j]);
            }
        }
    }

    // ����������(ָ�����������������Ե�����)
    // ���һ���ϴ�������ڽ�С����ǰ�棬��ô������������һ�������
    int inversions = 0;
    for (int i = 0; i < reverseBoard.size(); ++i) {
        for (int j = i + 1; j < reverseBoard.size(); ++j) {
            if (reverseBoard[i] > reverseBoard[j]) {
                inversions++;
            }
        }
    }
    // ����3x3�İ��������⣬��ʼ״̬��Ŀ��״̬����������ż����ͬʱ�ɽ�
    // Ŀ��״̬��������Ϊ0
    return inversions % 2 == 0;
}

// ��������ĳ�ʼ״̬
State generateRandomState() {
    State state;
    vector<int> numbers = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    srand(time(0));
    do {
        // ϴ���㷨
        // ����������һ��Ԫ�ؿ�ʼ������ǰÿ�����ѡ��ǰ���һ��Ԫ��(������ǰԪ��)���н���
        for (int i = 8; i > 0; --i) {
            int j = rand() % (i + 1);  // ��[0,i]�����ѡ��һ��λ��
            swap(numbers[i], numbers[j]);  
        }
        // ����ʼ����
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
    } while (!isSolvable(state)); // ȷ�����ɵ�״̬�ǿɽ��
    return state;
}

// A*�㷨���
vector<State> solveAStar(const State& initialState) {
    // ���ȶ���fֵС�����ȼ��ϸ�
    priority_queue<State> openList;
    unordered_set<State> closedList; // ȥ�أ���ֹ�ظ���չ
    unordered_map<State, State*> stateMap; // �洢״̬֮��ĸ��ӹ�ϵ���������ҵ�Ŀ����ؽ�·����������״̬��ֵ��ָ��״̬��ָ��

    State start = initialState;
    start.f = start.depth + calculateManhattanDistance(start);
    openList.push(start);

    while (!openList.empty()) {
        State current = openList.top();
        openList.pop();

        // ����Ƿ�ﵽĿ��״̬
        if (calculateManhattanDistance(current) == 0) {
            // �洢�ӳ�ʼ״̬��Ŀ��״̬������·��
            vector<State> path;
            // ��ʼ��ָ�룬ָ��ǰ״̬����Ŀ��״̬��
            State* currentPtr = &current; //ָ�� State ���͵�ָ��
            while (currentPtr != nullptr) {
                path.push_back(*currentPtr);
                if ((*currentPtr).parent == NULL) break;
                currentPtr = stateMap[*currentPtr];
            }
            reverse(path.begin(), path.end());
            return path; //�õ���·���Ǵ�Ŀ��״̬����ʼ״̬����Ҫ��תΪ�ӳ�ʼ״̬��Ŀ��״̬
        }

        // ����ǰ״̬�����ѷ��ʼ���
        closedList.insert(current);

        // �����ĸ�������ƶ�
        for (int i = 0; i < 4; ++i) {
            int newRow = current.zeroRow + dx[i];
            int newCol = current.zeroCol + dy[i];

            // ����Ƿ�Խ��
            if (newRow < 0 || newRow >= 3 || newCol < 0 || newCol >= 3) {
                continue;
            }

            // ������״̬
            State next = current; //��״̬next�̳�current����������
            //ģ��ո��ƶ�����
            next.board[next.zeroRow][next.zeroCol] = next.board[newRow][newCol];
            next.board[newRow][newCol] = 0;
            next.zeroRow = newRow;
            next.zeroCol = newCol;
            next.depth = current.depth + 1;
            next.f = next.depth + calculateManhattanDistance(next);
            next.parent = &current;

            // �����״̬�ѱ����ʣ�������
            if (closedList.find(next) != closedList.end()) {
                continue;
            }

            // ���游�ڵ���Ϣ���ڻ���
            stateMap[next] = new State(current); // ����һ���µ�State������Ϊcurrent״̬�ĸ��������ҷ��ظö����ָ��
            // ���뿪���б�
            openList.push(next);
        }
    }
    // ����޽⣬���ؿ�·��
    return vector<State>();
}



// �����������(DLS)
bool DLS(const State& state, int limit, vector<State>& path, unordered_set<State>& visited) {
    // ����ﵽĿ��״̬
    if (calculateManhattanDistance(state) == 0) {
        path.push_back(state);
        return true;
    }

    // ����ﵽ�������
    if (state.depth >= limit) {
        return false;
    }

    // ��ǵ�ǰ״̬Ϊ�ѷ���
    visited.insert(state);

    // �����ĸ�������ƶ�
    for (int i = 0; i < 4; ++i) {
        int newRow = state.zeroRow + dx[i];
        int newCol = state.zeroCol + dy[i];

        // ����Ƿ�Խ��
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

        // �����״̬�ѱ����ʣ�������
        if (visited.find(next) != visited.end()) {
            continue;
        }

        // �ݹ�����
        // ��һ��·���ѵ��׺�Ҫ������ܴﵽĿ��״̬���ַ������path�ϵ�ÿһ��state��Ȼ����һpush_back
        if (DLS(next, limit, path, visited)) {
            path.push_back(state);
            return true;
        }
    }

    return false;
}
// ������������(IDS)
vector<State> solveIDS(const State& initialState, int maxDepth = 40) {
    vector<State> path;

    // �����1��ʼ���������������
    for (int limit = 1; limit <= maxDepth; ++limit) {
        unordered_set<State> visited;
        if (DLS(initialState, limit, path, visited)) {
            reverse(path.begin(), path.end()); //����������ռ�����·���ĳ�����
            return path;
        }
    }

    // ��������������Ҳ����⣬���ؿ�·��
    return vector<State>();
}



// ̰�������������
vector<State> solveGreedyBestFirst(const State& initialState) {
    priority_queue<State> openList;
    unordered_set<State> closedList;
    unordered_map<State, State*> stateMap; // ȥ�ء�����

    State start = initialState;
    start.f = calculateManhattanDistance(start); // ��ʹ�������پ�����Ϊ����ʽ����
    openList.push(start);

    while (!openList.empty()) {
        State current = openList.top();
        openList.pop();

        if (calculateManhattanDistance(current) == 0) {
            vector<State> path;
            State* currentPtr = &current;
            while (currentPtr != nullptr) {
                path.push_back(*currentPtr);
                if ((*currentPtr).parent == NULL) break;
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
//��Ȼ̰���������������ִ��ʱ���������ƣ������ܱ�֤�ҵ����Ž⣬��ⲽ���϶�����Ϊ���ܻ�����ֲ����ţ�������ŵ�·����
//ʹ����������������� DLS �����ҽ��ҵ�Ŀ��״̬ʱ��·����ͨ���ݹ����ջ���򹹽��ģ��ݹ���ñ�����γ���һ����Ȼ�Ļ��ݻ��ƣ�������Ҫ�� A* �㷨��̰�����������������ά��һ�������ĸ��ڵ�ӳ��



// ��Ӧ�Ⱥ���(�����پ���ĵ���)
double fitness(const State& state) {
    return 1.0 / (1 + calculateManhattanDistance(state));
}

// ѡ�����(���̶�ѡ��)
// ��Ӧ��Խ�ߣ���ѡ�еĸ���Խ��
State wheelSelection(const vector<State>& population, const vector<double>& fitnessValues) {
    // ������Ⱥ�����и��������Ӧ��
    double  totalfitness = 0.0;
    for (double f : fitnessValues) {
        totalfitness += f;
    }

    //����һ��0������Ӧ��֮��������
    double r = (double)rand() / RAND_MAX * totalfitness; //r���൱���������������תָ���ָ����ָ��λ��
    double sum = 0.0; //��ʼ��һ���ۼӱ���
    //������Ⱥ�е�ÿ������
    for (int i = 0; i < population.size(); i++) {
        //��Ӧ��Խ�ߵĸ��壬���ۼӺͶ�Ӧ�����䷶ΧԽ�󣬱�ѡ�еĸ���Ҳ��Խ��
        sum += fitnessValues[i];
        if (sum >= r) {
            return population[i];
        }
    }
    return population.back();
}

// ���״̬�Ƿ���Ч
bool isValidState(const State& state) {
    vector<int> numbers(9, 0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = state.board[i][j];
            if (num < 0 || num>8) return false;
            numbers[num]++;
        }
    }
    for (int num : numbers) {
        if (num != 1) return false;
    }
    return true;
}


//�������
//���������Ӵ�����
pair<State, State> crossover(const State& parent1, const State& parent2, double crossoverProbability) {
    // ����һ�� 0 �� 1 ֮��������
    double randomValue = (double)rand() / RAND_MAX;
    if (randomValue < crossoverProbability) {
        State child1 = parent1;
        State child2 = parent2;
        int crossPoint = rand() % 9; // ���ѡ��һ������㣬��Χ�� 0 �� 8
        int index = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (index >= crossPoint) {
                    swap(child1.board[i][j], child2.board[i][j]);
                }
                index++;
            }
        }
        //���¿ո�λ��
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (child1.board[i][j] == 0) {
                    child1.zeroRow = i;
                    child1.zeroCol = j;
                }
                if (child2.board[i][j] == 0) {
                    child2.zeroRow = i;
                    child2.zeroCol = j;
                }
            }
        }
        if (!isValidState(child1)) {
            child1 = parent1;
        }
        if (!isValidState(child2)) {
            child2 = parent2;
        }
        return make_pair(child1, child2);
    }
    else
        return make_pair(parent1, parent2);
}

//�������
//���ѡ�������������ֵ�λ�ã��������ǵ�ֵ
void mutate(State& state) {
    // ���ѡȡ����λ��
    int index1 = rand() % 9;
    int index2 = rand() % 9;
    // ����������λ���������е��к���
    int row1 = index1 / 3;
    int col1 = index1 % 3;
    int row2 = index2 / 3;
    int col2 = index2 % 3;
    if (state.board[row1][col1] != 0 && state.board[row2][col2] != 0) {
        swap(state.board[row1][col1], state.board[row2][col2]);
        // ���¿�λλ��
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (state.board[i][j] == 0) {
                    state.zeroRow = i;
                    state.zeroCol = j;
                }
            }
        }
    }
}


//�Ŵ��㷨���
//populationsize��ʾ��Ⱥ��С,maxgenerations��ʾ����������
vector<State> solveGA(const State& InitialState,int populationSize,int maxgenerations) {
    unordered_map<State, State*> stateMap; // ��Ӹ��ڵ�ӳ��
    //��ʼ����Ⱥ
    vector<State> population(populationSize);
    for (int i = 0; i < populationSize; i++) {
        population[i] = generateRandomState();
    }
    //����ָ�������ĵ���
    for (int i = 0; i < maxgenerations; i++) {
        //�洢������Ӧ��
        vector<double> fitnessValues(populationSize);
        for (int i = 0; i < populationSize; i++) {
            fitnessValues[i] = fitness(population[i]);
        }
        //�洢�����ɵ���Ⱥ
        vector<State> newpopulation;
        // ÿ�����������Ӵ����壬����ѭ������Ϊ 2
        for (int i = 0; i < populationSize; i += 2) {
            State parent1 = wheelSelection(population, fitnessValues);
            State parent2 = wheelSelection(population, fitnessValues);
            //���н������
            pair<State, State> children = crossover(parent1, parent2,0.8);
            State child1 = children.first;
            State child2 = children.second;
            child1.parent = &parent1;
            child2.parent = &parent2;
            stateMap[child1] = new State(parent1);
            stateMap[child2] = new State(parent2);
            //�趨���ʶ����ɵ������Ӵ�������б������
            if ((double)rand() / RAND_MAX < 0.05) {
                State originalchild1 = child1;
                mutate(child1);
                child1.parent = &originalchild1;
                stateMap[child1] = new State(originalchild1);
            }
            if ((double)rand() / RAND_MAX < 0.05) {
                State originalchild2 = child2;
                mutate(child2);
                child2.parent = &originalchild2;
                stateMap[child2] = new State(originalchild2);
            }
            newpopulation.push_back(child1);
            newpopulation.push_back(child2);
        }
        population = newpopulation; // ������Ⱥ
        // �����Ⱥ���Ƿ��и���ﵽĿ��״̬
        for (State& current : population) {
            if (calculateManhattanDistance(current) == 0) {
                vector<State> path;
                State* currentPtr = &current;
                while (currentPtr != NULL) {
                    path.push_back(*currentPtr);
                    if ((*currentPtr).parent == NULL) break;
                    currentPtr = stateMap[*currentPtr];
                }
                reverse(path.begin(), path.end());
                return path;
            }
        }
    }
    return vector<State>();
}




// ��ʼ������
void startup()
{
    // ��ʼ��ͼ�δ���
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(WHITE);
    cleardevice();
    BeginBatchDraw();

    // ����Ŀ��״̬ (123/456/780)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            goalState.board[i][j] = i * 3 + j + 1;
        }
    }
    goalState.board[2][2] = 0;
    goalState.zeroRow = 2;
    goalState.zeroCol = 2;

    // ������ʼ״̬
    initialState = generateRandomState();
}

// �������̺���Ϸ״̬
void show()
{
    cleardevice();
    // �������̱���
    setfillcolor(RGB(240, 240, 240));
    fillrectangle(BOARD_X, BOARD_Y, BOARD_X + BOARD_SIZE, BOARD_Y + BOARD_SIZE);

    // ��������
    settextstyle(TILE_SIZE / 3, 0, _T("΢���ź�")); //���ָ߶ȡ�����Ӧ���
    settextcolor(BLACK);

    // ���Ʊ���
    TCHAR title[100];
    // ��ʼҳ��
    if (!solving) {
        wcscpy(title, _T("���������� - ��ʼ״̬")); //��һ�����ַ��ַ������Ƶ���һ�����ַ�����
    }
    // ѡ�񷽷�����ʼ����ҳ��
    else {
        swprintf_s(title, _T("���������� - %s"), methodName); //�Ѹ�ʽ���Ŀ��ַ�����д�뵽һ�����ַ�������
    }
    outtextxy(WINDOW_WIDTH / 2 - textwidth(title) / 2, 50, title);

    // ��ȡ��ǰҪ��ʾ��״̬
    State currentState;
    if (solving && solutionStep < solution.size()) {
        currentState = solution[solutionStep];
    }
    else {
        currentState = initialState;
    }

    // ����ÿ������
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = BOARD_X + j * TILE_SIZE;
            int y = BOARD_Y + i * TILE_SIZE;

            // ���Ʒ��񱳾��ͱ߿�
            setlinecolor(BLACK);
            rectangle(x, y, x + TILE_SIZE, y + TILE_SIZE);

            if (currentState.board[i][j] != 0) {
                setfillcolor(RGB(100, 180, 255));
                // ʹ��͸����ʽ���Ʒ���
                setbkmode(TRANSPARENT);
                fillrectangle(x, y, x + TILE_SIZE, y + TILE_SIZE);
                // ��������
                TCHAR numStr[2];
                swprintf_s(numStr, _T("%d"), currentState.board[i][j]);
                int numWidth = textwidth(numStr);
                int numHeight = textheight(numStr);
                //���־��д���
                outtextxy(x + TILE_SIZE / 2 - numWidth / 2, y + TILE_SIZE / 2 - numHeight / 2, numStr);
            }
        }
    }

    // ���Ʋ�����Ϣ
    if (solving) {
        TCHAR stepsInfo[50];
        swprintf_s(stepsInfo, _T("����ɲ���: %d / %d (��ʱ: %.2f��)"),
            solutionStep, solution.size() - 1, time_taken);
        outtextxy(WINDOW_WIDTH / 2 - textwidth(stepsInfo) / 2, STATS_Y, stepsInfo);
    }

    // ����ǳ�ʼ״̬����ʾ����˵��
    if (!solving) {
        //ÿ��Ԫ�ض���һ��ָ���� TCHAR �ַ�����ָ��
        const TCHAR* instructions[10] = {
            _T("����˵��:"),
            _T("A - ʹ��A*�㷨���"),
            _T("D - ʹ���������+���������������"),
            _T("S - ʹ��̰����������������"),
            _T("W - ʹ���Ŵ��㷨���"),
            _T("R - �������������ʼ״̬"),
            _T("ESC - �˳�����")
        };

        
        // ���ñ�������
        setfillcolor(RGB(245, 245, 245));
        int maxWidth = 0;
        // ��������������ÿ��
        for (int i = 0; i < 7; ++i) {
            int width = textwidth(instructions[i]);
            if (width > maxWidth) {
                maxWidth = width;
            }
        }

        // ����˵������
        int instructionsX = WINDOW_WIDTH / 2 - maxWidth / 2 - 20;
        int instructionsWidth = maxWidth + 40;
        fillrectangle(instructionsX, INSTRUCTIONS_Y - 10,
            instructionsX + instructionsWidth,
            INSTRUCTIONS_Y + 8 * INSTRUCTIONS_LINE_HEIGHT);

        // ����˵������
        for (int i = 0; i < 7; ++i) {
            outtextxy(WINDOW_WIDTH / 2 - textwidth(instructions[i]) / 2,
                INSTRUCTIONS_Y + i * INSTRUCTIONS_LINE_HEIGHT,
                instructions[i]);
        }
    }

    // ˢ����ʾ
    FlushBatchDraw();
}


// �������޹صĸ���
void updateWithoutInput()
{
    // ���������ʾ������
    if (solving && !solution.empty()) {
        solutionStep++;
        Sleep(500);
        // ����Ƿ�������в���
        if (solutionStep >= solution.size()) {
            // ��ʾ��ɣ��ص���ʼ״̬
            solving = false;
            solutionStep = 0;

            // ��ʾ�����Ϣ
            TCHAR completeMsg[100];
            swprintf_s(completeMsg, _T("�����ɣ��ܲ���: %d"), solution.size() - 1);
            MessageBox(GetHWnd(), completeMsg, _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
        }
    }
}


// �������йصĸ���
void updateWithInput()
{
    // ��������������У������û�����
    if (!solving) {
        // ������Ϣ
        ExMessage msg;
        if (peekmessage(&msg, EM_KEY)) { // ֻ��ȡ������ص���Ϣ
            if (msg.message == WM_KEYDOWN) { // ������̰���������
                switch (msg.vkcode) {
                case VK_ESCAPE: // ESC���˳�
                    running = false;
                    break;

                case 'A': // A*�㷨
                {
                    clock_t start = clock();
                    solution = solveAStar(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC; //ÿ����ʱ��λΪһ����

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("A*�㷨"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("�޷��ҵ����������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'D': // ������������
                {
                    clock_t start = clock();
                    solution = solveIDS(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC;

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("������������"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("�޷��ҵ����������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'S': // ̰�������������
                {
                    clock_t start = clock();
                    solution = solveGreedyBestFirst(initialState);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC;

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("̰�������������"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("�޷��ҵ����������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'W': // �Ŵ��㷨
                {
                    clock_t start = clock();
                    solution = solveGA(initialState,500,5000);
                    clock_t end = clock();
                    time_taken = double(end - start) / CLOCKS_PER_SEC;

                    if (!solution.empty()) {
                        swprintf_s(methodName, _T("�Ŵ��㷨���"));
                        solving = true;
                        solutionStep = 0;
                    }
                    else {
                        MessageBox(GetHWnd(), _T("�޷��ҵ����������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

                case 'R': // �����������״̬
                    initialState = generateRandomState();
                    break;
                }
            }
        }
    }
    else {
        // ���������ʾ������Ƿ���ESC��ֹͣ��ʾ
        ExMessage msg;
        if (peekmessage(&msg, EM_KEY)) {
            if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
                solving = false;
                solutionStep = 0;
            }
        }
    }
}

// ������
int main()
{
    startup();  // ��ʼ����������ִ��һ��

    while (running)  // һֱѭ��
    {
        show();  // ���л���
        updateWithoutInput();  // �������޹صĸ���
        updateWithInput();  // �������йصĸ���
    }

    // �ر�ͼ�δ���
    EndBatchDraw();
    closegraph();
    return 0;
}