#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int minimum(int a, int b, int c);//가장 작은 값 return하는 함수
int getOp(int **Table, char* SS,char* TS, int i, int j, int ins_cost, int del_cost, int sub_cost);
//back tracing에서 operation구하는 함수
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
				//(함수에서 allocate하기,포인트의 포인터형)


	int arr_len//배열 길이


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
				//(함수에서 allocate하기,포인트의 포인터형)


	int  *Mem_Allocated//allocate한 SR,OP,TR의 크기 합 저장, 
	//메모리 얼마나 사용했는지 출력할때 사용
) {
	int len_s = strlen(SS) + 1;
	int len_t = strlen(TS) + 1;//string길이+1(table의 행/렬 길이)
	int now_i, now_j, arr_len = 0, temp;//arr_len:allocation할때 사용하는 변수
	//temp: arr_len을 구할때 사용하는 변수

	for (int i = 0; i < len_s; i++) {
		Table[i][0] = i * del_cost;//열 초기화
	}

	for (int j = 0; j < len_t; j++) {
		Table[0][j] = j * ins_cost;//행 초기화
	}

	for (int i = 1; i < len_s; i++) {
		for (int j = 1; j < len_t; j++) {
			if (SS[i - 1] == TS[j - 1]) {//a=0
				Table[i][j] = minimum(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1]);
			}//단순한 cost 설정, operation을 저장하지 않음
			else {//a=sub_cost
				Table[i][j] = minimum(Table[i][j - 1] + ins_cost, Table[i - 1][j] + del_cost, Table[i - 1][j - 1] + sub_cost);
			}
		}
	}


	//table 완성(bottom-up)

	now_i = len_s - 1;
	now_j = len_t - 1;
	//table을 탐색할때의 현재의 인덱스 i와 j값
	//strlen(SS or TS)와 같다

	//operation 찾기

	//arr_len구하기
	while (now_i != 0 && now_j != 0) {

		temp = getOp(Table, SS,TS,now_i, now_j, ins_cost, del_cost, sub_cost);
		//temp: 실행된 operation
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
	//포인터의 포인터형
	*Mem_Allocated = sizeof(char)*(arr_len+1)*3;


	(*SR)[arr_len] = '\0';
	(*OP)[arr_len] = '\0';
	(*TR)[arr_len] = '\0';//널문자 삽입


	getAllOp(SS, TS, ins_cost, del_cost, sub_cost, Table, SR, OP, TR, arr_len);



	return;
}

int minimum(int a, int b, int c) {//a,b,c중에 가장 작(거나 같)은 것을 return 
	//Table을 완성할때 필요
	if (a <= b && a <= c)
		return a;
	else if (b <= a && b <= c)
		return b;
	else
		return c;
}






//operation을 구하는 함수
int getOp(int **Table, char*SS, char* TS,int i, int j, int ins_cost, int del_cost, int sub_cost) {
	//0:ins 1:nop 2:del 3:sub
	int temp,now=Table[i][j];
	int del_pre = Table[i - 1][j];
	int ins_pre = Table[i][j - 1];
	if (i == 0) {//i:0 j:0이 아니라고 가정
		return 0;
	}
	else if (j == 0) {
		return 2;
	}
	else {
		
		if (del_pre+del_cost != now && ins_pre+ins_cost != now) {
			//insert와 delete를 선택할 수 없는 경우
			if (SS[i - 1] == TS[j - 1]) {//nop
				return 1;
			}
			else {
				return 3;
			}

		}
		else if (del_pre + del_cost == now && ins_pre + ins_cost == now) {
			//선택이 두개
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

//SR,OP,TR을 구하는 함수
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
				//(함수에서 allocate하기,포인트의 포인터형)

//input
	int arr_len//배열 길이

) {

	int now_i = strlen(SS), now_j = strlen(TS);//현재 탐색중인 Table index
	int temp;//operation
	//operation 구하기
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
		
		//now_i랑 now_j둘다 0이 아닐때
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

