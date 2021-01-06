
#include "Graph_adj_array.h"

void Read_Graph_adj_array(int Vnum, vertex V[], int Enum, edge E[]) {
	// read graph information
	// V와 E의 name field는 각각 자신의 인덱스를 저장한다
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

	// ***이 함수를 작성한다

	int v_i, v_j, cost;//vertex i,j와 cost를 파일에서 입력받기
	for (int i = 0; i < Vnum; i++) {//vertex 저장
		V[i].flag = false;//초기값
		V[i].name = i;//name field에 인덱스 저장
		V[i].f_hd = NONE, V[i].r_hd = NONE;//NONE(-1)저장
	}
	for (int i = 0; i < Enum; i++) {//edge저장
		E[i].flag = false;
		E[i].name = i;
		E[i].fp = NONE, E[i].rp = NONE;//초기값으로 NONE저장
	}
	//초기값 설정

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
	
}

int DFS_Tree_adj_array(//return값이 int형이다(cost)
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0) 
) {
	// DFS를 사용하여 DFS tree에 속하는 에지의 flag을 true로 설정한다.
	// DFS 시작 vertex는 입력 파일에서 지정된다(src).
	// DFS tree에 속한 에지의 cost 함을 return 한다(모든 각 edge cost가 1이면 unique)
	// recursive 함수로 작성해도 무방하다.

	// ***이 함수를 작성한다***
	//printf("%d\n", src);
	int pop = -1;

	int sum = 0;//return 해줄값, cost의 sum
	int start;//방문할 edge의 vertex
	V[src].flag = true;//출발점 방문

	if (V[src].f_hd == -1 && V[src].r_hd == -1)
		return sum;//시작점와 연결된 edge가 없는경우
	if (V[src].f_hd != -1 && E[V[src].f_hd].flag == false) {
		start = V[src].f_hd;
		while (1) {//fp로 연결된 edge들 탐색
			if (V[E[start].vr].flag == false) {//edge도 방문하지 않고 vr인 vertex도 방문하지 않음
				E[start].flag = true;//edge방문
				V[E[start].vr].flag = true;//vertex방문
				sum += E[start].cost + DFS_Tree_adj_array(E[start].vr, Vnum, V, Enum, E);
				//recursive 함수로 작성
			}
			if (E[start].fp == NONE)
				break;//-1이면 f_hd로 연결된 edge는 모두 탐색한 것
			start = E[start].fp;//연결된 다음 edge로 이동
		}
	}
	
	if (V[src].r_hd != -1 && E[V[src].r_hd].flag == false) {
		start = V[src].r_hd;
		while (1) {//fp로 연결된 edge들 탐색
			if (V[E[start].vf].flag == false) {//edge도 방문하지 않고 vf인 vertex도 방문하지 않음
				E[start].flag = true;//edge방문
				V[E[start].vf].flag = true;//vertex방문
				sum += E[start].cost + DFS_Tree_adj_array(E[start].vf, Vnum, V, Enum, E);//recursive함수로 작성
			}
			if (E[start].rp == NONE)
				break;//-1이면 r_hd로 연결된 edge는 모두 탐색한 것
			start = E[start].rp;//연결된 다음 edge로 이동
		}
	}

	return sum;
}
