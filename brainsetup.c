#include "brainsetup.h"

#include <stdlib.h>

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
	//initialize all nodes with 0
	for (int i = 0; i < number_nodes_x; i++)
	{
		for (int j = 0; j < number_nodes_y; j++)
		{
			nodes[i][j] = 0.0;
		}
	}

	//initialize with start levels
	for (int i = 0; i < num_start_levels; i++)
	{
		nodes[start_nodes_x[i]][start_nodes_y[i]] = start_levels[i];
	}
}
