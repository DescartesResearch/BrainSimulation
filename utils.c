#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#ifdef _WIN32
	#include <Windows.h>
	#include <process.h>
#else
	#include <unistd.h>
#endif


nodeval_t **alloc_2d(const int m, const int n)
{
	nodeval_t **arr = malloc(m * sizeof(*arr));
	int i;
	for (i = 0; i < m; ++i)
	{
		arr[i] = malloc(n * sizeof(nodeval_t));
	}
	return arr;
}

nodeval_t ****alloc_4d(const int m, const int n, const int o, const int p)
{
	int i, j, k;
	nodeval_t ****arr = malloc(m * sizeof(***arr));
	for (i = 0; i < m; i++)
	{
		arr[i] = malloc(n * sizeof(**arr));
		for (j = 0; j < n; j++)
		{
			arr[i][j] = malloc(o * sizeof(*arr));
			for (k = 0; k < o; k++)
			{
				arr[i][j][k] = malloc(p * sizeof(nodeval_t));
			}
		}
	}
	return arr;
}

void init_zeros_2d(nodeval_t **nodes, int number_nodes_x, int number_nodes_y){
	//initialize all nodes with 0
	for (int i = 0; i < number_nodes_x; i++)
	{
		for (int j = 0; j < number_nodes_y; j++)
		{
			nodes[i][j] = 0.0;
		}
	}
}



int system_processor_online_count()
{
	#ifdef _WIN32
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	#else
		return sysconf(_SC_NPROCESSORS_ONLN);
	#endif
}

threadhandle_t * create_and_run_simulation_thread(unsigned int(*callback)(partialtickcontext_t *), partialtickcontext_t * context)
{
	threadhandle_t * handle = malloc(sizeof(threadhandle_t));
	#ifdef _WIN32
		*handle = (threadhandle_t) _beginthreadex(0, 0, callback, context, 0, 0);
	#else
		int ret = pthread_create(handle, NULL, (void * (*) (void * )) callback, context);
		if (ret)
		{
			printf("Error initializing thread. Error code %d.\n", ret);
			return NULL;
		}
	#endif
	return handle;
}

void join_and_close_simulation_threads(threadhandle_t ** handles, const int num_threads)
{
	for (int i = 0; i < num_threads; i++)
	{
		#ifdef _WIN32
			WaitForSingleObject(*handles[i], INFINITE);
		#else
			pthread_join(*handles[i], NULL);
		#endif
	}
	for (int i = 0; i < num_threads; i++)
	{
		#ifdef _WIN32
			CloseHandle(*handles[i]);
		#endif
		free(handles[i]);
	}
}

void init_partial_tick_context(partialtickcontext_t * context, double tick_ms,
	int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
	nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels,
	kernelfunc_t d_ptr, kernelfunc_t id_ptr,
	int thread_start_x, int thread_end_x)
{
	context->tick_ms = tick_ms;
	context->number_nodes_x = number_nodes_x;
	context->number_nodes_y = number_nodes_y;
	context->old_state = old_state;
	context->new_state = new_state;
	context->slopes = slopes;
	context->kernels = kernels;
	context->d_ptr = d_ptr;
	context->id_ptr = id_ptr;
	context->thread_start_x = thread_start_x;
	context->thread_end_x = thread_end_x;
}


void output_to_csv(char *filename, int length, nodeval_t *values){
	printf("Creating %s file\n",filename);
	FILE *fp = fopen(filename,"w+");
	if (fp==NULL){
		printf("File is null.\n Error: %s\n",strerror(errno));

	} else {
		fprintf(fp,"Energy-value,");
		int i;
		for(i=0;i<length;i++){
			fprintf(fp,"\n%f,",values[i]);
		}
		fclose(fp);
		printf("%s file created.\n",filename);
	}
}
