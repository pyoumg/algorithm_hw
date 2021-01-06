#define _CRT_SECURE_NO_WARNINGS
using namespace std;
#include <time.h>
#include <stack>
#include "DBL.h"

//#define NO_PATH_OUT   // comment out this line for path output
void Error_Exit(const char *s);

typedef struct _Vertex {
	dblStack S;		// adj list contains edge index
	int degree;
} Vertex;

typedef struct _Edge {
	int v1, v2;
} Edge;

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN);
void adjListGenerate(Vertex *V, Edge *E, int VN, int EN);
void deallocGraph(Vertex *V, Edge *E, int VN);
int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN);

DBList pool;	// DBL storage pool

int main() {
	int T, VN, EN;
	Vertex *V;
	Edge   *E;
	int *path;	// Euler cycle or path
	int pathN;  // path length
	int  flag;	// 0: cycle, 1: path, 2: none
	clock_t start_time, finish_time;

	scanf("%d", &T);	// read # of tests
	for (int t = 1; t <= T; t++) {	// for each test
		graphGeneration(&V, &E, &VN, &EN);

		start_time = clock(); // set the start time

		path = Find_Euler(V, E, VN, EN, &flag, &pathN); // find an Euler path or cycle

		finish_time = clock(); // set finish time
		
		double cmpt = (((double)(finish_time - start_time)) / CLK_TCK)*1000; // compute the time passed
		printf("Test= %d flag= %d VnumInCycle/Path)= %d ", t, flag, pathN);

		if (flag == 0)
			printf("Euler_cycle(exec_time= %.2f msec)\n",cmpt);
		else if (flag == 1)
			printf("Euler_path(exec_time= %.2f msec)\n", cmpt);
		else
			printf("not_Eulerian(exec_time= %.2f msec)\n", cmpt);

#ifndef NO_PATH_OUT
		if (flag != 2) {
			for (int i = 0; i < EN + 1; i++) {
				printf("%d\n", path[i]);
			}
		}
#endif
		if (flag != 2)
			delete[] path;
		deallocGraph(V, E, VN);
	}
	pool.freeDBL_pool();	// clear all the DBL elements

	return 0;
}

int *Find_Euler(Vertex *V, Edge *E, int VN, int EN, int *flag, int *pathN) {
	// input V, VN, E, EN
	// output through paramters
	//   *flag = 0 (Euler cycle), 1 (Euler path), 2 (not Eulerian)
	//   *pathN = size of path[] array
	// output by return
	//   *path = list of vertex ids found(Euler cycle or path)

	stack<int> ST;		// use STL stack as explained in the class
	int *path = NULL;

	// *** 이 부분을 작성하세요
	//Start vertex를 찾아야하는데(+존재여부 판단) (cycle은 상관없지만 path는 홀수 degree인 vertex에서 시작
	//이 과정에서 O(n)만큼의 시간이 소요됨.

	int start_vertex = 0;
	int odd_vertex = 0;
	int v;
	DBL* temp_dbl;//v에 인접한 edge를 선택할때 사용
	DBL* temp_dbl_twin;//temp_dbl의 twin으로 연결됨

	for (int i = 0; i < VN; i++) {
		if (V[i].degree % 2 != 0)
		{
			odd_vertex++;
			start_vertex = i;
		}
	}
	if (odd_vertex == 0) {
		*flag = 0;//degree가 전부 짝수, euler cycle
	}
	else if (odd_vertex == 2) {//2개 홀수 나머지 짝수: euler path
		*flag = 1;
	}
	else {
		*flag = 2;
		*pathN = 0;//cycle이나 path가 만들어지지 않으니까
		return path;//path를 위한 메모리를 할당하지 않기
	}

	*pathN = 0;//초기화
	path = new int[EN + 1];//path나 cycle
	ST.push(start_vertex);
	while (!ST.empty()) {//스택이 비어있지 않을때까지
		v = ST.top();
		if (V[v].degree == 0) {
			path[(*pathN)++] = v;
			ST.pop();//v가 ST의 top이었기 때문에 pop해도 무방

		}
		else {

			temp_dbl = V[v].S.pop();//edge제거
			temp_dbl_twin = temp_dbl->twin;
			
			

			V[v].degree--;//degree 1감소


			if (E[temp_dbl->d].v1 != v)//e=(v,w)에서 v1과 v2중 어떤것이 w인지 확인해야 한다.
			{
				ST.push(E[temp_dbl->d].v1);
				V[E[temp_dbl->d].v1].degree--;
				V[E[temp_dbl->d].v1].S.remove(temp_dbl_twin);//연결된 edge도 제거
			}
			else
			{
				ST.push(E[temp_dbl->d].v2);
				V[E[temp_dbl->d].v2].degree--;
				V[E[temp_dbl->d].v2].S.remove(temp_dbl_twin);//연결된 edge도 제거
			}
			pool.freeDBL(temp_dbl);//adj list에서 제거되므로 pool로 되돌려주기
			pool.freeDBL(temp_dbl_twin);//twin도 마찬가지이다
			
		}


	}

	return path;
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;

	// *** 여기에 adj list를 삭제하는 routine을 작성하세요
	for (int i = 0; i < VN; i++) {
		while (!V[i].S.empty()) {//비어있지 않을때까지 
			p=V[i].S.pop();//s가 비어있을때까지 pop하기
			pool.freeDBL(p);//다시 pool로 되돌려주기
		}
	}
	delete[] V;
	delete[] E;
}

