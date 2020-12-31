#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int promising(int i, int col[]); //노드가 유망한지 확인하는 함수


//montecarlo algorithm으로 n queens problem의 수행시간 추정하는 함수
int estimate_n_queens(int n, int col[]) {
	int i, j, z;
	int m, mprod, numnodes;
	int prom_children[8];
	int index = 0;
	int flag = 0;

	i = 0;
	numnodes = 1;
	m = 1; //level i에 있는 노드의 유망한 자식노드 개수의 평균 추정치
	mprod = 1;

	while (m != 0 && i != n) {
		//유망한 자식노드가 없을때까지 반복
		//n의 크기만큼 index 탐색
		mprod = mprod * m; //각 level에서 만들어진 m0 m1...mi-1 표현
		numnodes = numnodes + mprod * n; //상태공간트리에서 노드의 개수
		i++; //index 증가
		m = 0;

		for (z = 1; z <= n; z++) //자식노드의 집합 공집합으로 초기화
			prom_children[z] = 0;

		for (j = 1; j <= n; j++) {
			col[i] = j; //i row에서 여왕말이 놓여있는 column을 j로 초기화

			if (promising(i, col)) { //primising한 경우
				m++;

				for (z = 1; z <= n; z++)
					if (prom_children[z] == 0) {
						prom_children[z] = j;
						break;
					}
			}
		}

		if (m != 0) { //유망한 자식노드가 없는 경우
			for (z = 1; z <= n; z++) {
				if (prom_children[z] != 0)
					index++;
			}

			srand((unsigned)time(NULL));
			flag = (rand() % index) + 1; //자식의 index 범위내에서 random하게 노드 선택

			j = prom_children[flag]; //자식노드 중에서 random하게 하나 선택

			col[i] = j;
			index = 0;
		}
	}
	return numnodes; //점검한 노드 총 개수 return
}


//노드가 유망한지 확인하는 함수
int promising(int i, int col[]) {
	int k; //index
	int bSwitch; //참=1(유망) 거짓=0(유망X)

	k = 1;
	bSwitch = 1; //유망한 것으로 초기화

	while (k < i && bSwitch) { //i번째 row에 있는 여왕말을 위협하는 여왕말이 있는지 검사
		if (col[i] == col[k] || abs(col[i] - col[k]) == i - k)
			// 같은 column 또는 같은 diagonal
			bSwitch = 0;
		k++;
	}
	return bSwitch;
}

int main(void) {
	int i, n, * col;
	int mon[20]; //평균치로 추정치를 구하기 위해 monte carlo 기법을 반복하는 횟수
	int avg = 0;

	printf("n-Queens problem에서 n의 크기\n");
	scanf_s("%d", &n); //n queens problem의 행과 열의 수
	getchar();

	col = (int*)malloc(sizeof(int) * n); //col 배열 n의 크기로 동적할당

	for (i = 0; i < 20; i++) { //몬테칼로 기법을 수행할 때 마다의 추정치 기록
		printf("%d : %d\n", i + 1, mon[i] = estimate_n_queens(n, col));
		avg += mon[i];
		Sleep(1);
	}

	printf("평균값 : %d\n", avg / 20); //몬테칼로 기법 추정치들의 평균값 계산
	//queens(0,n,col);

	free(col);

	return 0;
}