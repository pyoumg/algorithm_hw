#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

void mergesort(unsigned n_start, unsigned n_end, double X[], unsigned S[], unsigned TMP[]);
void merge(unsigned n_start, unsigned h, unsigned n_end, double X[], unsigned S[], unsigned TMP[]);
double combine(
	unsigned Yid[], //이미 정렬됨
	unsigned L, unsigned R, unsigned M,
	double d_LR,
	double *X, double *Y,
	double x_mid,
	unsigned *pt1, unsigned *pt2, unsigned *TMP);
void selection_sort(unsigned start, unsigned end, double X[], unsigned S[]);





void   sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {
	// input  : X[]   (x position array of N points)
	//          Xid[] (index array of X (initially [0,1,...,N-1])
	//          TMP[] (temporary array of size N. may or may not be used)
	//          N   number of points to sort
	// output : Xid[] (sorted indices by x position in X[])

	//closestPariDC() 호출 전에 x좌표 비내림차순으로 정렬해서 index를 Xid에 저장
	//메인함수에서 입력받을때 Xid[i]=i로 초기값 설정했음
	//merge sort를 사용해서 정렬할 것이라서 TMP가 필요함

	unsigned THR = 10;//threshold

	if (N <= THR) {
		selection_sort(0, N - 1, X, Xid);//O(n^2) 알고리즘으로 계산
		//이 경우에는 TMP 필요 없음, Xid가 정렬된 배열이다.

	}
	else {//N이 THR보다 큰 경우
		mergesort(0, N - 1, X, Xid, TMP);//Xid  정렬
	}

	return;

}
void mergesort(unsigned n_start, unsigned n_end, double X[], unsigned S[], unsigned TMP[]) {
	//n_start:배열의 시작점,n_end:배열의 끝점, TMP: S를 sort할 배열

	unsigned len = n_end - n_start + 1; //배열의 길이

	if (len > 1) {
		const int h = len / 2 + n_start - 1;//인덱스라 -1을 한다

		mergesort(n_start, h, X, S, TMP);//n_start~h
		mergesort(h + 1, n_end, X, S, TMP);//h+1~n_end(recursive)
		merge(n_start, h, n_end, X, S, TMP);
	}
}


void merge(unsigned n_start, unsigned h, unsigned n_end, double X[], unsigned S[], unsigned TMP[]) {
	//S[n_start]~S[h]까지의 배열과
	//S[h+1]~S[n_end]까지의 배열을 sorting 해서 
	//TMP[n_start]~TMP[n_end]까지 저장(임시배열)
	//정렬한 TMP를 다시 S로 복사

	unsigned i = n_start, j = h + 1, k = n_start;
	//S[i],S[j]:현재 X에 탐색중인 index
	//k: TMP의 index

	while (i <= h && j <= n_end) {//인덱스 범위 0~h-1,0~m-1

		if (X[S[i]] < X[S[j]]) {

			TMP[k] = S[i];
			i++;
		}
		else {
			TMP[k] = S[j];
			j++;
		}
		k++;
	}

	if (i > h) {
		for (j; j <= n_end; j++) {
			TMP[k] = S[j];
			k++;
		}
	}
	else {
		for (i; i <= h; i++) {
			TMP[k] = S[i];
			k++;
		}
	}
	//정렬 끝

	for (unsigned j = n_start; j <= n_end; j++) {
		S[j] = TMP[j];//S로 복사

	}

}

void selection_sort(unsigned start, unsigned end, double X[], unsigned S[]) {
	//O(n^2) 
	//S:X를 통해서 정렬할 배열 
	//start~end: 배열에서 sort할 index
	unsigned temp, min;

	if (end != 0) {//언더플로우 방지,end==0이면 start는 0이므로 sort 할 필요가 없다.
		for (unsigned i = start; i <= end - 1; i++) {
			min = i;
			for (unsigned j = i + 1; j <= end; j++) {
				if (X[S[j]] < X[S[min]])
					min = j;
			}
			temp = S[min];//X는 변경하지 않고 S를 변경한다.
			S[min] = S[i];
			S[i] = temp;


		}
	}

	return;

}


