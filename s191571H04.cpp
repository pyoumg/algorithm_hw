#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

void mergesort(unsigned n_start, unsigned n_end, double X[], unsigned S[], unsigned TMP[]);
void merge(unsigned n_start, unsigned h, unsigned n_end, double X[], unsigned S[], unsigned TMP[]);
double combine(
	unsigned Yid[], //�̹� ���ĵ�
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

	//closestPariDC() ȣ�� ���� x��ǥ �񳻸��������� �����ؼ� index�� Xid�� ����
	//�����Լ����� �Է¹����� Xid[i]=i�� �ʱⰪ ��������
	//merge sort�� ����ؼ� ������ ���̶� TMP�� �ʿ���

	unsigned THR = 10;//threshold

	if (N <= THR) {
		selection_sort(0, N - 1, X, Xid);//O(n^2) �˰������� ���
		//�� ��쿡�� TMP �ʿ� ����, Xid�� ���ĵ� �迭�̴�.

	}
	else {//N�� THR���� ū ���
		mergesort(0, N - 1, X, Xid, TMP);//Xid  ����
	}

	return;

}
void mergesort(unsigned n_start, unsigned n_end, double X[], unsigned S[], unsigned TMP[]) {
	//n_start:�迭�� ������,n_end:�迭�� ����, TMP: S�� sort�� �迭

	unsigned len = n_end - n_start + 1; //�迭�� ����

	if (len > 1) {
		const int h = len / 2 + n_start - 1;//�ε����� -1�� �Ѵ�

		mergesort(n_start, h, X, S, TMP);//n_start~h
		mergesort(h + 1, n_end, X, S, TMP);//h+1~n_end(recursive)
		merge(n_start, h, n_end, X, S, TMP);
	}
}


void merge(unsigned n_start, unsigned h, unsigned n_end, double X[], unsigned S[], unsigned TMP[]) {
	//S[n_start]~S[h]������ �迭��
	//S[h+1]~S[n_end]������ �迭�� sorting �ؼ� 
	//TMP[n_start]~TMP[n_end]���� ����(�ӽù迭)
	//������ TMP�� �ٽ� S�� ����

	unsigned i = n_start, j = h + 1, k = n_start;
	//S[i],S[j]:���� X�� Ž������ index
	//k: TMP�� index

	while (i <= h && j <= n_end) {//�ε��� ���� 0~h-1,0~m-1

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
	//���� ��

	for (unsigned j = n_start; j <= n_end; j++) {
		S[j] = TMP[j];//S�� ����

	}

}

void selection_sort(unsigned start, unsigned end, double X[], unsigned S[]) {
	//O(n^2) 
	//S:X�� ���ؼ� ������ �迭 
	//start~end: �迭���� sort�� index
	unsigned temp, min;

	if (end != 0) {//����÷ο� ����,end==0�̸� start�� 0�̹Ƿ� sort �� �ʿ䰡 ����.
		for (unsigned i = start; i <= end - 1; i++) {
			min = i;
			for (unsigned j = i + 1; j <= end; j++) {
				if (X[S[j]] < X[S[min]])
					min = j;
			}
			temp = S[min];//X�� �������� �ʰ� S�� �����Ѵ�.
			S[min] = S[i];
			S[i] = temp;


		}
	}

	return;

}


double combine(
	unsigned Yid[], //�̹� ���ĵ�,
	unsigned L, unsigned R, unsigned M,
	double d_LR,
	double *X, double *Y,
	double x_mid,//[x_mid-d_LR,x_mid+d_LR]�� �ش��ϴ� �͸� TMP�� ����
	unsigned *pt1, unsigned *pt2, unsigned *TMP) {
	//d_min�� ���ؼ� return �ϴ� �Լ�

	unsigned len = R - L + 1;
	unsigned k = 0;//TMP�� index
	double d_min = d_LR, d;//�ʱⰪ�� d_LR,���������� �Ÿ��� �ּҰ�


	for (unsigned i = L; i <= R; i++) {

		if (X[Yid[i]] <= x_mid + d_LR && X[Yid[i]] >= x_mid - d_LR)
		{//�����ȿ� ����
			TMP[k] = Yid[i];//TMP�� ����
			k++;
		}

	}
	//y�� ���� ���ĵ� ������ ������ �ش�Ǵ°͸� copy
	//0~k-1���� �����

	d_min = d_LR * d_LR;
	//sqrt ���� Ƚ���� ���̱� ���� d_min=d_min^2�� ����


	if (k != 0) {//����÷ο� ����
		for (unsigned i = 0; i < k - 1; i++) {

			for (unsigned j = i + 1; j < k; j++)
			{
				if ((Y[TMP[j]] - Y[TMP[i]])*(Y[TMP[j]] - Y[TMP[i]]) >= d_min)
					break;//y�� ���ĵǾ������ϱ�
				//constant time
				d = (Y[TMP[j]] - Y[TMP[i]])*(Y[TMP[j]] - Y[TMP[i]]) + (X[TMP[j]] - X[TMP[i]])*(X[TMP[j]] - X[TMP[i]]);
				if (d < d_min) {//����Ѱ��� ���ݱ����� �ּڰ����� ������
					d_min = d;//d_min ����
					*pt1 = TMP[i], *pt2 = TMP[j]; //*pt1,*pt2�� ����
				}

			}
		}
	}
	return sqrt(d_min);//�������̾����ϱ� return�Ҷ��� sqrt���
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


	//ã�� �������� distance�� return�ϴ� recursive�Լ�
	//�����Լ����� ȣ���Ҷ� L=0,R=N-1�� ȣ������
	//�� �Լ� ȣ������ Xid�� �񳻸��������� ��������

	unsigned M = 0;//�����
	double d1 = DBL_MAX, d2 = DBL_MAX;//d1,d2:�� �Լ��� recursive�ϰ� ȣ���Ҷ� ����ϴ� ����
	//���� �������� d_LR�� ��
	double d_LR = DBL_MAX;
	double x_mid;
	double d_min = DBL_MAX;//return�� ��
	unsigned a_1, b_1, a_2, b_2;//pt1,pt2�� ���� ���Ҷ� ���Ǵ� ����


	if (R - L + 1 <= THR) {//brute force�� ����


		//�����ؾ��ߴµ� ����.... ����...��
		for (unsigned i = L; i <= R; i++) {
			Yid[i] = Xid[i];//�迭����
		}

		selection_sort(L, R, Y, Yid);
		//O(n^2) �˰������� Y��ǥ�� ������������ ����

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

		//���� ���ĵǾ������Ƿ� �ٷ� merge�ᵵ ��
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


