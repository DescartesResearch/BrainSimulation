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
 */
t_nodetimeseries * init_observation_timeseries(int num_oberservationnodes, const int * x_indices, const int * y_indices);

#endif
