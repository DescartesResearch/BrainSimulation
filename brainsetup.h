#ifndef BRAINSETUP_H
#define BRAINSETUP_H

#include "brainsimulation.h"

/**
 * Setup all node arrays to be passed to the brainsimulation.
 */

/**
 * Initializes num_obervationnodes timeseries structs and returns them in an array.
 * @param num_obervationnodes The number of timeseries to create.
 * @param x_indices The x indices of the nodes to observe. Must have num_obervationnodes as length.
 * @param y_indices The y indices of the nodes to observe. Must have num_obervationnodes as length.
 * @return Array of newly initialized timeseries structs. Array has num_oberservationnodes as length.
 */
t_nodetimeseries *init_observation_timeseries(int num_oberservationnodes,
                                              const int *x_indices, const int *y_indices);

/**
 * Sets a start time energy state for the node field. All unspecified nodes start with 0.
 * @param number_nodes_x The x dimension of the node field.
 * @param number_nodes_y The y dimension of the node field.
 * @param nodes The 2D node field to initialize.
 * @param num_start_levels The number of nodes in the field to initialize with non-zero values.
 * @param start_levels The energy levels of the starting non-zero nodes. Must have num_start_levels length.
 * @param start_nodes_x The x indices of the starting non-zero nodes. Must have num_start_levels length.
 * @param start_nodes_y The y indices of the starting non-zero nodes. Must have num_start_levels length.
 */
void init_start_time_state(int number_nodes_x, int number_nodes_y, t_nodeval **nodes,
                           int num_start_levels, t_nodeval *start_levels, int *start_nodes_x, int *start_node_y);

#endif
