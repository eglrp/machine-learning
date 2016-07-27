#include "stdafx.h"
#include "cpp_speed_up.h"
#include <stdio.h>
#include <math.h>
#include <float.h>
//����census
void __stdcall get_census(BOOLEAN result[], const INT16 image[], const INT32 strides[], const INT32 shapes[], const INT32 window_size) {
	//row length
	int row_length = shapes[0];
	//column length
	int column_length = shapes[1];
	//row and column size
	int S0 = strides[0] / sizeof(INT16);
	int S1 = strides[1] / sizeof(INT16);
	//�ȼ����м��get
	for (int row = window_size / 2; row < row_length - window_size / 2; row++) {
		for (int column = window_size / 2; column < column_length - window_size / 2; column++) {
			//�е�λ��
			int pos = row*S0 + column*S1;
			//��ʼλ��
			int start_pos = (row - window_size / 2)*S0 + (column - window_size / 2)*S1;
			//����ֵ
			int mid_value = image[pos];
			for (int i = 0; i < window_size; i++) {
				for (int j = 0; j < window_size; j++) {

					result[((row*column_length + column)*window_size*window_size) + i*window_size + j] =
						image[start_pos + i*S0 + j*S1] > mid_value ? true : false;
				}
			}
		}
	}
}

//��������bool����hamming����
INT16 __stdcall get_hamming_distance(const BOOLEAN census1[], const BOOLEAN census2[], const int len) {
	//��ͬ����
	INT16 count = 0;
	for (int i = 0; i < len; i++) {
		if (census1[i] != census2[i]) {
			count++;
		}
	}
	return count;
}
//���㵥�����
void __stdcall compute_cost_census_d(INT16 result[],BOOLEAN left[],BOOLEAN right[],const INT16 strides[],const INT16 shapes[]) {
	//row length
	int row_length = shapes[0];
	//column length
	int column_length = shapes[1];
	//census length
	int census_length = shapes[2];
	//row and column size
	int S0 = strides[0] / sizeof(BOOLEAN);
	int S1 = strides[1] / sizeof(BOOLEAN);
	int S2 = strides[2] / sizeof(BOOLEAN);
	for (int row = 0; row < row_length; row++) {
		for (int column = 0; column < column_length; column++) {
			int pos = row*S0 + column*S1;
			result[row*column_length + column] = 
				get_hamming_distance(&(left[pos]), &(right[pos]), census_length);
		}
	}
}