void graphGeneration(Vertex **V, Edge **E, int *VN, int *EN) {
	scanf("%d %d", VN, EN);	// read # of vertices and edges
	//allocVandEarray(V, E, *VN, *EN);	// vertex and edge array allocation

	*V = new Vertex[*VN];
	*E = new Edge[*EN];
	if (*V == NULL || *E == NULL) {
		Error_Exit("Memory Allocation Error!");
	}

	for (int v = 0; v < *VN; v++) {
		(*V)[v].degree = 0;
	}
	for (int e = 0; e < *EN; e++) {
		scanf("%d %d", &((*E)[e].v1), &((*E)[e].v2));	// read edge information
		++((*V)[(*E)[e].v1].degree);
		++((*V)[(*E)[e].v2].degree);
	}
	adjListGenerate(*V, *E, *VN, *EN);	// create adj lust in G=(V,E)
}

void adjListGenerate(Vertex *V, Edge *E, int VN, int EN) {
	// Construct adjacent list in V array
	int v1, v2;
	DBL *adjE1, *adjE2;

	// *** 이 부분을 작성하세요

	for (int i = 0; i < EN; i++) {//edge 수만큼 입력받았기 때문에!
		adjE1 = pool.allocDBL();
		adjE2 = pool.allocDBL();
		adjE1->d = i;//edge index기록
		adjE2->d = i;//edge index
		V[E[i].v1].S.push(adjE1);//rp,lp는 push함수에서 연결됨
		V[E[i].v2].S.push(adjE2);
		adjE1->twin = adjE2;
		adjE2->twin = adjE1;//원소간 상호연결 

	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	// *** 이 부분을 작성하세요
	if (DBL_pool == NULL) {
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL");//메모리 할당 에러
		}
		UsedMemoryForDBLs += sizeof(DBL);//사용되는 메모리만큼 더해주기
		p->d = NONE;
	}
	else {
		p = DBL_pool;
		DBL_pool = p->rp;
	}
	p->rp = p->lp = p->twin = NULL;

	++DBL_cnt;

	return(p);
}

void DBList::freeDBL(DBL *p) {
	// move p to pool

	if (p->d == NONE) {
		Error_Exit("This element is already freed(Free_DBL).");
	}
	
	// *** 이 부분을 작성하세요
	p->d = NONE;
	p->rp = DBL_pool;
	DBL_pool = p;

	--DBL_cnt;		// reduce # of active DBL elements by one 
}

void DBList::freeDBL_pool(void) {
	DBL *p = DBL_pool;

	while (p != NULL) {
		DBL_pool = p->rp;	// get next pointer
		delete p;
		p = DBL_pool;
		UsedMemoryForDBLs -= sizeof(DBL);
	}
	if (DBL_cnt != 0) {
		Error_Exit("Non-zero DBL_cnt after cleanup.");
	}
}

void dblStack::push(DBL *p) {

	// *** 이 부분을 작성하세요
	if (ST != NULL) {//ST가 비어있지 않으면 lp로 연결할 것이 있으므로
		ST->lp = p;
	}
	p->rp = ST;
	p->lp = NULL;
	ST = p;
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	// *** 이 부분을 작성하세요
	p = ST;
	if (ST->rp == NULL)
		ST = NULL;
	else {
		ST = ST->rp;
		ST->lp = NULL;
	}
	return p;
}

void dblStack::remove(DBL *p) {	// delete p from ST

	// *** 이 부분을 작성하세요
	if (ST == p) {
		ST = p->rp;
		if (p->rp != NULL) {
			(p->rp)->lp = NULL;

		}
	}

	else {
		(p->lp)->rp = p->rp;
		if (p->rp != NULL) {
			(p->rp)->lp = p->lp;
		}
	}


}

DBL *dblStack::top(void) {
	return ST;
}

bool dblStack::empty(void) {
	if (ST == NULL)
		return true;
	else
		return false;
}
