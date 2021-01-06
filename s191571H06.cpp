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
void push(int* index, int data,//index: minHeap�� ������ ä�������� ��Ÿ���� ����
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
	//minHeap�� ������ ������ �Լ��� �ۼ��Ѵ��� �� �Լ��� ȣ���ؼ� �̷������. 

	// OUTPUT
	// 1. V[].distance : shortest path length from src to this vertex
	// 2. shortest path spanning tree : set E[].flag = true if the edge is in the tree
	// 3. return the sum of edge costs in the shortest path spanning tree.
	//    should be small as possible (think a way to make it small)
) {
	int treeCost = 0;
	// *** �� �Լ��� �ۼ����� ***
	// �ݵ�� min-heap�� ����Ͽ� O((n+m)logn) �˰����� �����ؾ� �Ѵ�(�ƴϸ� trivial�� ���α׷���)
	// heap ���� �� �ʿ��� �Լ��� �����Ӱ� �ۼ��Ͽ� �߰��Ѵ�.
	// �׷��� global ����, dynamic array ���� �߰��� ������� �ʴ´�(������ �ʿ� ����)
	int temp, index = 0;//index:minHeap���� ���Ǵ� index ����
	int min;//distance[u]�� ���� ���� u

	//�ʱ�ȭ
	V[src].distance = 0;//�������� distance�� 0
	V[src].inS = true;//�������� S�ȿ� ����

	//����� vertex�� distance���ϱ�
	if (V[src].f_hd != -1) {//����� edge�� ������
		temp = V[src].f_hd;//����� edge num
		while (1) {
			V[E[temp].vr].distance = E[temp].cost;
			temp = E[temp].fp;
			if (temp == NONE)//�� ����� edge�� ������
				break;
		}
	}
	if (V[src].r_hd != 1) {//����� edge�� ������
		temp = V[src].r_hd;//����� edge num
		while (1) {
			V[E[temp].vf].distance = E[temp].cost;
			temp = E[temp].rp;
			if (temp == NONE)//�� ����� edge�� ������
				break;
		}
	}

	//�ʱ�ȭ ��
	
	// minheap ����

	for (int i = 0; i < Vnum; i++) {
		if (i != src) {//Vnum-1�� 
			push(&index, i, V, minHeap);
		}
	}
	
	for (int i = 0; i < Vnum - 1; i++) {
		printf("%d ", V[minHeap[i + 1]].distance);
	}
	printf("\n");
	

	for (int i = 0; i < Vnum - 2; i++) {

		min = minHeap[1];//���� ���� vertex


		printf("min: %d, distance %d\n", min,V[min].distance);
		V[min].inS = true;
		del(&index,V,minHeap);//heap���� ����



		if (V[min].f_hd != -1) {//����� edge�� ������
			temp = V[min].f_hd;//����� edge num
			printf("temp:%d\n", temp);
			while (1) {
				if (V[E[temp].vr].inS == false && V[min].distance + E[temp].cost < V[E[temp].vr].distance) {
					

					printf("distance,%d %d %d", V[min].distance, E[temp].cost, V[E[temp].vr].distance);
					V[E[temp].vr].distance = V[min].distance + E[temp].cost;//update distance
					
					adjust(E[temp].vr, V, minHeap);

				}
				temp = E[temp].fp;//����� edge num
				if (temp == NONE)//�� ����� edge�� ������
					break;
			}
		}

		if (V[min].r_hd != -1) {//����� edge�� ������
			temp = V[min].r_hd;//����� edge num
			printf("temp:%d\n", temp);

			while (1) {
				if (V[E[temp].vf].inS == false  && V[min].distance + E[temp].cost < V[E[temp].vf].distance) {
					printf("distance,%d %d %d", V[min].distance, E[temp].cost, V[E[temp].vf].distance);
					V[E[temp].vf].distance = V[min].distance + E[temp].cost;//update distance
					adjust(E[temp].vf, V, minHeap);
				}
				temp = E[temp].rp;
				if (temp == NONE)//�� ����� edge�� ������
					break;
			}
		}


	}
	//Dijkstra ��

	for (int i = 0; i < Vnum; i++)
		printf("%d\n", V[i].distance);


	//treeã��(back tracing)
	/*
	for (int i = 0; i < Vnum - 2; i++) {
		temp=
	}
	*/

	return treeCost;
}

