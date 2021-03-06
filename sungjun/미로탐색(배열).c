#include <stdio.h>
#include <limits.h>
#include "point.h"
#include "stack.h"
#include "deque.h"
#include "console.h"

#define MAX 12

Point moveNESW(Point point, int dir);
void dfsRecursion(int maze[][MAX], Point point);
void dfsStack(int maze[][MAX], Point point);
void bfsQueue(int maze[][MAX], Point point);
void shortestWay(int maze[][MAX], int depth[][MAX], Point point);
void printDepth(int depth[][MAX]);
void dijkstra(int maze[][MAX], Point point);
void initDistance(int distance[][MAX]);
void initWeight(int weight[][MAX][MAX][MAX]);
Point findMinDistance(int distance[][MAX], int permanent[][MAX]);
void printDistance(int distance[][MAX]);
void leastWeightWay(int maze[][MAX], Point comeFrom[][MAX], Point point);
void printMaze(int maze[][MAX], Point point);

int main() {

	int maze[MAX][MAX] = { // 0길 1벽 2갔던곳 3막혀서돌아간곳 4외벽 5도착 6최단거리 9출구
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 1, 9, 1, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4 },
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
	};

	Point point = { 1, 1 }; // 탐색 출발 위치

	set();

	//dfsRecursion(maze, point); // 재귀호출을 이용한 방법
	//dfsStack(maze, point); // 스택을 이용한 방법
	//bfsQueue(maze, point); // 큐를 이용한방법
	//dijkstra(maze, point); // 다익스트라 알고리즘을 이용한 방법
	//A스타
	//플로이드
	
}

Point moveNESW(Point point, int dir) { // dir가 0북 1동 2남 3서 point형 반환 

	if (dir == 0) 
		point.x--;
	else if (dir == 1) 
		point.y++;
	else if (dir == 2) 
		point.x++;
	else if (dir == 3) 
		point.y--;

	return point;
}

void dfsRecursion(int maze[][MAX], Point point) { // point위치 재귀적으로 탐색하기(DFS)

	int dir = 0;
	int x = point.x;
	int y = point.y;

	if (maze[x][y] == 9) { // 탐색한 곳이 출구면 깃발꽂고 끝낸다
		maze[x][y] = 5;
		printMaze(maze, point);
		gotoxy(0, MAX);
		exit(1);
	}

	if (maze[x][y] == 0) { // 탐색한곳이 길이면
		maze[x][y] = 2; // 간길로 표시하고
		printMaze(maze, point);

		for (dir = 0; dir < 4; dir++) { // 재귀적으로 북동남서를 탐색한다
			point = moveNESW(point, dir);
			dfsRecursion(maze, point);
			point.x = x;
			point.y = y;
		}

		maze[x][y] = 3; // point에서 북동남서 전부 재귀호출 탐색이 끝나게 되면 돌아나가는 길임을 표시한다
		printMaze(maze, point);
	}
}

void dfsStack(int maze[][MAX], Point point) { // point위치부터 스택을 이용하여 탐색하기(DFS)

	int dir = 0, canMove = 0, x = 0, y = 0;
	StackType *s = makeStack(); // 스택 생성

	push(s, point); // 현재 위치 push
	while (!isEmptyStack(s)) { // 스택이 비어있을때 까지 반복
		canMove = 0;
		x = point.x;
		y = point.y;

		if (maze[x][y] == 9) { // 현재 위치한 곳이 출구면 깃발꽂고 끝낸다
			maze[x][y] = 5;
			printMaze(maze, point);
			gotoxy(0, MAX);
			return;
		}

		else if (maze[x][y] == 0) { // 현재 위치한곳이 길이면
			maze[x][y] = 2; // 현재위치를 간곳으로 표시한다
			printMaze(maze, point);
		}

		for (dir = 3; dir >= 0; dir--) { // 현재 위치에서
			point = moveNESW(point, dir);
			if (maze[point.x][point.y] == 0 || maze[point.x][point.y] == 9) { // 현재 위치에서 서남동북순으로 검사하여 길이나 출구면 그 위치를 push한다
				push(s, point);
				canMove++; // 움직일 수 있다
			}
			point.x = x;
			point.y = y;
		}
		if (canMove == 0) { // 현재 위치에서 움직일곳이 없다면 돌아나가는 길임을 표시한다
			maze[point.x][point.y] = 3;
			printMaze(maze, point);
			pop(s); // 현재 위치 정보를 pop한다.
		}
		point = peek(s); // 현재 위치는 스택의 top이다
	}

	gotoxy(0, MAX); // 스택이 빌때 까지 출구를 찾지 못했다면 
	printf("출구로 가는 길 없음!\n");
}

