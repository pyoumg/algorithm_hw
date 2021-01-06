#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

#define NONE -1

typedef struct elm_edge {
	int  vf, vr;  // e = (vf, vr) where vf, vr are vertex array indices
	int  cost;    // edge cost
	bool flag;    // true if the edge is in the SP spanning tree, false otherwise 
	int  fp, rp;  // adj list ptr of forward and reverse (-1 if none)
} edge;

typedef struct elm_vertex {
	int  f_hd, r_hd;	// adj list header(-1 if none)
	int  distance;		// distance from src to this vertex
	int  heap_idx;		// minHeap index
	bool inS;			// true if SP length found(true when move from V-S to S) 
} vertex;

void del(int* total, vertex* V, int *minHeap);
void adjust(int index, vertex* V, int* minHeap);
void push(int* index, int data,//index: minHeap가 어디까지 채워졌는지 나타내는 변수
	vertex *V,
	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function
);


int SPT_Dijkstra(
	int src,	// source vertex index
	// graph structure array
	// 1. the adjacency list structure is the same as PHW02
	// 2. additional fields are added for Dijkstra's algorithm(see .h file)
	int Vnum, vertex *V,	// Vertex array size and the array
	int Enum, edge *E,		// Edge array size and the array

	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function
	//minHeap의 구성은 별도의 함수를 작성한다음 그 함수를 호출해서 이루어진다. 

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** 이 함수를 작성하자 ***
	// 반드시 min-heap을 사용하여 O((n+m)logn) 알고리즘을 구현해야 한다(아니면 trivial한 프로그램임)
	// heap 연산 등 필요한 함수는 자유롭게 작성하여 추가한다.
	// 그러나 global 변수, dynamic array 등은 추가로 사용하지 않는다(실제로 필요 없다)
	int temp, index = 0;//index:minHeap에서 사용되는 index 범위
	int min;//distance[u]가 가장 작은 u

	//초기화
	V[src].distance = 0;//시작점의 distance는 0
	V[src].inS = true;//시작점은 S안에 있음

	//연결된 vertex의 distance구하기
	if (V[src].f_hd != -1) {//연결된 edge가 있으면
		temp = V[src].f_hd;//연결된 edge num
		while (1) {
			V[E[temp].vr].distance = E[temp].cost;
			temp = E[temp].fp;
			if (temp == NONE)//더 연결된 edge가 없으면
				break;
		}
	}
	if (V[src].r_hd != 1) {//연결된 edge가 있으면
		temp = V[src].r_hd;//연결된 edge num
		while (1) {
			V[E[temp].vf].distance = E[temp].cost;
			temp = E[temp].rp;
			if (temp == NONE)//더 연결된 edge가 없으면
				break;
		}
	}

	//초기화 끝
	
	// minheap 구성

	for (int i = 0; i < Vnum; i++) {
		if (i != src) {//Vnum-1번 
			push(&index, i, V, minHeap);
		}
	}
	
	for (int i = 0; i < Vnum - 1; i++) {
		printf("%d ", V[minHeap[i + 1]].distance);
	}
	printf("\n");
	

	for (int i = 0; i < Vnum - 2; i++) {

		min = minHeap[1];//가장 작은 vertex


		printf("min: %d, distance %d\n", min,V[min].distance);
		V[min].inS = true;
		del(&index,V,minHeap);//heap에서 제거



		if (V[min].f_hd != -1) {//연결된 edge가 있으면
			temp = V[min].f_hd;//연결된 edge num
			printf("temp:%d\n", temp);
			while (1) {
				if (V[E[temp].vr].inS == false && V[min].distance + E[temp].cost < V[E[temp].vr].distance) {
					

					printf("distance,%d %d %d", V[min].distance, E[temp].cost, V[E[temp].vr].distance);
					V[E[temp].vr].distance = V[min].distance + E[temp].cost;//update distance
					
					adjust(E[temp].vr, V, minHeap);

				}
				temp = E[temp].fp;//연결된 edge num
				if (temp == NONE)//더 연결된 edge가 없으면
					break;
			}
		}

		if (V[min].r_hd != -1) {//연결된 edge가 있으면
			temp = V[min].r_hd;//연결된 edge num
			printf("temp:%d\n", temp);

			while (1) {
				if (V[E[temp].vf].inS == false  && V[min].distance + E[temp].cost < V[E[temp].vf].distance) {
					printf("distance,%d %d %d", V[min].distance, E[temp].cost, V[E[temp].vf].distance);
					V[E[temp].vf].distance = V[min].distance + E[temp].cost;//update distance
					adjust(E[temp].vf, V, minHeap);
				}
				temp = E[temp].rp;
				if (temp == NONE)//더 연결된 edge가 없으면
					break;
			}
		}


	}
	//Dijkstra 끝

	for (int i = 0; i < Vnum; i++)
		printf("%d\n", V[i].distance);


	//tree찾기(back tracing)
	/*
	for (int i = 0; i < Vnum - 2; i++) {
		temp=
	}
	*/

	return treeCost;
}

void del(int* total, vertex* V, int *minHeap) {
	//현재 minHeap는 1~*total까지 채워져있는 상태
	//minHeap[1]를 제거한 후 minHeap상태를 유지한다.
	int temp,parent=1,child=2;
	V[minHeap[1]].heap_idx = NONE;//기존에 index1이었던 원소는 heap에서 삭제됨
	temp = minHeap[(*total)--];


	
	while (child <= *total) {
		if ((child < *total) && (minHeap[child] > minHeap[child + 1])) {   
			child++;//left child/right child 결정
		}
		if (temp <= minHeap[child]) 
			break;
		minHeap[parent] = minHeap[child];
		parent = child;
		child *= 2;
	}
	minHeap[parent] = temp;

	return;
}



