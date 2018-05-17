#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * Common definitions.
 */

/**
 * 1 if multithreading is enabled, 0 otherwise.
 */
#define MULTITHREADING 0

/**
 * Int factor to modifiy the number of threads in relation to the number
 * of logical processors on the execution system.
 */
#define THREADFACTOR 1


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
* Struct to pass all execution information to a new thread
* for executing a tick (or parts thereof).
*/
typedef struct {
	/**
	* Milliseconds in between each simulation tick.
	*/
	int tick_ms;

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
}
partialtickcontext_t;

#endif
