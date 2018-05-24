#ifndef DEFINITIONS_H
#define DEFINITIONS_H

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

//types

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
}
partialtickcontext_t;

#endif
