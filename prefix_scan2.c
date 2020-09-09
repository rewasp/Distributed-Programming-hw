#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE 0
#if VERBOSE
#define LOG_V(...) {fprintf(stdout, __VA_ARGS__);fprintf(stdout, "\n");} 
#else
#define LOG_V(...)
#endif

#define RAND_NUM 10000

int main(int argc, char *argv[])
{

	int rank, tasks, num, i, j, div; // np must be divisible by RAND_NUM
	int *send_buf;
	int *recv_buf;
	int *tmp;
	int val_one, sum;
	double start, end;
	srand((unsigned)time(NULL));
	val_one = 0;
	sum = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &tasks);
	
	div = RAND_NUM/tasks;
	recv_buf = (int*)malloc(div*sizeof(int));

	start = MPI_Wtime();
	
	if(rank==0){
		send_buf = (int*)malloc(RAND_NUM*sizeof(int));
			for(i=0; i<RAND_NUM; i++){
				num = rand();
				send_buf[i] = num % 153;
				LOG_V("rand number is %d, send buf %d is %d\n",num, i, send_buf[i]);
			}
		}
	
	MPI_Scatter(send_buf, div , MPI_INT, recv_buf, div, MPI_INT, 0, MPI_COMM_WORLD);	
	
	for(i=0;i<div;i++){
		val_one += recv_buf[i];
	}
	MPI_Scan(&val_one, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
	tmp = (int*)malloc(div*sizeof(int));

	for(i=div-2; i>-1; i--){
		tmp[i] = sum - recv_buf[i];
		recv_buf[i-1] += recv_buf[i];
	}
	tmp[div-1] = sum;

	MPI_Gather(tmp, div, MPI_INT, send_buf, div, MPI_INT, 0, MPI_COMM_WORLD);

	end = MPI_Wtime();
	printf("rank %d: Elapsed time is %lf\n",rank, end - start);

	if(rank==0){
		for(i=0;i<RAND_NUM;i++){
			printf("%d => %d\n",i, send_buf[i]);
		}
		free(send_buf);
	}

	free(tmp);
	free(recv_buf);
	MPI_Finalize();
		
}
