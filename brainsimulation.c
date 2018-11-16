#include "brainsimulation.h"
#include "nodefunc.h"
#include "kernels.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// implement the actual simulation here
unsigned int simulate(double tick_ms, int num_ticks, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
                      int num_obervationnodes, nodetimeseries_t *observationnodes, int number_inputs,
                      nodeinputseries_t *inputs) {
    executioncontext_t executioncontext;
    init_executioncontext(&executioncontext);
    printf("Starting simulation.\n");
    printf("Grid size: %d x %d => %d simulated nodes.\n", number_nodes_x, number_nodes_y, number_nodes_x *
                                                                                          number_nodes_y);
    printf("Number of ticks: %d\n", num_ticks);
    printf("Length of each tick (ms): %f\n", tick_ms);
    printf("Number of threads: %d\n", executioncontext.num_threads);
	printf("Number of observation nodes: %d\n", num_obervationnodes);
    struct timeval tv1, tv2;
    get_daytime(&tv1);
	if (num_obervationnodes == number_nodes_x * number_nodes_y) {
		printf("WARNING: Observing all nodes. This is very slow and result files will occupy a lot of disk space.\n");
	} else {
		for (int i = 0; i < num_obervationnodes; ++i) {
			printf("(%d;%d), ", observationnodes[i].x_index, observationnodes[i].y_index);
		}
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

#if MULTITHREADING
    execute_simulation_multithreaded(&executioncontext, num_ticks,
                                     tick_ms, number_nodes_x, number_nodes_y, num_obervationnodes, observationnodes,
                                     old_state, new_state, slopes, kernels,
                                     d_kernel, id_kernel, number_inputs, inputs);
#else
    execute_simulation_singlethreaded(&executioncontext, num_ticks,
        tick_ms, number_nodes_x, number_nodes_y, num_obervationnodes, observationnodes,
        old_state, new_state, slopes, kernels,
        d_kernel, id_kernel, number_inputs, inputs);
#endif
    printf("Simulation finished succesfully!\n");
    get_daytime(&tv2);
    printf("Total time = %f seconds\n",
           (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
           (double) (tv2.tv_sec - tv1.tv_sec));
    return 0;
}

unsigned int execute_simulation_multithreaded(executioncontext_t *executioncontext,
                                              int num_ticks, double tick_ms, int number_nodes_x, int number_nodes_y,
                                              int num_obervationnodes, nodetimeseries_t *observationnodes,
                                              nodeval_t **old_state,
                                              nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels,
                                              kernelfunc_t d_ptr, kernelfunc_t id_ptr,
                                              int number_global_inputs, nodeinputseries_t *global_inputs) {
    //initialize barrier
    init_thread_barrier(&executioncontext->barrier, executioncontext->num_threads);
    //spawn threads
    for (int i = 0; i < executioncontext->num_threads; i++) {
        int thread_start_x = (i * number_nodes_x) / executioncontext->num_threads;
        int thread_end_x = ((i + 1) * number_nodes_x) / executioncontext->num_threads;
        init_partial_simulation_context(&executioncontext->contexts[i],
                                        num_ticks, tick_ms, number_nodes_x, number_nodes_y,
                                        num_obervationnodes, observationnodes, old_state,
                                        new_state, slopes, kernels, d_ptr, id_ptr, number_global_inputs, global_inputs,
                                        thread_start_x, thread_end_x, &executioncontext->barrier);
        executioncontext->handles[i] =
                create_and_run_simulation_thread(execute_partial_simulation, &executioncontext->contexts[i]);
    }
    //wait for threads to finish
    join_and_close_simulation_threads(executioncontext->handles, executioncontext->num_threads);
    destroy_thread_barrier(&executioncontext->barrier);
    return 0;
}

unsigned int execute_simulation_singlethreaded(executioncontext_t *executioncontext,
                                               int num_ticks, double tick_ms, int number_nodes_x, int number_nodes_y,
                                               int num_obervationnodes, nodetimeseries_t *observationnodes,
                                               nodeval_t **old_state,
                                               nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels,
                                               kernelfunc_t d_ptr, kernelfunc_t id_ptr,
                                               int number_global_inputs, nodeinputseries_t *global_inputs) {
    init_partial_simulation_context(executioncontext->contexts,
                                    num_ticks, tick_ms, number_nodes_x, number_nodes_y,
                                    num_obervationnodes, observationnodes, old_state,
                                    new_state, slopes, kernels, d_ptr, id_ptr, number_global_inputs, global_inputs,
                                    0, number_nodes_x, &executioncontext->barrier);
    return execute_partial_simulation(executioncontext->contexts);
}

unsigned int execute_partial_simulation(partialsimulationcontext_t *context) {
    for (int j = 0; j < context->num_ticks; j++) {
        int returncode = execute_partial_tick(context);
        if (returncode != 0) {
            printf("Executing tick %d failed with return code %d. Aborting simulation.\n", j, returncode);
            return returncode;
        }
        //waiting at barrier, this returns 1 only if this thread has been selected as the "management" thread
#if MULTITHREADING
        if (wait_at_barrier(context->barrier)) {
#endif
            if (!(j % 100)) {
                printf("Executed tick %d.\n", j);
            }
#if MULTITHREADING
        }
#endif
        // add the input signals AFTER the actual computation takes place.
        process_partial_inputs(j, context->tick_ms, context->new_state, context->number_partial_inputs,
                               context->partial_inputs);
        //extract observation nodes
#if MULTITHREADING
		wait_at_barrier(context->barrier);
#endif
        extract_observationnodes(j, context->num_partial_obervationnodes,
				context->partial_observationnodes, context->new_state);
        //everyone swaps their own pointers
        // swap array states -> the new_state becomes the old_state, old_state can be overwritten
        nodeval_t **tmp = context->old_state;
        context->old_state = context->new_state;
        context->new_state = tmp;

        //wait at the barrier again before starting the next cycle, this makes sure that no one writes during observation extraction
#if MULTITHREADING
        wait_at_barrier(context->barrier);
#endif
    }
    return 0;
}

unsigned int execute_partial_tick(partialsimulationcontext_t *context) {
    for (int i = context->thread_start_x; i < context->thread_end_x; ++i) {
        for (int j = 0; j < context->number_nodes_y; ++j) {
            // call the given kernel functions for calculating the kernel
            int d_count = (*(context->d_ptr))(context->kernels[i][j][0], context->number_nodes_x,
                                              context->number_nodes_y, context->old_state, i, j);
            int id_count = (*(context->id_ptr))(context->kernels[i][j][1], context->number_nodes_x,
                                                context->number_nodes_y, context->old_state, i, j);
            // execute one node
            nodestate_t res = process(context->old_state[i][j],
                                      context->slopes[i][j], d_count, context->kernels[i][j][0], id_count,
                                      context->kernels[i][j][1]);
            // store result
            context->new_state[i][j] = res.act;
            context->slopes[i][j] = res.slope;
        }
    }
    return 0;
}

void extract_observationnodes(int ticknumber, int num_obervationnodes, nodetimeseries_t **observationnodes,
                              nodeval_t **state) {
    for (int i = 0; i < num_obervationnodes; ++i) {
        observationnodes[i]->timeseries[ticknumber] = state[observationnodes[i]->x_index][observationnodes[i]->y_index];
    }
    return;
}

void process_global_inputs(int tick_number, double tick_ms,
                           nodeval_t **state, int number_global_inputs, nodeinputseries_t *global_inputs) {
    for (int i = 0; i < number_global_inputs; ++i) {
        process_input(tick_number, tick_ms, state, &global_inputs[i]);
    }
}

void process_partial_inputs(int tick_number, double tick_ms,
                            nodeval_t **state, int number_partial_inputs, nodeinputseries_t **partial_inputs) {
    for (int i = 0; i < number_partial_inputs; ++i) {
        process_input(tick_number, tick_ms, state, partial_inputs[i]);
    }
}

void process_input(int tick_number, double tick_ms,
                   nodeval_t **state, nodeinputseries_t *input) {
    int x = input->x_index;
    int y = input->y_index;
    nodeval_t increase = input->timeseries[tick_number % input->timeseries_ticks];
    // printf("Increased node (%d|%d). State before %f, state now: %f.\n", x, y,
    // 	   state[x][y], state[x][y] + increase);
    state[x][y] = state[x][y] + increase;
}

//void add_input_influence(int tick_number, double tick_ms, int number_nodes_x, int number_nodes_y,
//                             nodeval_t **state){
//    // every 50th tick -> add burst input at node 20, 20 of value 100
//    int tick_period = 50;
//    int x_node = 20;
//    int y_node = 20;
//    nodeval_t increase = 100;
//    if(tick_number % (int) (tick_period / tick_ms) == 0){
//        printf("Increased node (%d|%d). State before %f, state now: %f.\n", x_node, y_node, state[x_node][y_node],
//               state[x_node][y_node] + increase);
//        state[x_node][y_node] = state[x_node][y_node] + increase;
//    }
//}
