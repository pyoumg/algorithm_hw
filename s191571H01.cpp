#include "Graph_adj_list.h"
#include <queue>

extern void errorExit(const char *s);

SLList2 pool;
unsigned long UsedMemoryForArray = 0;

void Read_Graph_adj_list(int Vnum, vertex *V, int Enum, edge *E) {
	// ���Ͽ��� �׷��� ������ �о� V�� E �迭�� �����Ѵ�.
	// V[ ].name�� E[ ].name�� ������ �ʿ䰡 ������ ���ǻ� �� �迭������
	//   index�� �����Ѵ�.
	// V[ ].S�� adjacent list header�� ���� ����ε� ���ȴ�.
	// ���� edge (i,j)�� ������ V[i].S�� edge index k�� push�ϰ�
	//  ���������� V[j].S�� k�� push�Ѵ�.
	// V[].flag�� E[].flag�� �ݵ�� false�� �����ؾ� �Ѵ�.

	// ***�� �Լ��� �ۼ��Ѵ�

	int v_i, v_j, cost;//�Է� �޴� �� �ʿ��� ��������. v_i�� v_j�� �մ� edge�� cost

	for (int i = 0; i < Vnum; i++) {
		V[i].name = i;//���ǻ� index����
		V[i].flag = false;//�ݵ�� false�� ����
	}
	
	for (int i = 0; i < Enum; i++) {
		E[i].name = i;//���ǻ� index����
		E[i].flag = false;//�ݵ�� false�� ����
	}
	
	for (int i = 0; i < Enum; i++) {//�Է��� Edge�� ������ŭ(Enum) ���´�
		scanf_s("%d %d %d", &v_i, &v_j, &cost);
		//E[i]�� v_i�� v_j�� �հ� cost��ŭ�� cost�� ������.
		E[i].cost = cost;
		E[i].vf = v_i;
		E[i].vr = v_j;//���Ͽ��� ���� �� ����

		SLL* s1 = pool.allocSLL();//SLL �Ҵ�
		SLL* s2 = pool.allocSLL();//�Ҵ�
		s1->i = i;//V[i].S�� edge index push�ؾ��ϹǷ� edge index �ֱ�
		s1->p = NULL;//�ʱ�ȭ
		s2->i = i;
		s2->p = NULL;//s1�� ���������� �Ѵ�.
		V[v_i].S.push(s1);//push����
		V[v_j].S.push(s2);
	}


}

void Free_Graph_adj_list(int Vnum, vertex *V) {
	// V �迭�� adjacency list�� ���Ҹ� pop()�� ����Ͽ�
	// ���  pool�� �ǵ��� �ش�
	int k;

	// ***�� �Լ��� �ۼ��Ѵ�
	SLL *temp;//pop�� �κ�
	for (k = 0; k < Vnum; k++) {
		if (V[k].S.empty() == false) {//������� �ʴٸ�
			temp = V[k].S.pop();
			pool.freeSLL(temp);
			while (V[k].S.empty()!=true) {//�� �������������
				temp = V[k].S.pop();
				//main�Լ����� �� �Լ��� ������ �Ŀ� pool.cnt==0 ���� üũ�ϱ� ������
				pool.freeSLL(temp);//freeSLL�� �ʿ��ϴ�(pool�� �ǵ����ֱ�)
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
	// BFS�� ����Ͽ� BFS tree�� ���ϴ� ������ flag�� true�� �����Ѵ�.
	// BFS ���� vertex�� �Է� ���Ͽ��� �����ȴ�(src).
	// V[]�� adjacent list Ž���� .top() ��� �Լ��� ����Ͽ�
	//  adj list ����� ��� �̸� ���󰡸� ���� vertex�� Ž���Ѵ�.
	// BFS tree�� ���� ������ cost ���� return �Ѵ�
	//   (not unique but ��� �� edge cost�� 1�̸� unique)
	// -- �Լ��� ��� parameter�� ���� �ʿ�� ����.
	// -- BFS�� ���� queue�� �ʿ��ϸ� STL�� queue�� ����ص� ����

	// ***�� �Լ��� �ۼ��Ѵ�

	//src�� �����ϴ� index
	
	int sum = 0;//���ϰ�(cost�� ��)
	
	int j;//adjacent list���󰡸鼭 Ž���ϴ� vertex
	std::queue<int> q; //ť
	SLL* s= V[src].S.top();//edge������ vertex
	SLL* k;//for������ ���� 

	if (!s)//s�� null�̸� sum�� 0, tree�� ������� �� ����
		return 0;

	V[src].flag = true;//����� �湮
	if (E[s->i].vf == src)
		V[E[s->i].vr].flag=true;//edge����
	else
		V[E[s->i].vf].flag = true;
	E[s->i].flag = true;//����� edge �湮
	sum += E[s->i].cost;
	q.push(src);//edge������(bfs��)
	if (E[s->i].vf == src)
		q.push(E[s->i].vr);//edge����
	else
		q.push(E[s->i].vf); //q���� vertex���� ����.
	while (q.empty()==0) {//ť�� ������� ����������
		j = q.front();//ť �Ǿտ��ִ� ������
		q.pop();//ť pop�ϱ�
		for (k = V[j].S.top(); k; k = k->p) {//k�� null�� �ƴҶ�����
			if (E[k->i].flag == false) {//���⼭�� �������� edge���� ������ �ƴҼ��� �ִ�
				
				//���� �湮 ���غ� edge�̰� 

				//edge�� ����(vr)�� �湮 ������ �� 
				if (E[k->i].vr != j && V[E[k->i].vr].flag == false) {
					sum += E[k->i].cost;//sum�� cost���ϱ�
					E[k->i].flag = true;//edge �湮
					V[E[k->i].vf].flag = true;//edge�� �ٸ��� vertex �湮
					V[E[k->i].vr].flag = true; //edge�� vertex �湮
					q.push(E[k->i].vr);//ť�� push
				}
				else if (E[k->i].vf != j && V[E[k->i].vf].flag == false) {//edge�� ������(vf)�� �湮 ��������
					sum += E[k->i].cost;
					E[k->i].flag = true;//�湮
					V[E[k->i].vf].flag = true;//edge�� vertex
					V[E[k->i].vr].flag = true;//edge�� �ٸ��� vertex
					q.push(E[k->i].vf);//q�� push
				}
			
			}
			
		}
	}
	return sum;
}
		



// SLL management functions
// �Ʒ� �Լ����� 02_04_CSE3081_CPrg&DataStructure_�߰��ڷ�.pdf��
//   �����Ͽ� �� �Լ����� ä�� ����.
// ���ø� �ʿ��ϹǷ� ť�� ��� �Լ��� �Ʒ� �Լ� ����Ʈ�� ����

// stack member fuctions
void sllStack2::push(SLL *p) {
	// insert p to the top of stack ST
	p->p = ST;
	ST = p;
}

SLL *sllStack2::pop(void) {
	// remove and return p at the top of ST

	// ***�� �Լ��� �ۼ��Ѵ�
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

	// ***�� �Լ��� �ۼ��Ѵ�
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

	// ***�� �Լ��� �ۼ��Ѵ�
	p->i = NONE;//���� �����͸� �ʱⰪ���� ����
	p->p = SLL_pool;
	SLL_pool = p;//SLL_pool�� �ǵ����ش�
	--SLL_cnt;//cnt����
}

void SLList2::freeSLL_pool(void) {	// clear SLL_pool
	SLL *p = SLL_pool;

	// ***�� �Լ��� �ۼ��Ѵ�
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