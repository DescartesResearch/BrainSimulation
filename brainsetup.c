#include "brainsetup.h"

#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265

nodetimeseries_t *init_observation_timeseries(const int num_oberservationnodes, 
	const int *x_indices, const int *y_indices, const int num_timeseries_elements)
{
    nodetimeseries_t *series = malloc(num_oberservationnodes * sizeof(nodetimeseries_t));
    for (int i = 0; i < num_oberservationnodes; i++) {
        series[i].x_index = x_indices[i];
        series[i].y_index = y_indices[i];
		series[i].timeseries = malloc(num_timeseries_elements * sizeof(nodeval_t));
		series[i].timeseries_ticks = num_timeseries_elements;
    }
    return series;
}

void init_start_time_state(const int number_nodes_x, const int number_nodes_y, nodeval_t **nodes,
	const int num_start_levels, const nodeval_t *start_levels, const int *start_nodes_x, const int *start_nodes_y)
{
	init_zeros_2d(nodes,number_nodes_x,number_nodes_y);

	//initialize with start levels
	for (int i = 0; i < num_start_levels; i++)
	{
		nodes[start_nodes_x[i]][start_nodes_y[i]] = start_levels[i];
	}
}

nodeinputseries_t* read_input_behavior(const int number_of_inputnodes, const int *x_indices, const int *y_indices,
									   const char **inputnodefilenames, const int *number_of_elements){
	nodeinputseries_t *series = malloc(number_of_inputnodes * sizeof(nodeinputseries_t));
	int i;
	for (i = 0; i < number_of_inputnodes; ++i) {
		series[i].x_index = x_indices[i];
		series[i].y_index = y_indices[i];
		series[i].timeseries = malloc(number_of_elements[i] * sizeof(nodeval_t));
		series[i].timeseries_ticks = number_of_elements[i];
		parse_file(series[i], inputnodefilenames[i]);
	}
	return series;
}

double* generate_sin_time_series(int hz, double tick_ms, int number_of_samples){
	double* series = malloc(number_of_samples * sizeof(double));
	int i;
	for (i = 0; i < number_of_samples; ++i) {
		double arg = PI*hz*2*tick_ms*((double)i)/(1000);
		series[i] = sin(arg);
		printf("Arg: %f, sin(arg): %f\n", arg, series[i]);
	}
	return series;
}
