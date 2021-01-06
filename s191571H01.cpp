#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// 파일에서 그래프 정보를 읽어 V와 E 배열을 구성한다.
	// V[ ].name과 E[ ].name은 설정할 필요가 없으나 편의상 각 배열에서의
	//   index를 저장한다.
	// V[ ].S는 adjacent list header로 스택 헤더로도 사용된다.
	// 따라서 edge (i,j)를 읽으면 V[i].S에 edge index k를 push하고
	//  마찬가지로 V[j].S에 k를 push한다.
	// V[].flag과 E[].flag은 반드시 false로 설정해야 한다.

	// ***이 함수를 작성한다

	int v_i, v_j, cost;//입력 받는 데 필요한 지역변수. v_i와 v_j를 잇는 edge의 cost

	for (int i = 0; i < Vnum; i++) {
		V[i].name = i;//편의상 index저장
		V[i].flag = false;//반드시 false로 설정
	}
	
	for (int i = 0; i < Enum; i++) {
		E[i].name = i;//편의상 index저장
		E[i].flag = false;//반드시 false로 설정
	}
	
	for (int i = 0; i < Enum; i++) {//입력은 Edge의 개수만큼(Enum) 들어온다
		scanf_s("%d %d %d", &v_i, &v_j, &cost);
		//E[i]는 v_i와 v_j를 잇고 cost만큼의 cost를 가진다.
		E[i].cost = cost;
		E[i].vf = v_i;
		E[i].vr = v_j;//파일에서 읽은 값 저장

		SLL* s1 = pool.allocSLL();//SLL 할당
		SLL* s2 = pool.allocSLL();//할당
		s1->i = i;//V[i].S에 edge index push해야하므로 edge index 넣기
		s1->p = NULL;//초기화
		s2->i = i;
		s2->p = NULL;//s1과 마찬가지로 한다.
		V[v_i].S.push(s1);//push해줌
		V[v_j].S.push(s2);
	}


}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V 배열의 adjacency list의 원소를 pop()을 사용하여
	// 모두  pool로 되돌려 준다
	int k;

	// ***이 함수를 작성한다
	SLL *temp;//pop할 부분
	for (k = 0; k < Vnum; k++) {
		if (V[k].S.empty() == false) {//비어있지 않다면
			temp = V[k].S.pop();
			pool.freeSLL(temp);
			while (V[k].S.empty()!=true) {//다 비어있을때까지
				temp = V[k].S.pop();
				//main함수에서 이 함수를 수행한 후에 pool.cnt==0 인지 체크하기 때문에
				pool.freeSLL(temp);//freeSLL이 필요하다(pool로 되돌려주기)
			}
		}
	}

	
	
	
}

