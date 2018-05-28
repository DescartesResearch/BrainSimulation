#ifndef UTILS_H
#define UTILS_H

#include "definitions.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <pthread.h>
	#include <sys/time.h>
#endif

/**
 * @file
 * Common helper functions.
 */



/**
 * Struct to pass a simulation's technical execution information
 * to the ticks. Contains information on thread-counts and global handles.
 */
typedef struct {
    /**
     * Number of threads in use by the simulation.
     */
    unsigned int num_threads;

    /**
     * Pointers to thread handles provided by the operating system.
     */
    threadhandle_t **handles;

    /**
     * Array of contexts for the different threads. Has num_threads length.
     */
    partialsimulationcontext_t *contexts;

	/**
	 * Synchronization barrier to be used by all threads that run in this execution.
	 */
	threadbarrier_t barrier;
}
executioncontext_t;


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
 * Sets all values of the given array to zero.
 * @param nodes 2D array to be modified. Dimension x * y.
 * @param number_nodes_x Number of nodes in the first dimension.
 * @param number_nodes_y Number of nodes in the second dimension.
 */
void init_zeros_2d(nodeval_t **nodes, int number_nodes_x, int number_nodes_y);

/**
 * Returns the number of processor cores online in the system.
 * @return The number of processors online in the system.
 */
int system_processor_online_count();

/**
 * Creates a new platform-specific thread with the context and starts it.
 * @param callback The callback function to run.
 * The function must take a threadcontext struct and returns 0 or an error code.
 * @param context The threadcontext struct to pass.
 * @return A handle for the running thread.
 */
threadhandle_t *
create_and_run_simulation_thread(unsigned int(*callback)(partialsimulationcontext_t *), partialsimulationcontext_t *context);

/**
 * Initializes tread barrier.
 * @param barrier Barrier to initialize.
 * @param number_threads: The number of threads that the barrier should be configured to block.
 */
void init_thread_barrier(threadbarrier_t *barrier, const unsigned int number_threads);

/**
 * Destroys the thread barrier.
 * @param barrier Barrier to destroy.
 */
void destroy_thread_barrier(threadbarrier_t *barrier);

/**
* Waits at the barrier. Returns 1 if for the management thread and 0 for all other threads.
* @param barriers Barrier to wait at.
*/
unsigned int wait_at_barrier(threadbarrier_t *barrier);

/**
 * Joins all threads and then closes them. Frees all thread handles.
 * @param handles Array of thread handle pointers.
 * @param num_threads length of handles array.
 */
void join_and_close_simulation_threads(threadhandle_t **handles, const int num_threads);

/**
 * Convenience function to pass all members to a partialsimulationcontext_t in a single line.
 * Automatically derives \ref partialsimulationcontext_t.number_partial_inputs and
 * \ref partialsimulationcontext_t.partial_inputs from the global inputs.
 * @param context Pointer to the context.
 * @param num_ticks The number of ticks in the simulation.
 * @param tick_ms Milliseconds in between each simulation tick.
 * @param number_nodes_x The number of total nodes in the first dimension of nodes.
 * @param number_nodes_y The number of total nodes in the second dimension of nodes.
 * @param num_obervationnodes The number of nodes to observe.
 * @param observationnodes Pointers to the timeseries for the nodes to observe. Observations are written here.
 * @param old_state 2D array of nodes with their current energy level.Size number_nodes_x * number_nodes_y.
 * @param new_state 2D array of nodes with the new energy level.Values will be overwritten.Size number_nodes_x *
 * number_nodes_y.
 * @param slopes 2D array of nodes with their slope from the last tick iteration level. Size number_nodes_x *
 * number_nodes_y.
 * @param kernels 2D array containing the kernels of each node at each index.Each index node points to an array
 * containing(currently) two kernels, each(currently) containing 4 neighbouring noides.Dimensions: number_nodes_x *
 * number_nodes_y * 2 * 4.
 * @param d_ptr Function pointer pointing to the kernel function for the direct neighborhood.
 * @param id_ptr Function pointer pointing to the kernel function for the indirect neighborhood.
 * @param number_global_inputs Number of all inputs on the entire node-grid inputs to be processed.
 * @param global_inputs Inputs on the entire node-grid inputs to be processed. Length: number_partial_inputs.
 * Partial inputs in the sub-grid (defined by thread_start_x and thread_end_x) are automatically derived
 * from this global list. 
 * @param thread_start_x Node x index at which to start working in this thread (inclusive).
 * @param thread_end_x Node x index at which to stop working in this thread (exclusive).
 * @param The barrier for threads to wait at. May be NULL if not used.
 */
void init_partial_simulation_context(partialsimulationcontext_t *context, int num_ticks, double tick_ms,
                               int number_nodes_x, int number_nodes_y,
							   int num_obervationnodes, nodetimeseries_t *observationnodes,
							   nodeval_t **old_state,
                               nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels,
                               kernelfunc_t d_ptr, kernelfunc_t id_ptr,
							   int number_global_inputs, nodeinputseries_t *global_inputs,
                               int thread_start_x, int thread_end_x, threadbarrier_t *barrier);

/**
 * Initializes a simuatlionexecutioncontext.
 * Derives the number of threads for execution and writes the result to context->num_threads.
 * Allocates memory for the arrays in the context.
 * @param context The context to initialize.
 */
void init_executioncontext(executioncontext_t *context);

/**
 * Writes the given array to a .csv with every entry in its own line.
 *
 * @param filename Name of the file to write to.
 * @param length Number of entries to write.
 * @param values The values to write. Length length.
 */
void output_to_csv(char *filename, int length, nodeval_t *values);

/**
 * Get the time of day.
 * Platform-independent abstraction of gettimeofday in sys/time.h.
 * @param tp The timeval to write back to.
 * @return Error codes.
 */
int get_daytime(struct timeval *tp);

/**
 * Parses the .csv file, given by the specific filename and writes its contents into the given timeseries of the
 * nodeinputseries_t.
 * @param timeseries The nodeinputseries_t struct to write the parsed file into.
 * @param filename The filename of the .csv file.
 */
void parse_file(nodeinputseries_t timeseries, const char *filename);

/**
 * Gets the n-th integer value of a comma-separated (.csv) line.
 *
 * @param line The line to parse.
 * @param num The index of the column to get, i.e. n.
 * @return The parsed integer.
 */
int get_field(char *line, int num);

#endif