void del(int* total, vertex* V, int *minHeap) {
	//���� minHeap�� 1~*total���� ä�����ִ� ����
	//minHeap[1]�� ������ �� minHeap���¸� �����Ѵ�.
	int temp,parent=1,child=2;
	V[minHeap[1]].heap_idx = NONE;//������ index1�̾��� ���Ҵ� heap���� ������
	temp = minHeap[(*total)--];


	
	while (child <= *total) {
		if ((child < *total) && (minHeap[child] > minHeap[child + 1])) {   
			child++;//left child/right child ����
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



void push(int* index, int data,//index: minHeap�� ������ ä�������� ��Ÿ���� ����
	vertex *V,
	int *minHeap	// array for min heap (array size = Vnum+1)
		// heap index range is 1 ~ (Vnum - 1) note: src must not in the initial heap
		// just arry is passed. must construct min-heap in this function
) {
	int child;
	
	child = ++(*index);//minHeap�� 1���� ����,index�� 1 �����ؾ���
	
	while (V[data].distance < V[child / 2].distance&&child>1) {
		minHeap[child] = minHeap[child / 2];
		//�θ� ��忡 �ִ��� �ڽ� ����� ���� �ȴ�.
		V[minHeap[child/2]].heap_idx = child;
		child /= 2;

	}
	minHeap[child] = data;//�ش��ϴ� �ڸ��� �ֱ�
	printf("push child,data: %d %d\n", child,data);
	V[data].heap_idx = child;


	return;
}

void adjust(int index,vertex* V,int* minHeap) {//distance���� �����Ǿ��� ������ minheap�� �����Ǿ�� �Ѵ�.
	int child = V[index].heap_idx;
	printf("adj index,child: %d %d\n", index,child);
	while (V[index].distance < V[child / 2].distance&&child>1) {
		minHeap[child] = minHeap[child / 2];//�θ� ��忡 �ִ��� �ڽ� ����� ���� �ȴ�.
		V[minHeap[child/2]].heap_idx = child;
		child /= 2;

	}
	minHeap[child] = index;//�ش��ϴ� �ڸ��� �ֱ�

	V[index].heap_idx = child;

	return;
}



void Read_Graph(int Vnum, vertex *V, int Enum, edge *E) {
	// Graph �ڷᱸ���� ����� �Լ�
	// *** �� �Լ��� �߰����� ***
	// PHW02�� Read_Graph_adj_array()�� �� ������ �ڷᱸ���� ���� ��¦ �����Ͽ� ����Ѵ�
	// ��, Read_Graph_adj_array()���� ���ʿ��� ������ ������ �� ���.



	int v_i, v_j, cost;//vertex i,j�� cost�� ���Ͽ��� �Է¹ޱ�
	for (int i = 0; i < Vnum; i++) {//vertex ����
		V[i].inS = false;//�ʱⰪ
		V[i].distance = INT_MAX;//�ִ밪
		V[i].heap_idx = NONE;//�ʱⰪ
		V[i].f_hd = NONE, V[i].r_hd = NONE;//NONE(-1)����
	}
	for (int i = 0; i < Enum; i++) {//edge����
		E[i].flag = false;
		E[i].fp = NONE, E[i].rp = NONE;//�ʱⰪ���� NONE����
	}
	//�ʱⰪ ����(���� �б� ��)

	//���� �б�
	for (int i = 0; i < Enum; i++) {
		scanf_s("%d %d %d", &v_i, &v_j, &cost);//vertex�� edge �Է¹���
		E[i].vf = v_i;
		E[i].vr = v_j;
		E[i].cost = cost;


		if (V[v_i].f_hd == NONE)//f_hd�� ������ ����� edge�� ������
			V[v_i].f_hd = i;//edge ����
		else {
			E[i].fp = V[v_i].f_hd;//������ ����� edge�� ������
			V[v_i].f_hd = i;//f_hd�� ����� edge�� E[i].fp�� ����
			//f_hd�� edge����
		}
		if (V[v_j].r_hd == NONE)//r_hd�� ���������� edge�� ����
			V[v_j].r_hd = i;
		else {

			E[i].rp = V[v_j].r_hd;
			V[v_j].r_hd = i;
		}

	}
	//���� �б� ��
	




	return;
}

// the following functions are for testing if the submitted program is correct.
//int  Tree_Check(int Vnum, vertex *V, int Enum, edge *E, int *visited);
//bool SPT_test(int src, int Vnum, vertex *V, int Enum, edge *E, int *minHeap);//obj���Ͽ� ����!!
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
		
		// �Ʒ� Tree_Check�� SPT_test �Լ��� ÷���� SPT_test.obj ���Ͽ� �ִ�.
		// �� �׽�Ʈ���� ������ ���Ͽ� ���α׷��� �����ϸ� ���� �߸��� ���̴�(�����ص� 0��)
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
