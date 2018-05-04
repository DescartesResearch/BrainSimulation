#include "brainsetup.h"

#include <stdlib.h>

t_nodetimeseries *init_observation_timeseries(int num_oberservationnodes, const int *x_indices, const int *y_indices) {
    t_nodetimeseries *series = malloc(num_oberservationnodes * sizeof(t_nodetimeseries));
    for (int i = 0; i < num_oberservationnodes; i++) {
        series[i].x_index = x_indices[i];
        series[i].y_index = y_indices[i];
    }
    return series;
}