void bfsQueue(int maze[][MAX], Point point) { // point위치부터 큐(덱)를 이용하여 탐색하기(BFS)

	int dir = 0, x = 0, y = 0;
	int depth[MAX][MAX] = { 0 }; // 시작점에서 그 셀 위치까지의 간선의 길이 (깊이)
	memset(depth, -1, sizeof(depth)); //깊이 배열을 -1로 모두 초기화
	depth[1][1] = 0; //시작위치의 깊이는 0이다
	DequeType *d = makeDeque(); // 덱 생성

	enqueueFront(d, point); // 현재 위치를 덱 front에 넣는다
	while (!isEmptyDeque(d)) { // 덱이 빌때 까지 반복한다
		x = point.x;
		y = point.y;

		if (maze[x][y] == 9) { // 현재 위치한 곳이 출구면 깃발꽂는다
			maze[x][y] = 5;
			printMaze(maze, point);
			shortestWay(maze, depth, point); // 출발위치에서 출구로 가는 가장짧은 길을 표시해준다
			gotoxy(0, MAX*2);
			return; // 함수 종료
		}

		else if (maze[x][y] == 2) { // 현재 위치한곳이 한번 간길이면 
			dequeueFront(d); // 현재위치인 덱의 헤드를 뺀다
		}

		else if (maze[x][y] == 0) { // 현재 위치한곳이 길이면 
			maze[x][y] = 2; // 현재위치를 간곳으로 표시한다
			printMaze(maze, point);
			dequeueFront(d); // 현재위치인 덱의 헤드를 뺀다

			for (dir = 0; dir <= 3; dir++) { // 현재 위치에서
				point = moveNESW(point, dir);
				if (maze[point.x][point.y] == 0 || maze[point.x][point.y] == 9) { // 현재 위치에서 북동남서순으로 검사하여 길이나 출구면
					enqueueRear(d, point); // 그 위치를 덱의 꼬리에 넣는다
					depth[point.x][point.y] = depth[x][y] + 1; // 북동남서에 위치한 길은 현재위치보다 깊이가 1더 깊다
					printDepth(depth); //깊이 배열을 출력한다
				}
				point.x = x;
				point.y = y;
			}
		}

		point = peekFront(d); // 현재위치는 헤드의 위치이다.
	}
	
	gotoxy(0, MAX*2); // 덱이 빌때 까지 출구를 찾지 못했다면 
	printf("출구로 가는 길 없음!\n");
}

void shortestWay(int maze[][MAX], int depth[][MAX], Point point) { //출발지에 도착지로 가는 최단거리를 거꾸로 탐색하여 표시한다

	int dir = 0, x = 0, y = 0;
	while (point.x != 1 || point.y != 1) { // 현재위치가 출발지가 아니면 반복한다
		x = point.x;
		y = point.y;

		for (dir = 0; dir <= 3; dir++) { // 현재 위치에서
			point = moveNESW(point, dir); // 현재 위치에서 북동남서순으로 검사하여
			if (depth[point.x][point.y] == depth[x][y] - 1) { // 그곳의 깊이가 현재위치보다 하나 작으면
				maze[point.x][point.y] = 6; //그 위치를 최단거리로 표시한다
				printMaze(maze, point);
				break; //for문을 빠져나간다 현재위치가 최단거리로 표시한 위치가 된다
			}
			point.x = x;
			point.y = y;
		}
	}
}

void printDepth(int depth[][MAX]) { // 깊이 배열을 출력해주는 함수

	int i = 0, j = 0;
	gotoxy(0, MAX);
	cursorView(0);

	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++)
			printf("%-3d", depth[i][j]);
		printf("\n");
	}
	cursorView(1);
}

