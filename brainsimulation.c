#include "brainsimulation.h"
#include "nodefunc.h"
#include "utils.h"
#include "kernels.h"

#include <stdio.h>
#include <time.h>

// implement the actual simulation here
int simulate(double tick_ms, int num_ticks, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
             int num_obervationnodes, nodetimeseries_t *observationnodes) {
    printf("Starting simulation.\n");
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %f\n", tick_ms);
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
    // initializing memory
    nodeval_t **new_state = alloc_2d(number_nodes_x, number_nodes_y);
    nodeval_t **slopes = alloc_2d(number_nodes_x, number_nodes_y);
    init_zeros_2d(slopes, number_nodes_x, number_nodes_y);
    nodeval_t ****kernels = alloc_4d(number_nodes_x, number_nodes_y, 2, 4);

    kernelfunc_t d_kernel = d_kernel_function_factory("");
    kernelfunc_t id_kernel = id_kernel_function_factory("");

    int j;
    for (j = 0; j < num_ticks; ++j) {
        int returncode = execute_tick(tick_ms, number_nodes_x, number_nodes_y, old_state, new_state, slopes, kernels,
                d_kernel, id_kernel);
        printf("Executed tick %d.\n", j);
        if (returncode != 0) {
            printf("Executing tick %d failed with return code %d. Aborting simulation.\n", j, returncode);
            return returncode;
        }
        // add the input signals AFTER the actual computation takes place.
        // Change location?
        add_input_influence(j, tick_ms, number_nodes_x, number_nodes_y, new_state);
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

int execute_tick(double tick_ms, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
                 nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels, kernelfunc_t d_ptr, kernelfunc_t id_ptr) {
    for (int i = 0; i < number_nodes_x; ++i) {
        for (int j = 0; j < number_nodes_y; ++j) {
            // call the given kernel functions for calculating the kernel
            int d_count = (*d_ptr)(kernels[i][j][0], number_nodes_x, number_nodes_y, old_state, i, j);
            int id_count =(*id_ptr)(kernels[i][j][1], number_nodes_x, number_nodes_y, old_state, i, j);
            // execute one node
            nodestate_t res = process(old_state[i][j], slopes[i][j], d_count, kernels[i][j][0], id_count,
                                      kernels[i][j][1]);
            // store result
            new_state[i][j] = res.act;
            slopes[i][j] = res.slope;
        }
    }
    return 0;
}

void extract_observationnodes(int ticknumber, int num_obervationnodes, nodetimeseries_t *observationnodes,
                              nodeval_t **state) {
    for (int i = 0; i < num_obervationnodes; ++i) {
        observationnodes[i].timeseries[ticknumber] = state[observationnodes[i].x_index][observationnodes[i].y_index];
    }
    return;
}

void add_input_influence(int tick_number, double tick_ms, int number_nodes_x, int number_nodes_y,
                             nodeval_t **state){
    // every 50th tick -> add burst input at node 20, 20 of value 100
    int tick_period = 50;
    int x_node = 20;
    int y_node = 20;
    nodeval_t increase = 100;
    if(tick_number % (int) (tick_period / tick_ms) == 0){
        printf("Increased node (%d|%d). State before %f, state now: %f.\n", x_node, y_node, state[x_node][y_node],
               state[x_node][y_node] + increase);
        state[x_node][y_node] = state[x_node][y_node] + increase;
    }
}