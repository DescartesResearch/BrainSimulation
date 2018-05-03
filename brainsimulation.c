#include "brainsimulation.h"
#include <stdio.h>
// implement the actual simulation here
int simulate(int tick_ms, int num_ticks, int number_nodes_x, int number_nodes_y, t_nodeval ** nodes,
             int num_obervationnodes, t_nodetimeseries * observationnodes){
    printf("Starting simulation.\n");
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %d\n", tick_ms);
    printf("Number of observation nodes: %d\n", num_obervationnodes);
    printf("Observation nodes: %d\n", num_obervationnodes);
    int i = 0;
    for (i = 0; i < num_obervationnodes; ++i) {
        printf("(%d;%d), ", observationnodes[i].x_index, observationnodes[i].y_index);
    }
    printf("\n");
    return 0;
}