void dijkstra(int maze[][MAX], Point point) { // point위치부터 다익스트라를 이용하여 탐색하기 

	int dir = 0, x = 0, y = 0;
	int distance[MAX][MAX] = { 0 }; // 시작점에서 그 셀 위치까지의 길중 가중치의합이 최저인 값
	int permanent[MAX][MAX] = { 0 }; // 가중치 합이 영구적으로 최저값으로 인식될때 1이 된다
	initDistance(distance); // 각 셀로의 거리를 무한하다고 가정하여 초기화
	int weight[MAX][MAX][MAX][MAX] = { 0 }; // 어느셀에서 어느셀까지의 가중치
	initWeight(weight); // 전부 1로 초기화, 실제 인접행렬은 간선이 있는곳만 가중치가 주어진다. 미로탐색은 상하좌우만 탐색 가능한 특성이 있으므로 못가는곳의 가중치는 무관하게 여기자
	Point comeFrom[MAX][MAX]; // 길을 기억해주는 배열
	int temp = 0;
	weight[9][10][10][10] = 100; 
	weight[10][9][10][10] = 10; 
	weight[2][3][3][3] = 10;
	weight[3][2][3][3] = 10;
	weight[3][4][3][3] = 10;
	weight[4][3][3][3] = 10;
	weight[7][6][6][6] = 5; // 다익스트라의 단점을 보여주는 예
	distance[1][1] = 0; // 시작위치의 거리는 0이다
	permanent[1][1] = 1;

	while (permanent[6][6] == 0) { // 목적지의 거리값이 고정될때까지 
		x = point.x;
		y = point.y;

		if (maze[x][y] == 0) { // 현재 위치한곳이 길이면 
			maze[x][y] = 2; // 현재위치를 간곳으로 표시한다
			printMaze(maze, point);
		}

		for (dir = 0; dir <= 3; dir++) { // 현재 위치에서 북동남서순으로 검사하여
			point = moveNESW(point, dir);
			if ((maze[point.x][point.y] == 0 || maze[point.x][point.y] == 9 || maze[point.x][point.y] == 2) && permanent[point.x][point.y] == 0) { // 검사한곳이 길이거나 출구이거나 갔던곳이고 거리최소값이 고정되지 않았으면
				temp = distance[point.x][point.y];
				distance[point.x][point.y] = min(distance[x][y] + weight[x][y][point.x][point.y], distance[point.x][point.y]); // 검사한곳의 위치로 가는 거리를 계산하여 그거리로 도달하는 최솟값을 갱신한다.
				if (distance[point.x][point.y] != temp) { // 최소 거리로 가는 값이 갱신되었으면
					comeFrom[point.x][point.y].x = x; // 어디서 온건지 기억해준다
					comeFrom[point.x][point.y].y = y;
				}
			}
			point.x = x;
			point.y = y;
		}
		printDistance(distance); // 거리값을 출력한다
		
		point = findMinDistance(distance, permanent); // 현재위치는 임시적 거리값을 가진 정점들 중 최소값을 가진곳으로 한다.
		if (point.x == 0) { // findMinDistance함수에서 출구로 가는 모든 길을 찾아봤지만 출구값이 고정되지 않았으면 0을 받는다
			gotoxy(0, MAX * 2); // 출구를 찾지 못했다면 
			printf("출구로 가는 길 없음!\n");
			return;
		}
		permanent[point.x][point.y] = 1; // 그 거리 값은 아무리 우회해도 현재보다 절대 작은 거리값을 가질 수 없다.
	}

	leastWeightWay(maze, comeFrom, point);
	gotoxy(0, MAX * 2);
}

void initDistance(int distance[][MAX]) {

	int i = 0, j = 0;

	for (i = 0; i < MAX; i++) 
		for (j = 0; j < MAX; j++) 
			distance[i][j] = INT_MAX;
}

void initWeight(int weight[][MAX][MAX][MAX]) {

	int i = 0, j = 0, x = 0, y = 0;

	for (i = 0; i < MAX; i++) 
		for (j = 0; j < MAX; j++) 
			for (x = 0; x < MAX; x++)
				for (y = 0; y < MAX; y++)
					weight[i][j][x][y] = 1;
}

Point findMinDistance(int distance[][MAX], int permanent[][MAX]) {

	int i = 0, j = 0;
	int minTemp = INT_MAX;
	Point min = { 0, 0 };

	for (i = 1; i < MAX - 1; i++) {
		for (j = 1; j < MAX - 1; j++) {
			if (permanent[i][j] == 0 && minTemp > distance[i][j]) {
				minTemp = distance[i][j];
				min.x = i;
				min.y = j;
			}
		}
	}
	return min;
}

void printDistance(int distance[][MAX]) { // 거리 배열을 출력해주는 함수

	int i = 0, j = 0;
	gotoxy(0, MAX);
	cursorView(0);

	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			if (distance[i][j] == INT_MAX)
				printf("%-4d", -1);
			else
				printf("%-4d", distance[i][j]);
		}
		printf("\n");
	}
	cursorView(1);
}

void leastWeightWay(int maze[][MAX], Point comeFrom[][MAX], Point point) { // 도착지부터 출발지까지 최저 가중치가 드는 길을 표시한다

	maze[point.x][point.y] = 5;

	while (point.x != 1 || point.y != 1) { // 현재위치가 출발지가 아니면 반복한다
		
		point = comeFrom[point.x][point.y];
		maze[point.x][point.y] = 6; // 위치를 최단거리로 표시한다
		printMaze(maze, point);
	}
}

void printMaze(int maze[][MAX], Point point) {

	cursorView(0);
	gotoxy(0, 0);

	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			switch (maze[i][j]) {
			case 0: //길이면
				printf("□");
				break;
			case 1: //벽이면
				printf("■");
				break;
			case 2: //간길이면
				printf("▦");
				break;
			case 3: //돌아나가는 길이면
				printf("▣");
				break;
			case 4: //외벽이면
				printf("■");
				break;
			case 5: //깃발꽂은 출구면
				printf("★");
				break;
			case 6: //최단 거리의 길이면
				printf("♥");
				break;
			case 9: //출구면
				printf("☆");
				break;
			}
		}
		printf("\n");
	}

	gotoxy(point.y * 2, point.x);
	cursorView(1); //현재 위치를 커서로 표시한다
	Sleep(5); //대기
}