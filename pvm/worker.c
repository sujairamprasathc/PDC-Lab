#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include "pvm3.h"

int calculate_sum(int *array)
{
	int sum = 0;
	for (int i = 0; i < 10; i++) {
                sum+=array[i];
        }
	return sum;
}

int calculate_max(int *array)
{
	int max = 0;
	for (int i = 0; i < 10; i++) {
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

int calculate_min(int *array)
{
	int min = INT_MAX;
	for (int i = 0; i < 10; i++) {
		if (array[i] < min) {
			min = array[i];
		}
	}
	return min;
}

int calculate_product(int *array)
{
	int product = 1;
	for (int i = 0; i < 10; i++) {
		product *= array[i];
	}
	return product;
}

int main()
{
        int ptid, i = 0, res = 0, N, mytid;

        ptid = pvm_parent();
        mytid = pvm_mytid();

        pvm_recv(ptid, -1);

	int split_size = 0;
	pvm_upkint(&split_size, 1, 1);

        int ptr[split_size];
        pvm_upkint(&ptr, split_size, 1);

        pvm_initsend(PvmDataDefault);
	if (mytid % 4 == 0) {
		res = calculate_sum(ptr);
	}
	else if (mytid % 4 == 1) {
		res = calculate_max(ptr);
	}
	else if (mytid % 4 == 2) {
		res = calculate_min(ptr);
	}
	else {
		res = calculate_product(ptr);
	}

        pvm_pkint(&res,1,1);
        pvm_send(ptid,2);

        pvm_exit();

        return 0;
}
