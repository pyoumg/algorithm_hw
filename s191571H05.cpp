#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int minimum(int a, int b, int c);//���� ���� �� return�ϴ� �Լ�
int getOp(int **Table, char* SS,char* TS, int i, int j, int ins_cost, int del_cost, int sub_cost);
//back tracing���� operation���ϴ� �Լ�
void getAllOp(char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,
	// insertion, deletion, substitution cost
// outputs
int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
char **SR,		// Source string with insertion to T marks '*'
char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
char **TR,       // Edited string with deletion form S mark '*'
				// NOTE: ST, OP, TR must be allocated inside the function as needed(minimum as possible)
				//(�Լ����� allocate�ϱ�,����Ʈ�� ��������)


	int arr_len//�迭 ����


);






void Edit_Distance(
	// inputs
	char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,
	// insertion, deletion, substitution cost
// outputs
int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
char **SR,		// Source string with insertion to T marks '*'
char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
char **TR,       // Edited string with deletion form S mark '*'
				// NOTE: ST, OP, TR must be allocated inside the function as needed(minimum as possible)
				//(�Լ����� allocate�ϱ�,����Ʈ�� ��������)


	int  *Mem_Allocated//allocate�� SR,OP,TR�� ũ�� �� ����, 
	//�޸� �󸶳� ����ߴ��� ����Ҷ� ���
) {
	int len_s = strlen(SS) + 1;
	int len_t = strlen(TS) + 1;//string����+1(table�� ��/�� ����)
	int now_i, now_j, arr_len = 0, temp;//arr_len:allocation�Ҷ� ����ϴ� ����
	//temp: arr_len�� ���Ҷ� ����ϴ� ����

	for (int i = 0; i < len_s; i++) {
		Table[i][0] = i * del_cost;//�� �ʱ�ȭ
	}

	for (int j = 0; j < len_t; j++) {
		Table[0][j] = j * ins_cost;//�� �ʱ�ȭ
	}

	for (int i = 1; i < len_s; i++) {
		for (int j = 1; j < len_t; j++) {
			if (SS[i - 1] == TS[j - 1]) {//a=0
				Table[i][j] = minimum(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1]);
			}//�ܼ��� cost ����, operation�� �������� ����
			else {//a=sub_cost
				Table[i][j] = minimum(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub_cost);
			}
		}
	}


	//table �ϼ�(bottom-up)

	now_i = len_s - 1;
	now_j = len_t - 1;
	//table�� Ž���Ҷ��� ������ �ε��� i�� j��
	//strlen(SS or TS)�� ����

	//operation ã��

	//arr_len���ϱ�
	while (now_i != 0 && now_j != 0) {

		temp = getOp(Table, SS,TS,now_i, now_j, ins_cost, del_cost, sub_cost);
		//temp: ����� operation
		if (temp == 0)//insert
		{
			now_j--;
		}
		else if (temp == 2) {//delete

			now_i--;
		}
		else {//nop or sub
			now_j--;
			now_i--;
		}
		arr_len++;
	}

	if (now_i > 0)
		arr_len += now_i;
	if (now_j > 0)
		arr_len += now_j;




	//allocation
	*SR = new char[arr_len + 1];
	*OP = new char[arr_len + 1];
	*TR = new char[arr_len + 1];
	//�������� ��������
	*Mem_Allocated = sizeof(char)*(arr_len+1)*3;


	(*SR)[arr_len] = '\0';
	(*OP)[arr_len] = '\0';
	(*TR)[arr_len] = '\0';//�ι��� ����


	getAllOp(SS, TS, ins_cost, del_cost, sub_cost, Table, SR, OP, TR, arr_len);



	return;
}

int minimum(int a, int b, int c) {//a,b,c�߿� ���� ��(�ų� ��)�� ���� return 
	//Table�� �ϼ��Ҷ� �ʿ�
	if (a <= b && a <= c)
		return a;
	else if (b <= a && b <= c)
		return b;
	else
		return c;
}






//operation�� ���ϴ� �Լ�
int getOp(int **Table, char*SS, char* TS,int i, int j, int ins_cost, int del_cost, int sub_cost) {
	//0:ins 1:nop 2:del 3:sub
	int temp,now=Table[i][j];
	int del_pre = Table[i - 1][j];
	int ins_pre = Table[i][j - 1];
	if (i == 0) {//i:0 j:0�� �ƴ϶�� ����
		return 0;
	}
	else if (j == 0) {
		return 2;
	}
	else {
		
		if (del_pre+del_cost != now && ins_pre+ins_cost != now) {
			//insert�� delete�� ������ �� ���� ���
			if (SS[i - 1] == TS[j - 1]) {//nop
				return 1;
			}
			else {
				return 3;
			}

		}
		else if (del_pre + del_cost == now && ins_pre + ins_cost == now) {
			//������ �ΰ�
			if (ins_cost <= del_cost)
				return 0;
			else//del
				return 2;
		}
		else if(del_pre + del_cost == now) {
			return 2;

		}
		else {
			return 0;
		}

		
	}


}

//SR,OP,TR�� ���ϴ� �Լ�
void getAllOp(char *SS,		// Source string array
	char *TS,		// Edited string array
	int ins_cost, int del_cost, int sub_cost,
	// insertion, deletion, substitution cost

int **Table,    // Cost Table Calculated (2-d array (|SS|+1) x (|TS|+1) will be set)
// outputs
char **SR,		// Source string with insertion to T marks '*'
char **OP,		// Operation sequence('i', 'd', 's', or '.' (nop))
char **TR,       // Edited string with deletion form S mark '*'
				// NOTE: ST, OP, TR must be allocated inside the function as needed(minimum as possible)
				//(�Լ����� allocate�ϱ�,����Ʈ�� ��������)

//input
	int arr_len//�迭 ����

) {

	int now_i = strlen(SS), now_j = strlen(TS);//���� Ž������ Table index
	int temp;//operation
	//operation ���ϱ�
	for (int i = arr_len - 1; i >= 0; i--) {
		if (now_i == 0) {//insert
			(*OP)[i] = 'i';//insertion
			(*SR)[i] = '*';
			(*TR)[i] = TS[--now_j];

		}
		else if (now_j == 0) {//del
			(*OP)[i] = 'd';
			(*SR)[i] = SS[--now_i];
			(*TR)[i] = '*';

		}
		
		//now_i�� now_j�Ѵ� 0�� �ƴҶ�
		else {
			temp = getOp(Table, SS, TS, now_i, now_j, ins_cost, del_cost, sub_cost);
		
			
			if (temp == 0)//insert
			{

				(*OP)[i] = 'i';//insertion
				(*SR)[i] = '*';
				(*TR)[i] = TS[--now_j];


			}
			else if (temp == 2) {//delete


				(*OP)[i] = 'd';
				(*SR)[i] = SS[--now_i];
				(*TR)[i] = '*';

			}
			else if (temp == 1) {//nop
				(*OP)[i] = '.';
				(*SR)[i] = SS[--now_i];
				(*TR)[i] = TS[--now_j];
			}
			else {//sub
				(*OP)[i] = 's';
				(*SR)[i] = SS[--now_i];
				(*TR)[i] = TS[--now_j];
			}

		}
		
	}



	return;
}