double combine(
	unsigned Yid[], //이미 정렬됨,
	unsigned L, unsigned R, unsigned M,
	double d_LR,
	double *X, double *Y,
	double x_mid,//[x_mid-d_LR,x_mid+d_LR]에 해당하는 것만 TMP로 복사
	unsigned *pt1, unsigned *pt2, unsigned *TMP) {
	//d_min을 구해서 return 하는 함수

	unsigned len = R - L + 1;
	unsigned k = 0;//TMP의 index
	double d_min = d_LR, d;//초기값은 d_LR,두점사이의 거리의 최소값


	for (unsigned i = L; i <= R; i++) {

		if (X[Yid[i]] <= x_mid + d_LR && X[Yid[i]] >= x_mid - d_LR)
		{//범위안에 들어가면
			TMP[k] = Yid[i];//TMP로 복사
			k++;
		}

	}
	//y에 대해 정렬된 순서로 범위에 해당되는것만 copy
	//0~k-1까지 저장됨

	d_min = d_LR * d_LR;
	//sqrt 연산 횟수를 줄이기 위해 d_min=d_min^2로 설정


	if (k != 0) {//언더플로우 방지
		for (unsigned i = 0; i < k - 1; i++) {

			for (unsigned j = i + 1; j < k; j++)
			{
				if ((Y[TMP[j]] - Y[TMP[i]])*(Y[TMP[j]] - Y[TMP[i]]) >= d_min)
					break;//y로 정렬되어있으니까
				//constant time
				d = (Y[TMP[j]] - Y[TMP[i]])*(Y[TMP[j]] - Y[TMP[i]]) + (X[TMP[j]] - X[TMP[i]])*(X[TMP[j]] - X[TMP[i]]);
				if (d < d_min) {//계산한값이 지금까지의 최솟값보다 작으면
					d_min = d;//d_min 변경
					*pt1 = TMP[i], *pt2 = TMP[j]; //*pt1,*pt2값 변경
				}

			}
		}
	}
	return sqrt(d_min);//제곱값이었으니까 return할때는 sqrt사용
}

double closestPairDC(
	unsigned L, unsigned R,	  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2, // closest pair points indices
	double *X, double *Y,         // (x,y) positions array(input)
	unsigned *Xid,  // point index array(sorted by x coordinates)
	unsigned *Yid,  // point index array(gradually sorted by y coordinates)
	unsigned *TMP,  // temporal index array
	unsigned THR	// threshold value
) {
	// Input : L, R, X[], Y[], Xid[]
	// Output : pt1, pt2 ( the distance between (X[pt1],Y[pt1]) and
	//                     (X[pt2],Y[pt2]) is the closest )
	//          return the distance value
	// Others ; Yid[], TMP[] (scratch arrays for D&C algorithm)


	//찾은 두점간의 distance를 return하는 recursive함수
	//메인함수에서 호출할때 L=0,R=N-1로 호출했음
	//이 함수 호출전에 Xid를 비내림차순으로 정렬했음

	unsigned M = 0;//가운데값
	double d1 = DBL_MAX, d2 = DBL_MAX;//d1,d2:이 함수를 recursive하게 호출할때 사용하는 변수
	//둘중 작은값이 d_LR이 됨
	double d_LR = DBL_MAX;
	double x_mid;
	double d_min = DBL_MAX;//return할 값
	unsigned a_1, b_1, a_2, b_2;//pt1,pt2의 값을 정할때 사용되는 변수


	if (R - L + 1 <= THR) {//brute force로 구함


		//구현해야했는데 안함.... 감점...ㅠ
		for (unsigned i = L; i <= R; i++) {
			Yid[i] = Xid[i];//배열복사
		}

		selection_sort(L, R, Y, Yid);
		//O(n^2) 알고리즘으로 Y좌표의 오름차순으로 정렬

	}
	else {
		M = (L + R) / 2;
		d1 = closestPairDC(L, M, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
		a_1 = *pt1;
		a_2 = *pt2;
		d2 = closestPairDC(M + 1, R, pt1, pt2, X, Y, Xid, Yid, TMP, THR);
		b_1 = *pt1;
		b_2 = *pt2;
		merge(L, M, R, Y, Yid, TMP);

		//각각 정렬되어있으므로 바로 merge써도 됨
		//cn

		if (d1 < d2) {//d1
			d_LR = d1, *pt1 = a_1, *pt2 = a_2;
		}
		else {//d2
			d_LR = d2, *pt1 = b_1, *pt2 = b_2;
		}

		//d_LR=min{d1,d2}


		x_mid = (X[Xid[M]] + X[Xid[M + 1]]) / 2;
		d_min = combine(Yid, L, R, M, d_LR, X, Y, x_mid, pt1, pt2, TMP);


	}

	return d_min;
}


