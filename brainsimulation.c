#include "brainsimulation.h"
#include "nodefunc.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * Static system environment information initialized at first simulation run.
 * Remains constant during execution.
 */
static int num_threads;

// implement the actual simulation here
unsigned int simulate(int tick_ms, int num_ticks, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
             int num_obervationnodes, nodetimeseries_t *observationnodes)
{
	num_threads = system_processor_online_count() * THREADFACTOR;
    printf("Starting simulation.\n");
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %d\n", tick_ms);
    printf("Number of observation nodes: %d\n", num_obervationnodes);
    printf("Observation nodes: %d\n", num_obervationnodes);
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    int i = 0;
    for (i = 0; i < num_obervationnodes; ++i)
	{
        printf("(%d;%d), ", observationnodes[i].x_index, observationnodes[i].y_index);
    }
    printf("\n");
    // Starting simulation
    // initializing memory
    nodeval_t **new_state = alloc_2d(number_nodes_x, number_nodes_y);
    nodeval_t ****kernels = alloc_4d(number_nodes_x, number_nodes_y, 2, 4);

    int j;
    for (j = 0; j < num_ticks; ++j)
	{
        int returncode = execute_tick(tick_ms, number_nodes_x, number_nodes_y, old_state, new_state, kernels);
		if (!(j % 10))
		{
			printf("Executed tick %d.\n", j);
		}
        if (returncode != 0) {
            printf("Executing tick %d failed with return code %d. Aborting simulation.\n", j, returncode);
            return returncode;
        }
        extract_observationnodes(j, num_obervationnodes, observationnodes, new_state);
        // swap array states -> the new_state becomes the old_state, old_state can be overwritten
        nodeval_t **tmp = old_state;
        old_state = new_state;
        new_state = tmp;
    }
    printf("Simulation finished succesfully!\n");
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", cpu_time_used);
    return 0;
}

unsigned int execute_tick(int tick_ms, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
                 nodeval_t **new_state, nodeval_t ****kernels)
{
	if (MULTITHREADING) {
		threadhandle_t ** handles = malloc(num_threads * sizeof(threadhandle_t *));
		for (int i = 0; i < num_threads; i++)
		{
			int thread_start_x = (i * number_nodes_x) / num_threads;
			int thread_end_x = ((i + 1) * number_nodes_x) / num_threads;
			partialtickcontext_t context;
			init_partial_tick_context(&context, tick_ms, number_nodes_x, number_nodes_y, old_state,
				new_state, kernels, thread_start_x, thread_end_x);
			handles[i] = create_and_run_simulation_thread(execute_partial_tick, &context);
		}
		join_and_close_simulation_threads(handles, num_threads);
		free(handles);
	}
	else
	{
		partialtickcontext_t context;
		init_partial_tick_context(&context, tick_ms, number_nodes_x, number_nodes_y, old_state,
			new_state, kernels, 0, number_nodes_x);
		execute_partial_tick(&context);
	}
	return 0;
}

unsigned int execute_partial_tick(partialtickcontext_t * context)
{
	for (int i = context->thread_start_x; i < context->thread_end_x; ++i)
	{
		for (int j = 0; j < context->number_nodes_y; ++j)
		{
			d_kernel(context->kernels[i][j][0], context->number_nodes_x,
				context->number_nodes_y, context->old_state, i, j);
			id_kernel(context->kernels[i][j][1], context->number_nodes_x,
				context->number_nodes_y, context->old_state, i, j);
			// we do not know what slope is, yet.
			nodeval_t slope_old = 0;
			context->new_state[i][j]
				= process(context->old_state[i][j], slope_old, 4,
				context->kernels[i][j][0], 4, context->kernels[i][j][1]);
		}
	}
	return 0;
}

void extract_observationnodes(int ticknumber, int num_obervationnodes, nodetimeseries_t *observationnodes,
                              nodeval_t **state)
{
    for (int i = 0; i < num_obervationnodes; ++i)
	{
        observationnodes[i].timeseries[ticknumber] = state[observationnodes[i].x_index][observationnodes[i].y_index];
    }
    return;
}

void d_kernel(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int x, int y)
{
    if (x - 1 >= 0) {
        result[0] = nodegrid[x - 1][y];
    } else {
        // border behavior
        result[0] = 0;
    }
    if (y - 1 >= 0) {
        result[1] = nodegrid[x][y - 1];
    } else {
        // border behavior
        result[1] = 0;
    }
    if (y + 1 < number_nodes_y) {
        result[2] = nodegrid[x][y + 1];
    } else {
        // border behavior
        result[2] = 0;
    }
    if (x + 1 < number_nodes_x) {
        result[3] = nodegrid[x + 1][y];
    } else {
        // border behavior
        result[3] = 0;
    }
}

void id_kernel(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int x, int y)
{
    if (x - 1 >= 0 && y - 1 >= 0) {
        result[0] = nodegrid[x - 1][y - 1];
    } else {
        // border behavior
        result[0] = 0;
    }
    if (x - 1 >= 0 && y + 1 < number_nodes_y) {
        result[1] = nodegrid[x - 1][y + 1];
    } else {
        // border behavior
        result[1] = 0;
    }
    if (x + 1 < number_nodes_x && y - 1 >= 0) {
        result[2] = nodegrid[x + 1][y - 1];
    } else {
        // border behavior
        result[2] = 0;
    }
    if (x + 1 < number_nodes_x && y + 1 < number_nodes_y) {
        result[3] = nodegrid[x + 1][y + 1];
    } else {
        // border behavior
        result[3] = 0;
    }
}