int BFS_Tree_adj_list(
	int     src,   // source node index
	int     Vnum,  // number of vertices
	vertex *V,     // vertex structure array (starting index 0)
	int     Enum,  // number of edges
	edge   *E     // edge structure array (starting index 0)
) {
	// BFS를 사용하여 BFS tree에 속하는 에지의 flag을 true로 설정한다.
	// BFS 시작 vertex는 입력 파일에서 지정된다(src).
	// V[]의 adjacent list 탐색은 .top() 멤버 함수를 사용하여
	//  adj list 헤더를 얻고 이를 따라가며 인접 vertex를 탐색한다.
	// BFS tree에 속한 에지의 cost 함을 return 한다
	//   (not unique but 모든 각 edge cost가 1이면 unique)
	// -- 함수의 모든 parameter가 사용될 필요는 없다.
	// -- BFS를 위한 queue가 필요하면 STL의 queue를 사용해도 좋다

	// ***이 함수를 작성한다

	//src가 시작하는 index
	
	int sum = 0;//리턴값(cost의 합)
	
	int j;//adjacent list따라가면서 탐색하는 vertex
	std::queue<int> q; //큐
	SLL* s= V[src].S.top();//edge지나는 vertex
	SLL* k;//for문에서 사용됨 

	if (!s)//s가 null이면 sum은 0, tree는 만들어질 수 없다
		return 0;

	V[src].flag = true;//출발점 방문
	if (E[s->i].vf == src)
		V[E[s->i].vr].flag=true;//edge끝점
	else
		V[E[s->i].vf].flag = true;
	E[s->i].flag = true;//출발한 edge 방문
	sum += E[s->i].cost;
	q.push(src);//edge시작점(bfs라서)
	if (E[s->i].vf == src)
		q.push(E[s->i].vr);//edge끝점
	else
		q.push(E[s->i].vf); //q에는 vertex값이 들어간다.
	while (q.empty()==0) {//큐가 비어있지 않을때까지
		j = q.front();//큐 맨앞에있는 데이터
		q.pop();//큐 pop하기
		for (k = V[j].S.top(); k; k = k->p) {//k가 null이 아닐때까지
			if (E[k->i].flag == false) {//여기서의 시작점이 edge에서 끝점이 아닐수도 있다
				
				//아직 방문 안해본 edge이고 

				//edge의 끝점(vr)이 방문 안했을 때 
				if (E[k->i].vr != j && V[E[k->i].vr].flag == false) {
					sum += E[k->i].cost;//sum에 cost더하기
					E[k->i].flag = true;//edge 방문
					V[E[k->i].vf].flag = true;//edge의 다른쪽 vertex 방문
					V[E[k->i].vr].flag = true; //edge의 vertex 방문
					q.push(E[k->i].vr);//큐에 push
				}
				else if (E[k->i].vf != j && V[E[k->i].vf].flag == false) {//edge의 시작점(vf)이 방문 안했을때
					sum += E[k->i].cost;
					E[k->i].flag = true;//방문
					V[E[k->i].vf].flag = true;//edge의 vertex
					V[E[k->i].vr].flag = true;//edge의 다른쪽 vertex
					q.push(E[k->i].vf);//q에 push
				}
			
			}
			
		}
	}
	return sum;
}
		



// SLL management functions
// 아래 함수들은 02_04_CSE3081_CPrg&DataStructure_추가자료.pdf를
//   참조하여 각 함수들을 채워 넣자.
// 스택만 필요하므로 큐의 멤버 함수는 아래 함수 리스트에 없다

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST

	// ***이 함수를 작성한다
	SLL *p = ST;
	if (ST->p == NULL)
		ST = NULL;
	else
		ST = ST->p;
	return p;
}

SLL *sllStack2::top(void) {
	// return p at the top of ST
	return ST;
}

bool sllStack2::empty(void) {
	// return true if empty
	if (ST == NULL)
		return true;
	else
		return false;
}

// SLList2 member functions
SLL *SLList2::allocSLL(void) {		// allocate an SLL element
	SLL *p;

	// ***이 함수를 작성한다
	if (SLL_pool == NULL) {
		p = (SLL*)malloc(sizeof(SLL));
		if (p == NULL)
			errorExit("Memory allocation error(allocSLL)");
		UsedMemoryForSLLs += sizeof(SLL);
		p->i = NONE;
	}
	else {
		p = SLL_pool;
		SLL_pool = p->p;
	}
	p->p = NULL;
	++SLL_cnt;

	return(p);
}

void SLList2::freeSLL(SLL *p) {		// free *p (move it to SLL_pool
	if (p->i == NONE) {
		errorExit("This is already freed(freeSLL).");
	}

	// ***이 함수를 작성한다
	p->i = NONE;//기존 데이터를 초기값으로 설정
	p->p = SLL_pool;
	SLL_pool = p;//SLL_pool로 되돌려준다
	--SLL_cnt;//cnt감소
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	// ***이 함수를 작성한다
	while (p != NULL) {
		SLL_pool = p->p;
		free(p);
		UsedMemoryForSLLs -= sizeof(SLL);
		p = SLL_pool;
	}



	if (SLL_cnt != 0) {
		errorExit("Non-zero SLL_cnt after cleanup.");
	}
}