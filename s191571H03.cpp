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

	// *** �� �κ��� �ۼ��ϼ���
	//Start vertex�� ã�ƾ��ϴµ�(+���翩�� �Ǵ�) (cycle�� ��������� path�� Ȧ�� degree�� vertex���� ����
	//�� �������� O(n)��ŭ�� �ð��� �ҿ��.

	int start_vertex = 0;
	int odd_vertex = 0;
	int v;
	DBL* temp_dbl;//v�� ������ edge�� �����Ҷ� ���
	DBL* temp_dbl_twin;//temp_dbl�� twin���� �����

	for (int i = 0; i < VN; i++) {
		if (V[i].degree % 2 != 0)
		{
			odd_vertex++;
			start_vertex = i;
		}
	}
	if (odd_vertex == 0) {
		*flag = 0;//degree�� ���� ¦��, euler cycle
	}
	else if (odd_vertex == 2) {//2�� Ȧ�� ������ ¦��: euler path
		*flag = 1;
	}
	else {
		*flag = 2;
		*pathN = 0;//cycle�̳� path�� ��������� �����ϱ�
		return path;//path�� ���� �޸𸮸� �Ҵ����� �ʱ�
	}

	*pathN = 0;//�ʱ�ȭ
	path = new int[EN + 1];//path�� cycle
	ST.push(start_vertex);
	while (!ST.empty()) {//������ ������� ����������
		v = ST.top();
		if (V[v].degree == 0) {
			path[(*pathN)++] = v;
			ST.pop();//v�� ST�� top�̾��� ������ pop�ص� ����

		}
		else {

			temp_dbl = V[v].S.pop();//edge����
			temp_dbl_twin = temp_dbl->twin;
			
			

			V[v].degree--;//degree 1����


			if (E[temp_dbl->d].v1 != v)//e=(v,w)���� v1�� v2�� ����� w���� Ȯ���ؾ� �Ѵ�.
			{
				ST.push(E[temp_dbl->d].v1);
				V[E[temp_dbl->d].v1].degree--;
				V[E[temp_dbl->d].v1].S.remove(temp_dbl_twin);//����� edge�� ����
			}
			else
			{
				ST.push(E[temp_dbl->d].v2);
				V[E[temp_dbl->d].v2].degree--;
				V[E[temp_dbl->d].v2].S.remove(temp_dbl_twin);//����� edge�� ����
			}
			pool.freeDBL(temp_dbl);//adj list���� ���ŵǹǷ� pool�� �ǵ����ֱ�
			pool.freeDBL(temp_dbl_twin);//twin�� ���������̴�
			
		}


	}

	return path;
}

void deallocGraph(Vertex *V, Edge *E, int VN) {
	DBL *p;

	// *** ���⿡ adj list�� �����ϴ� routine�� �ۼ��ϼ���
	for (int i = 0; i < VN; i++) {
		while (!V[i].S.empty()) {//������� ���������� 
			p=V[i].S.pop();//s�� ������������� pop�ϱ�
			pool.freeDBL(p);//�ٽ� pool�� �ǵ����ֱ�
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

	// *** �� �κ��� �ۼ��ϼ���

	for (int i = 0; i < EN; i++) {//edge ����ŭ �Է¹޾ұ� ������!
		adjE1 = pool.allocDBL();
		adjE2 = pool.allocDBL();
		adjE1->d = i;//edge index���
		adjE2->d = i;//edge index
		V[E[i].v1].S.push(adjE1);//rp,lp�� push�Լ����� �����
		V[E[i].v2].S.push(adjE2);
		adjE1->twin = adjE2;
		adjE2->twin = adjE1;//���Ұ� ��ȣ���� 

	}

}

void Error_Exit(const char *s) {
	printf("%s", s);
	exit(-1);
}

DBL *DBList::allocDBL(void) {		// allocate a DBL element
	DBL *p;

	// *** �� �κ��� �ۼ��ϼ���
	if (DBL_pool == NULL) {
		p = new DBL;
		if (p == NULL) {
			Error_Exit("Memory alloc error(Alloc_DBL");//�޸� �Ҵ� ����
		}
		UsedMemoryForDBLs += sizeof(DBL);//���Ǵ� �޸𸮸�ŭ �����ֱ�
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
	
	// *** �� �κ��� �ۼ��ϼ���
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

	// *** �� �κ��� �ۼ��ϼ���
	if (ST != NULL) {//ST�� ������� ������ lp�� ������ ���� �����Ƿ�
		ST->lp = p;
	}
	p->rp = ST;
	p->lp = NULL;
	ST = p;
}

DBL *dblStack::pop(void) {	// remove and return p in front of Q
	DBL *p;

	// *** �� �κ��� �ۼ��ϼ���
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

	// *** �� �κ��� �ۼ��ϼ���
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
