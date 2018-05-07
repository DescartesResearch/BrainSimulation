#ifndef HELPERS_H
#define HELPERS_H

#include "definitions.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <pthread.h>
#endif

/**
 * Common helper functions.
 */

/**
 * Platform-independent thread handle.
 */
#ifdef _WIN32
	typedef HANDLE threadhandle_t;
#else
	typedef pthread_t threadhandle_t;
#endif

/**
* Struct to pass all execution information to a new thread.
*/
typedef struct {
	/**
	 * Milliseconds in between each simulation tick.
	 */
	int tick_ms;

	/**
	 * The number of nodes in the first dimension of nodes.
	 */
	int number_nodes_x;

	/**
	 * The number of nodes in the second dimension of nodes.
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
threadcontext_t;

/**
* Allocates a new 2d array with m pointers, pointing to a list of n elements.
* @param m The number of nodes in the first dimension (x-axis).
* @param n The number of nodes in the second dimension (y-axis).
*
* @return A two-dimensional array of size m*n.
*/
nodeval_t **alloc_2d(const int m, const int n);


/**
* Allocates a new 4d array with m pointers, pointing to a list of n elements, pointing to a list of o elements,
* pointing to a list of p elements.
* @param m The number of nodes in the first dimension.
* @param n The number of nodes in the second dimension.
* @param o The number of nodes in the third dimension.
* @param p The number of nodes in the fourth dimension.
*
* @return A four-dimensional array of size m*n*o*p.
*/
nodeval_t ****alloc_4d(const int m, const int n, const int o, const int p);

/**
 * Returns the number of processor cores online in the system.
 * @return The number of processors online in the system.
 */
int system_processor_online_count();

/**
 * Creates a new platform-specific thread with the context and starts it.
 * @param callback The callback function to run. The function must take a threadcontext struct and return nothing.
 * @param context The threadcontext struct to pass.
 * @return A handle for the running thread.
 */
threadhandle_t * create_and_run_simulation_thread(void (* callback)(threadcontext_t * ), threadcontext_t * context);

/**
 * Joins all threads and then closes them. Frees all thread handles.
 * @param handles Array of thread handle pointers.
 * @param num_threads length of handles array.
 */
void join_and_close_simulation_threads(threadhandle_t ** handles, const int num_threads);

#endif
