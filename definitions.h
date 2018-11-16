#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef _WIN32
#include <Windows.h>
#else

#include <pthread.h>

#endif

/**
 * @file
 * Common definitions (mostly types).
 */

#ifndef MULTITHREADING
/**
 * 1 if multithreading is enabled, 0 otherwise.
 */
#define MULTITHREADING 1
#endif

#ifndef THREADFACTOR
/**
 * Flouting point factor to modifiy the number of threads in relation to the number
 * of logical processors on the execution system.
 */
#define THREADFACTOR 1
#endif

#ifndef D_NEIGHBORFACTOR
/**
 * Ratio of how much the direct neighbors influence the energy state of any node. This is a factor multiplied with the direct neighbor-energy. See parameter (a1) in the flowchart. Usually a number in (0,1], however numbers > 1
 * are possible. Default is 1.
 */
#define D_NEIGHBORFACTOR 1
#endif

#ifndef ID_NEIGHBORFACTOR
/**
 * Ratio of how much the indirect neighbors influence the energy state of any node. This is a factor multiplied with the indirect neighbor-energy. See parameter (a2) in the flowchart. Usually a number in (0,1], however numbers
 * > 1 are possible. Default is 1.
 */
#define ID_NEIGHBORFACTOR 1
#endif

#ifndef DAMPING
/**
 * DEPRECATED. CURRENTLY IGNORED. The amount of physical damping applied at each simulation step for each energy level, i.e., the ratio of energy loss
 * per simulation tick. The energy level is damped using the function: enery_level = energy_level / (1 + DAMPING).
 * Default: 0.001.
 */
#define DAMPING 0.001
#endif

#ifndef ENERGY_FACTOR
/**
 * Ratio of how much the historical energy state negatively influences the current energy slope. This is a factor multiplied with the historical energy. See parameter (b) in the flowchart.
 * Usually a number in (0,1], however numbers > 1 are possible. Default is 1.
 */
#define ENERGY_FACTOR 1
#endif

#ifndef ENERGY_WEIGHT
/**
 * Ratio of how much the historical energy state influences the current energy state. This is a factor multiplied with the historical energy. See parameter (g) in the flowchart.
 * Usually a number in (0,1], however numbers > 1 are possible. Default is 1.
 */
#define ENERGY_WEIGHT 1
#endif

#ifndef DELTA_FACTOR
/**
 * Ratio of how much the current slope vector influences the slope calculation. This is a factor multiplied with the current slope vector. See parameter (e) in the flowchart.
 * Usually a number in (0,1], however numbers > 1 are possible. Default is 1.
 */
#define DELTA_FACTOR 1
#endif

#ifndef SLOPE_FACTOR
/**
 * Ratio of how much the historical slope influences the current energy state. This is a factor multiplied with the historical slope. See parameter (d) in the flowchart.
 * Usually a number in (0,1], however numbers > 1 are possible. Default is 1.
 */
#define SLOPE_FACTOR 1
#endif

#ifndef SLOPE_WEIGHT
/**
 * Ratio of how much the current calculated slope influences the current energy state. This is a factor multiplied with the slope. See parameter (h) in the flowchart.
 * Usually a number in (0,1], however numbers > 1 are possible. Default is 1.
 */
#define SLOPE_WEIGHT 1
#endif

//types

/**
 * Platform-independent thread handle.
 */
#ifdef _WIN32
typedef HANDLE threadhandle_t;
#else
typedef pthread_t threadhandle_t;
#endif

/**
 * Platform-independent thread barrier.
 */
#ifdef _WIN32
typedef SYNCHRONIZATION_BARRIER threadbarrier_t;
#else
typedef pthread_barrier_t threadbarrier_t;
#endif

/**
 * Type that the nodes in the brainsimulation use to store their energy level.
 */
typedef double nodeval_t;

/**
 * Struct to store the results of a simulation for a single observed node.
 * x_index and y_index members must be set when passing it to a simulation.
 */
