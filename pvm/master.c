#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pvm3.h"

/*void populateBuffer(int *buffer, int n)
{
}*/

int main()
{
	int number_of_elements = 20;
	int number_of_slaves = 8;
	int error = 0;
	int tids[100] = { 0 };
	int buffer_id = 0;
	int split_size = 10;

	int buffer[100] = { 0 };
	for(int i = 0 ; i < number_of_elements; i++) {
		buffer[i] = i + 1;
	}
	//populateBuffer(buffer, number_of_elements);

	int my_task_id = pvm_mytid();

	if(my_task_id < 0) {
		printf("Error");
		exit(-1);
	}

	error = pvm_spawn("/home/suprasath/worker", NULL, 0, "", number_of_slaves, &tids);

	if(error == number_of_slaves) {
		buffer_id = pvm_initsend(PvmDataDefault);

		for (int i = 0; i < number_of_elements / split_size; i++) {
			int temp_buffer[split_size];
			for (int j = 0; j < split_size; j++) {
				temp_buffer[j] = buffer[i * split_size + j];
			}

			error = pvm_pkint(&split_size, 1, 1);
			error = pvm_pkint(temp_buffer, split_size, 1);

			pvm_mcast(&tids[i * 4], 4, i + 3);
		}

		long long start = 0, end = 0, x = 0;
		long long sum = 0;
		int max = 0;
		int min = INT_MAX;
		long long product = 1;

		for(int i = 0; i < number_of_slaves; i++) {
			pvm_recv(tids[i], 2);
			pvm_upkint(&x, 1, 1);
			printf("%d %d\n", tids[i], x);

			switch (tids[i] % 4) {
				case 0:
					sum += x;
					break;
				case 1:
					if (x > max) {
						max = x;
					}
					break;
				case 2:
					if (x < min) {
						min = x;
					}
					break;
				case 3:
					product *= x;
					break;
			}
		}

		printf("Result\nSum=%lld\nMax=%lld\nMin=%lld\nProduct=%lld\n", sum, max, min, product);
	}
	else {
		printf("Error");
	}

	pvm_exit();

	return 0;
}
