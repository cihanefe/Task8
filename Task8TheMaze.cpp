#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <windows.h>

using namespace std;

// Default values
int m = 10, n = 15;
int M = m * 2 + 1, N = n * 2 + 1;
int agaX = 1, agaY = 1;
int startX, startY;
int save = 0;
int q = 1;

void ClearScreen()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void displayMaze(int M, int N, char** maze) {
    ClearScreen();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == agaY && j == agaX) {
                cout << "o ";
            }
            else {
                cout << maze[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void Player(int M, int N, char** maze) {

    if (GetAsyncKeyState(VK_RIGHT) < 0) { //Player controller
        agaX++;
        displayMaze(M, N, maze);
    }
    else if (GetAsyncKeyState(VK_LEFT) < 0) {
        agaX--;
        displayMaze(M, N, maze);
    }
    else if (GetAsyncKeyState(VK_UP) < 0) {
        agaY--;
        displayMaze(M, N, maze);
    }
    else if (GetAsyncKeyState(VK_DOWN) < 0) {
        agaY++;
        displayMaze(M, N, maze);
    }
    else if (GetAsyncKeyState(VK_SPACE) < 0 && save <= 5) {
        save++;
        displayMaze(M, N, maze);
        Sleep(100);
    }

    if (save == 1) {
        startY = agaY;
        startX = agaX;
        maze[agaY][agaX] = 'S';
        save = 2;
    }
    else if (save == 3) {
        maze[agaY][agaX] = 'E';
        save = 4;
    }
}

void Solve(int M, int N, char** maze) {
    ClearScreen();


    if (maze[agaY][agaX + 1] == 'E' || maze[agaY][agaX - 1] == 'E' || maze[agaY + 1][agaX] == 'E' || maze[agaY - 1][agaX] == 'E') {
    q = 0;
    }

    else if (maze[agaY][agaX + 1] == ' ') {                      //right  
        maze[agaY][agaX] = '+';
        agaX++;
        displayMaze(M, N, maze);
        Sleep(100);
    }

    else if (maze[agaY + 1][agaX] == ' ') {                     //down
        maze[agaY][agaX] = '+';
        agaY++;
        displayMaze(M, N, maze);
        Sleep(100);
    }

    else if (maze[agaY][agaX - 1] == ' ') {                 //left
        maze[agaY][agaX] = '+';
        agaX--;
        displayMaze(M, N, maze);
        Sleep(100);
    }

    else if (maze[agaY - 1][agaX] == ' ') {                 //up
        maze[agaY][agaX] = '+';
        agaY--;
        displayMaze(M, N, maze);
        Sleep(100);
    }
    //çıkmaz yol (ingilizcesini bilmiyorum)("blocked path" olabilirdi aslında)
    else if (maze[agaY][agaX + 1] == '+') {
        maze[agaY][agaX] = '.';
        agaX++;
        displayMaze(M, N, maze);
    }
    else if (maze[agaY + 1][agaX] == '+') {
        maze[agaY][agaX] = '.';
        agaY++;
        displayMaze(M, N, maze);
    }
    else if (maze[agaY][agaX - 1] == '+') {
        maze[agaY][agaX] = '.';
        agaX--;
        displayMaze(M, N, maze);
    }
    else if (maze[agaY - 1][agaX] == '+') {
        maze[agaY][agaX] = '.';
        agaY--;
        displayMaze(M, N, maze);
    }
}

int getIdx(int x, int y, vector < pair<int, pair<int, int> > > cell_list) {
    for (int i = 0; i < cell_list.size(); i++)
    {
        if (cell_list[i].second.first == x && cell_list[i].second.second == y)
            return cell_list[i].first;
    }
    cout << "getIdx() couldn't find the index!" << endl;
    return -1;
}

void createMaze(int M, int N, char** maze) {

    for (int i = 0; i < M; i++) {
        maze[i] = new char[N];
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (!(i & 1) || !(j & 1))
                maze[i][j] = '#';
            else
                maze[i][j] = ' ';

        }
    }
    for (int i = 1; i < M; i += 2) {
        for (int j = 1; j < N; j += 2) {
            maze[i][j] = ' ';
        }

    }

    vector < pair<int, pair<int, int> > > cell_list;
    vector <bool> visited(m * n, false);
    stack<pair<int, pair<int, int> > > m_stack;
    random_device rdev;
    mt19937 rng(rdev());
    uniform_int_distribution<mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < M; i += 2) {
        for (int j = 1; j < N; j += 2) {
            cell_list.push_back(make_pair(k, make_pair(i, j)));
            k++;
        }
    }

    int randIdx = dist100(rng) % m * n;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    // Algo
    while (nVisited < m * n) {

        vector <int> neighbours;
        // North
        if (m_stack.top().second.first > 1) {
            if (maze[m_stack.top().second.first - 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < N - 2) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]) {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < M - 2) {
            if (maze[m_stack.top().second.first + 2][m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(2);
            }
        }
        // West
        if (m_stack.top().second.second > 1) {
            if (maze[m_stack.top().second.first + 0][m_stack.top().second.second - 2] &&
                !visited[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]) {
                neighbours.push_back(3);
            }
        }
        // Neighbours available?
        if (!neighbours.empty()) {
            // Choose a random direction
            int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
            // Create a path between this cell and neighbour
            switch (next_cell_dir) {
            case 0: // North
                maze[m_stack.top().second.first - 1][m_stack.top().second.second + 0] = ' ';
                m_stack.push(cell_list[getIdx(m_stack.top().second.first - 2, m_stack.top().second.second + 0, cell_list)]);
                break;
            case 1: // East
                maze[m_stack.top().second.first + 0][m_stack.top().second.second + 1] = ' ';
                m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second + 2, cell_list)]);
                break;
            case 2: // South
                maze[m_stack.top().second.first + 1][m_stack.top().second.second + 0] = ' ';
                m_stack.push(cell_list[getIdx(m_stack.top().second.first + 2, m_stack.top().second.second + 0, cell_list)]);
                break;
            case 3: // West
                maze[m_stack.top().second.first + 0][m_stack.top().second.second - 1] = ' ';
                m_stack.push(cell_list[getIdx(m_stack.top().second.first + 0, m_stack.top().second.second - 2, cell_list)]);
                break;
            }

            visited[m_stack.top().first] = true;
            nVisited++;
        }
        else {
            m_stack.pop();
        }
    }
}

int main() {
    char** maze = new char* [M];
    
    createMaze(M, N, maze);
    displayMaze(M, N, maze);

    while (save < 5) {
        Player(M, N, maze);
        Sleep(50);
    }
    agaY = startY;
    agaX = startX;
    while (q == 1) {
        Solve(M, N, maze);
    }
    Sleep(3000);
    return 0;
}