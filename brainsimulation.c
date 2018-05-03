#include "brainsimulation.h"
#include "nodefunc.h"

// implement the actual simulation here
int simulate(int tick_ms, int num_ticks, t_nodeval ** nodes, int num_obervationnodes,
             t_nodetimeseries * oberservationnodes){
    printf("Starting simulation.\n");
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %d\n", tickms);
    printf("Number of observation nodes: %d\n", num_obervationnodes);
    printf("Observation nodes: %d\n", num_obervationnodes);
    int i = 0;
    for (i = 0; i < num_obervationnodes; ++i) {
        printf("(%d;%d), ", obervationnodes[i].x_index, obervationnodes[i].y_index);
    }
    printf("\n");
    return 0;
}