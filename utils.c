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

#ifdef _WIN32
static const unsigned __int64 EPOCH = ((unsigned __int64)116444736000000000ULL);
#endif

nodeval_t **alloc_2d(const int m, const int n) {
    nodeval_t **arr = malloc(m * sizeof(*arr));
    int i;
    for (i = 0; i < m; ++i) {
        arr[i] = malloc(n * sizeof(nodeval_t));
    }
    return arr;
}

nodeval_t ****alloc_4d(const int m, const int n, const int o, const int p) {
    int i, j, k;
    nodeval_t ****arr = malloc(m * sizeof(***arr));
    for (i = 0; i < m; i++) {
        arr[i] = malloc(n * sizeof(**arr));
        for (j = 0; j < n; j++) {
            arr[i][j] = malloc(o * sizeof(*arr));
            for (k = 0; k < o; k++) {
                arr[i][j][k] = malloc(p * sizeof(nodeval_t));
            }
        }
    }
    return arr;
}

void init_zeros_2d(nodeval_t **nodes, int number_nodes_x, int number_nodes_y) {
    //initialize all nodes with 0
    for (int i = 0; i < number_nodes_x; i++) {
        for (int j = 0; j < number_nodes_y; j++) {
            nodes[i][j] = 0.0;
        }
    }
}


