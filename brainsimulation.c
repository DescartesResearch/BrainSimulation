#include "brainsimulation.h"
// implement the actual simulation here
int simulate(int tick_ms, int num_ticks, int number_nodes_x, int number_nodes_y, t_nodeval ** old_state,
             int num_obervationnodes, t_nodetimeseries * observationnodes){
    printf("Starting simulation.\n");
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %d\n", tick_ms);
    printf("Number of observation nodes: %d\n", num_obervationnodes);
    printf("Observation nodes: %d\n", num_obervationnodes);
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    int i = 0;
    for (i = 0; i < num_obervationnodes; ++i) {
        printf("(%d;%d), ", observationnodes[i].x_index, observationnodes[i].y_index);
    }
    printf("\n");
    // Starting simulation
    t_nodeval ** new_state = alloc_2d(number_nodes_x, number_nodes_y);

    int j;
    for (j = 0; j < num_ticks; ++j) {
        int returncode = execute_tick(tick_ms, number_nodes_x, number_nodes_y, old_state, new_state);
        printf("Executed tick %d.\n", j);
        if (returncode != 0) {
            printf("Executing tick %d failed with return code %d. Aborting simulation.\n", j, returncode);
            return returncode;
        }
        extract_observationnodes(j, num_obervationnodes, observationnodes, new_state);
        // swap array states -> the new_state becomes the old_state, old_state can be overwritten
        t_nodeval ** tmp = old_state;
        old_state = new_state;
        new_state = tmp;
    }
    printf("Simulation finished succesfully!\n");
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f s.\n", cpu_time_used);
    return 0;
}

t_nodeval ** alloc_2d(int m, int n)
{
    t_nodeval ** arr = malloc(m * sizeof(*arr));
    int i;
    for (i = 0; i < m; ++i)
    {
        arr[i] = malloc(n * sizeof(t_nodeval));
    }
    return arr;
}

int execute_tick(int tick_ms, int number_nodes_x, int number_nodes_y, t_nodeval ** old_state,
                   t_nodeval ** new_state){
    for (int i = 0; i < number_nodes_x; ++i) {
        for (int j = 0; j < number_nodes_y; ++j) {
            // room for optimization -> does this initialization here lead to unnecessary copying and mallocs?
            t_nodeval d[4] = {0};
            t_nodeval id[4] = {0};
            d_kernel(d, number_nodes_x, number_nodes_y, old_state, i, j);
            id_kernel(id, number_nodes_x, number_nodes_y, old_state, i, j);
            // we do not know what slope is, yet.
            t_nodeval slope_old = 0;
            new_state[i][j] = process(old_state[i][j], slope_old, d, id);
        }
    }
    return 0;
}

void extract_observationnodes(int ticknumber, int num_obervationnodes, t_nodetimeseries * observationnodes,
                                t_nodeval ** state){
    for (int i = 0; i < num_obervationnodes; ++i) {
        observationnodes[i].timeseries[ticknumber] = state[observationnodes[i].x_index][observationnodes[i].y_index];
    }
    return;
}

void d_kernel(t_nodeval * result, int number_nodes_x, int number_nodes_y, t_nodeval ** nodegrid, int x, int y){
    if (x - 1 >= 0){
        result[0] = nodegrid[x-1][y];
    }
    if (y - 1 >= 0){
        result[1] = nodegrid[x][y-1];
    }
    if (y + 1 < number_nodes_y){
        result[2] = nodegrid[x][y+1];
    }
    if (x + 1 < number_nodes_x){
        result[3] = nodegrid[x+1][y];
    }
}

void id_kernel(t_nodeval * result, int number_nodes_x, int number_nodes_y, t_nodeval ** nodegrid, int x, int y){
    if (x - 1 >= 0 && y - 1 >= 0){
        result[0] = nodegrid[x-1][y-1];
    }
    if (x - 1 >= 0 && y + 1 < number_nodes_y){
        result[1] = nodegrid[x-1][y+1];
    }
    if (x +1 < number_nodes_x && y - 1 >= 0){
        result[2] = nodegrid[x+1][y-1];
    }
    if (x +1 < number_nodes_x && y + 1 < number_nodes_y){
        result[3] = nodegrid[x+1][y+1];
    }
}