void push(int* index, int data,//index: minHeap가 어디까지 채워졌는지 나타내는 변수
	vertex *V,
	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function
) {
	int child;
	
	child = ++(*index);//minHeap는 1부터 시작,index도 1 증가해야함
	
	while (V[data].distance < V[child / 2].distance&&child>1) {
		minHeap[child] = minHeap[child / 2];
		//부모 노드에 있던게 자식 노드의 값이 된다.
		V[minHeap[child/2]].heap_idx = child;
		child /= 2;

	}
	minHeap[child] = data;//해당하는 자리에 넣기
	printf("push child,data: %d %d\n", child,data);
	V[data].heap_idx = child;


	return;
}

void adjust(int index,vertex* V,int* minHeap) {//distance값이 수정되었기 때문에 minheap도 수정되어야 한다.
	int child = V[index].heap_idx;
	printf("adj index,child: %d %d\n", index,child);
	while (V[index].distance < V[child / 2].distance&&child>1) {
		minHeap[child] = minHeap[child / 2];//부모 노드에 있던게 자식 노드의 값이 된다.
		V[minHeap[child/2]].heap_idx = child;
		child /= 2;

	}
	minHeap[child] = index;//해당하는 자리에 넣기

	V[index].heap_idx = child;

	return;
}



void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph 자료구조를 만드는 함수
	// *** 이 함수를 추가하자 ***
	// PHW02의 Read_Graph_adj_array()를 이 과제의 자료구조에 맞춰 살짝 수정하여 사용한다
	// 즉, Read_Graph_adj_array()에서 불필요한 내용을 제거한 후 사용.



	int v_i, v_j, cost;//vertex i,j와 cost를 파일에서 입력받기
	for (int i = 0; i < Vnum; i++) {//vertex 저장
		V[i].inS = false;//초기값
		V[i].distance = INT_MAX;//최대값
		V[i].heap_idx = NONE;//초기값
		V[i].f_hd = NONE, V[i].r_hd = NONE;//NONE(-1)저장
	}
	for (int i = 0; i < Enum; i++) {//edge저장
		E[i].flag = false;
		E[i].fp = NONE, E[i].rp = NONE;//초기값으로 NONE저장
	}
	//초기값 설정(파일 읽기 전)

	//파일 읽기
	for (int i = 0; i < Enum; i++) {
		scanf_s("%d %d %d", &v_i, &v_j, &cost);//vertex와 edge 입력받음
		E[i].vf = v_i;
		E[i].vr = v_j;
		E[i].cost = cost;


		if (V[v_i].f_hd == NONE)//f_hd에 기존에 저장된 edge가 없으면
			V[v_i].f_hd = i;//edge 저장
		else {
			E[i].fp = V[v_i].f_hd;//기존에 저장된 edge가 있으면
			V[v_i].f_hd = i;//f_hd에 저장된 edge를 E[i].fp에 저장
			//f_hd에 edge저장
		}
		if (V[v_j].r_hd == NONE)//r_hd도 마찬가지로 edge를 저장
			V[v_j].r_hd = i;
		else {

			E[i].rp = V[v_j].r_hd;
			V[v_j].r_hd = i;
		}

	}
	//파일 읽기 끝
	




	return;
}

// the following functions are for testing if the submitted program is correct.
//int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
//bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);//obj파일에 있음!!
void Error_Exit(const char *s);

int main(void) {
	int		src;
	vertex *V;		int Vnum;
	edge   *E;		int Enum;
	int    *minHeap;
	int    Tree_cost;
	int    Tnum; 		// # of test cases
	clock_t start, finish;
	double cmpt;

	scanf_s("%d", &Tnum);		// read # of tests

	for (int t = 0; t < Tnum; t++) {
		scanf_s("%d %d %d", &Vnum, &Enum, &src);
		V = new vertex[Vnum];
		E = new edge[Enum];
		minHeap = new int[Vnum + 1];	// heap array allocation
		if (V == NULL || E == NULL || minHeap == NULL) {
			Error_Exit("Memory Allocation Error");
		}
		Read_Graph(Vnum, V, Enum, E);

		/**/start = clock();	// start timer

		Tree_cost = SPT_Dijkstra(src, Vnum, V, Enum, E, minHeap);	// code by students

		/**/finish = clock();	// stop timer
		cmpt = ((double)(finish - start)) / (double)CLK_TCK;
		
		// 아래 Tree_Check와 SPT_test 함수는 첨부한 SPT_test.obj 파일에 있다.
		// 이 테스트에서 오류로 인하여 프로그램이 정지하면 뭔가 잘못된 것이다(제출해도 0점)
		/*if (Tree_Check(Vnum, V, Enum, E, minHeap) == 0) {
			Error_Exit("   ERROR The result is not a spanning tree");
		}
		if (SPT_test(src, Vnum, V, Enum, E, minHeap) == false) {
			Error_Exit("** Something wrong in applying Dijkstra's");
		}
		if (t != 0)
			printf("\n");
			*/
		printf("**T%d (Dijkstra) (V = %d, E = %d, time = %.3f sec) Tree Cost = %d\n",
			t + 1, Vnum, Enum, cmpt, Tree_cost);
			
		delete[] minHeap; delete[] V; delete[] E;
	}
	return 0;
}

void Error_Exit(const char *s) {
	printf("%s\n", s);
	exit(-1);
}