const unsigned int system_processor_online_count() {
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

threadhandle_t *
create_and_run_simulation_thread(unsigned int(*callback)(partialsimulationcontext_t *),
                                 partialsimulationcontext_t *context) {
    threadhandle_t *handle = malloc(sizeof(threadhandle_t));
#ifdef _WIN32
    *handle = (threadhandle_t) _beginthreadex(0, 0, callback, context, 0, 0);
#else
    int ret = pthread_create(handle, NULL, (void *(*)(void *)) callback, context);
    if (ret) {
        printf("Error initializing thread. Error code %d.\n", ret);
        return NULL;
    }
#endif
    return handle;
}

void join_and_close_simulation_threads(threadhandle_t **handles, const int num_threads) {
    for (int i = 0; i < num_threads; i++) {
#ifdef _WIN32
        WaitForSingleObject(*handles[i], INFINITE);
#else
        pthread_join(*handles[i], NULL);
#endif
    }
    for (int i = 0; i < num_threads; i++) {
#ifdef _WIN32
        CloseHandle(*handles[i]);
#endif
        free(handles[i]);
    }
}

void init_thread_barrier(threadbarrier_t *barrier, const unsigned int number_threads) {
#ifdef _WIN32
    InitializeSynchronizationBarrier(barrier, number_threads, -1);
#else
    pthread_barrier_init(barrier, NULL, number_threads);
#endif
}

void destroy_thread_barrier(threadbarrier_t *barrier) {
#ifdef _WIN32
    DeleteSynchronizationBarrier(barrier);
#else
    pthread_barrier_destroy(barrier);
#endif
}

unsigned int wait_at_barrier(threadbarrier_t *barrier) {
#ifdef _WIN32
    return EnterSynchronizationBarrier(barrier, 0);
#else
    if (pthread_barrier_wait(barrier) == PTHREAD_BARRIER_SERIAL_THREAD) {
        return 1;
    } else {
        return 0;
    }
#endif
}

void init_partial_simulation_context(partialsimulationcontext_t *context, int num_ticks, double tick_ms,
					int number_nodes_x, int number_nodes_y,
					int num_global_obervationnodes, nodetimeseries_t *global_observationnodes,
					nodeval_t **old_state,
					nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels,
					kernelfunc_t d_ptr, kernelfunc_t id_ptr,
					int number_global_inputs, nodeinputseries_t *global_inputs,
					int thread_start_x, int thread_end_x, threadbarrier_t *barrier) {
    context->num_ticks = num_ticks;
    context->tick_ms = tick_ms;
    context->number_nodes_x = number_nodes_x;
    context->number_nodes_y = number_nodes_y;
	context->num_global_obervationnodes = num_global_obervationnodes;
    context->global_observationnodes = global_observationnodes;
    context->old_state = old_state;
    context->new_state = new_state;
    context->slopes = slopes;
    context->kernels = kernels;
    context->d_ptr = d_ptr;
    context->id_ptr = id_ptr;
    context->number_global_inputs = number_global_inputs;
    context->global_inputs = global_inputs;
    context->thread_start_x = thread_start_x;
    context->thread_end_x = thread_end_x;
    context->barrier = barrier;

	//derive the partial observation nodes
	context->num_partial_obervationnodes = 0;
	for (int i = 0; i < num_global_obervationnodes; i++) { //count partial array elements
		if (global_observationnodes != NULL
			&& global_observationnodes[i].x_index >= thread_start_x && global_observationnodes[i].x_index < thread_end_x) {
			context->num_partial_obervationnodes++;
		}
	}
	context->partial_observationnodes = malloc(context->num_partial_obervationnodes * sizeof(nodetimeseries_t *));
	int j = 0;
	for (int i = 0; i < num_global_obervationnodes; i++) {
		if (global_observationnodes != NULL
			&& global_observationnodes[i].x_index >= thread_start_x && global_observationnodes[i].x_index < thread_end_x) {
			context->partial_observationnodes[j] = &(global_observationnodes[i]);
			j++;
		}
	}

    //derive the partial inputs
	context->number_partial_inputs = 0;
	for (int i = 0; i < number_global_inputs; i++) { //count partial array elements
		if (global_inputs != NULL
			&& global_inputs[i].x_index >= thread_start_x && global_inputs[i].x_index < thread_end_x) {
			context->number_partial_inputs++;
		}
	}
	context->partial_inputs = malloc(context->number_partial_inputs * sizeof(nodeinputseries_t *));
	j = 0;
	for (int i = 0; i < number_global_inputs; i++) {
		if (global_inputs != NULL
			&& global_inputs[i].x_index >= thread_start_x && global_inputs[i].x_index < thread_end_x) {
			context->partial_inputs[j] = &(global_inputs[i]);
			j++;
		}
	}
}

void init_executioncontext(executioncontext_t *context) {
    if (MULTITHREADING) {
        context->num_threads = (int) (THREADFACTOR * system_processor_online_count());
    } else {
        context->num_threads = 1;
    }
    context->handles = malloc(context->num_threads * sizeof(threadhandle_t *));
    context->contexts = malloc(context->num_threads * sizeof(partialsimulationcontext_t));
}

void output_to_csv(char *filename, int length, nodeval_t *values) {
    printf("Creating %s file\n", filename);
    FILE *fp = fopen(filename, "w+");
    if (fp == NULL) {
        printf("File is null.\n Error: %s\n", strerror(errno));

    } else {
        fprintf(fp, "Energy-value,");
        int i;
        for (i = 0; i < length; i++) {
            fprintf(fp, "\n%f,", values[i]);
        }
        fclose(fp);
        printf("%s file created.\n", filename);
    }
}

int get_daytime(struct timeval *tp) {
#ifdef _WIN32
    /*
    * Taken from PostgerSQL's gettimeofday.c
    *    Win32 gettimeofday() replacement
    *
    * src/port/gettimeofday.c
    *
    * Copyright (c) 2003 SRA, Inc.
    * Copyright (c) 2003 SKC, Inc.
    *
    * Permission to use, copy, modify, and distribute this software and
    * its documentation for any purpose, without fee, and without a
    * written agreement is hereby granted, provided that the above
    * copyright notice and this paragraph and the following two
    * paragraphs appear in all copies.
    *
    * IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
    * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
    * LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
    * DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED
    * OF THE POSSIBILITY OF SUCH DAMAGE.
    *
    * THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
    * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    * A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS
    * IS" BASIS, AND THE AUTHOR HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE,
    * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
    */
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (long)((ularge.QuadPart - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

    return 0;
#else
    return gettimeofday(tp, NULL);
#endif
}

void parse_file(nodeinputseries_t timeseries, const char *filename) {
    FILE *stream = fopen(filename, "r");
    int i = 0;
    char line[1024];
    while (fgets(line, 1024, stream)) {
        char *tmp = strdup(line);
        timeseries.timeseries[i++] = get_field(tmp, 1);
        free(tmp);
    }
    printf("Read %d lines for file %s.\n", i, filename);
}

int get_field(char *line, int num) {
    // parsing to integer
    int integer = atoi(line);
    return integer;
}