typedef struct {
    /**
    * x index of the node.
    */
    int x_index;
    /**
    * y index of the node.
    */
    int y_index;
    /**
    * Series of observed node energy levels. One element per tick.
    * Has #timeseries_ticks as length.
    */
    nodeval_t *timeseries;
    /**
    * Length of #timeseries.
    */
    int timeseries_ticks;
}
        nodetimeseries_t;

/**
 * Struct to store the input for the simulation for a single node.
 * All members must be set when passing it to a simulation.
 */
typedef struct {
    /**
    * x index of the node.
    */
    int x_index;
    /**
    * y index of the node.
    */
    int y_index;
    /**
    * Series of energy levels to be added at the specified node at the given tick.
    * Has #timeseries_ticks as length.
    */
    nodeval_t *timeseries;
    /**
    * Length of #timeseries.
    */
    int timeseries_ticks;
}
        nodeinputseries_t;

/**
 * Struct to store the status of one node. Includes the energy-level of the node, as well as the slope.
 */
typedef struct {
    /**
    * Energy-level of node
    */
    nodeval_t act;
    /**
    * Slope of node.
    */
    nodeval_t slope;
}
        nodestate_t;

/**
* Definition of the kernel-function interface.
*/
typedef int(*kernelfunc_t)(nodeval_t *, int, int, nodeval_t **, int, int);

/**
 * Struct to pass all execution information to a new thread
 * for executing a tick (or parts thereof).
 */
typedef struct {

    /**
     * Number of ticks in the simulation.
     */
    int num_ticks;

    /**
    * Milliseconds in between each simulation tick.
    */
    double tick_ms;

    /**
    * The number of total nodes in the first dimension of nodes.
    */
    int number_nodes_x;

    /**
    * The number of total nodes in the second dimension of nodes.
    */
    int number_nodes_y;

    /**
     * The number of nodes to observe.
     */
    int num_global_obervationnodes;

    /**
     * Timeseries to write the observations into.
     */
    nodetimeseries_t *global_observationnodes;

	/**
	* The number of nodes to observe in this context.
	*/
	int num_partial_obervationnodes;

	/**
	* Timeseries to write the observations into within this context,
	* points to time series held within the global context.
	*/
	nodetimeseries_t **partial_observationnodes;

    /**
    * 2D array of nodes with their current energy level.Size number_nodes_x * number_nodes_y.
    */
    nodeval_t **old_state;

    /**
    * 2D array of nodes with the new energy level.Values will be overwritten.Size number_nodes_x *
    * number_nodes_y.
    */
    nodeval_t **new_state;

    /**
     * 2D array of nodes with their slope from the last tick iteration level. Size number_nodes_x *
     * number_nodes_y.
     */
    nodeval_t **slopes;

    /**
    * 2D array containing the kernels of each node at each index.Each index node points to an array
    * containing(currently) two kernels, each(currently) containing 4 neighbouring noides.Dimensions: number_nodes_x *
    * number_nodes_y * 2 * 4.
    */
    nodeval_t ****kernels;

    /**
    * Node x index at which to start working in this thread (inclusive).
    */
    int thread_start_x;

    /**
    * Node x index at which to stop working in this thread (exclusive).
    */
    int thread_end_x;

    /**
     * Function pointer pointing to the kernel function for the direct neighborhood.
     */
    kernelfunc_t d_ptr;

    /**
     * Function pointer pointing to the kernel function for the indirect neighborhood.
     */
    kernelfunc_t id_ptr;

    /**
     * Number of inputs on the entire node grid.
     */
    int number_global_inputs;

    /**
     * Contains information about the coordinates and the values of all input nodes to be changed during
     * execution. All values must be set. Length: number_global_inputs.
     */
    nodeinputseries_t *global_inputs;

    /**
     * Number of inputs in the sub-grid of this partial simulation.
     * Sub-grid is defined by thread_start_x and thread_end_x.
     */
    int number_partial_inputs;

    /**
    * Points the the inputs
    * in the sub-grid of this partial simulation.
    * Sub-grid is defined by thread_start_x and thread_end_x.
    * Length: number_partial_inputs.
    */
    nodeinputseries_t **partial_inputs;

    /**
     * Barrier to wait at.
     */
    threadbarrier_t *barrier;
}
        partialsimulationcontext_t;

#endif
