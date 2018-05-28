#ifndef BRAINSIMULATION_H
#define BRAINSIMULATION_H

#include "definitions.h"
#include "utils.h"

/**
 * @file
 * Main entry points for the brainsimultion.
 */

//function declarations

/**
 * Simulates the brain.
 * @param tick_ms Milliseconds in between each simulation tick.
 * @param num_ticks The number of ticks to simulate.
 * This will also be the length of all timeseries contained in the returned
 * oberservationnodes.
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
 * @param nodes 2D array of nodes with their starting energy level.
 * @param num_obervationnodes The number of nodes to observe during simulation.
 * @param oberservationnodes The nodes to observe during simulation. x_index and y_index members
 * must be set. All other members will be overwritten with the simulation
 * results. Must have num_obervationnodes as length.
 * @param number_inputs The number of input nodes to be changed during execution.
 * @param inputs Contains information about the coordinates and the values of the input nodes to be changed during
 * execution. All values must be set. Length: number_inputs.
 * @return Return-codes.
 */
unsigned int simulate(double tick_ms,
                      int num_ticks,
                      int number_nodes_x,
                      int number_nodes_y,
                      nodeval_t **nodes,
                      int num_obervationnodes,
                      nodetimeseries_t *oberservationnodes,
                      int number_inputs,
                      nodeinputseries_t *inputs);

/**
* Executes the inner simulation in a multithreaded fashion. Called after setup of nodes, inputs, etc.
*
* @param executioncontext The technical gobal context of the simulation.
* Contains information on threading settings, etc.
* @param num_ticks The number of ticks in the simulation.
* @param tick_ms Milliseconds in between each simulation tick.
* @param number_nodes_x The number of nodes in the first dimension of nodes.
* @param number_nodes_y The number of nodes in the second dimension of nodes.
* @param num_obervationnodes The number of nodes to observe.
* @param observationnodes Pointers to the timeseries for the nodes to observe. Observations are written here.
* @param old_state 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
* @param new_state 2D array of nodes with the new energy level. Values will be overwritten. Size number_nodes_x *
* number_nodes_y.
* @param slopes 2D array of nodes with their slope from the last tick iteration level. Size number_nodes_x *
* number_nodes_y.
* @param kernels 4D array containing the kernels of each node at each index. Each index node points to an array
* containing (currently) two kernels, each (currently) containing 4 neighbouring noides. Dimensions: number_nodes_x *
* number_nodes_y * 2 * 4.
* @param d_ptr Function pointer pointing to the kernel function for the direct neighborhood.
* @param id_ptr Function pointer pointing to the kernel function for the indirect neighborhood.
* @param number_global_inputs Number of global inputs.
* @param global_inputs Inputs on the entire node field. Length: number_global_inputs
* @return Return-codes.
*/
unsigned int execute_simulation_multithreaded(executioncontext_t *executioncontext,
	int num_ticks, double tick_ms, int number_nodes_x, int number_nodes_y,
	int num_obervationnodes, nodetimeseries_t *observationnodes, nodeval_t **old_state,
	nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels, kernelfunc_t d_ptr, kernelfunc_t id_ptr,
	int number_global_inputs, nodeinputseries_t *global_inputs);

/**
* Executes the inner simulation in a singlethreaded fashion. Called after setup of nodes, inputs, etc.
*
* @param executioncontext The technical gobal context of the simulation.
* Contains information on threading settings, etc.
* @param num_ticks The number of ticks in the simulation.
* @param tick_ms Milliseconds in between each simulation tick.
* @param number_nodes_x The number of nodes in the first dimension of nodes.
* @param number_nodes_y The number of nodes in the second dimension of nodes.
* @param num_obervationnodes The number of nodes to observe.
* @param observationnodes Pointers to the timeseries for the nodes to observe. Observations are written here.
* @param old_state 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
* @param new_state 2D array of nodes with the new energy level. Values will be overwritten. Size number_nodes_x *
* number_nodes_y.
* @param slopes 2D array of nodes with their slope from the last tick iteration level. Size number_nodes_x *
* number_nodes_y.
* @param kernels 4D array containing the kernels of each node at each index. Each index node points to an array
* containing (currently) two kernels, each (currently) containing 4 neighbouring noides. Dimensions: number_nodes_x *
* number_nodes_y * 2 * 4.
* @param d_ptr Function pointer pointing to the kernel function for the direct neighborhood.
* @param id_ptr Function pointer pointing to the kernel function for the indirect neighborhood.
* @param number_global_inputs Number of global inputs.
* @param global_inputs Inputs on the entire node field. Length: number_global_inputs
* @return Return-codes.
*/
unsigned int execute_simulation_singlethreaded(executioncontext_t *executioncontext,
	int num_ticks, double tick_ms, int number_nodes_x, int number_nodes_y,
	int num_obervationnodes, nodetimeseries_t *observationnodes, nodeval_t **old_state,
	nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels, kernelfunc_t d_ptr, kernelfunc_t id_ptr,
	int number_global_inputs, nodeinputseries_t *global_inputs);

/**
 * Executes a partial simulation, as defined by a partial simulation context.
 * Usually executed in a separate thread. Executes the entire simulation on a bounded subset
 * of the node-field.
 * @param context The partial context to handle in this call.
 * @return Return-codes, usually 0.
 */
unsigned int execute_partial_simulation(partialsimulationcontext_t * context);

/**
* Executes a partial tick of the simulation.
* Usually executed in a separate thread.
* @param context The partial context to handle in this call.
* @return Return-codes, usually 0.
*/
unsigned int execute_partial_tick(partialsimulationcontext_t *context);

/**
 * Extracts and stores/saves the information into the specified observation nodes.
 *
 * @param ticknumber The current tick number, i.e., the tick number to store.
 * @param num_obervationnodes The number of observation nodes.
 * @param observationnodes An array containing all observation nodes. All values in the struct elements must be already
 * initialized. Length: num_observationnodes.
 * @param state The current state to store.
 */
void extract_observationnodes(int ticknumber, int num_obervationnodes, nodetimeseries_t *observationnodes,
                              nodeval_t **state);

/**
 * Adds the influence of the defined input nodes to the current state.
 *
 * @param tick_number The current tick number.
 * @param tick_ms Milliseconds in between each simulation tick.
 * @param state 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
 * @param number_global_inputs The number of input nodes to be changed.
 * @param global_inputs Contains information about the coordinates and the values of the input nodes to be changed.
 * Length: number_inputs
 */
void process_global_inputs(int tick_number, double tick_ms,
                    nodeval_t **state, int number_global_inputs, nodeinputseries_t *global_inputs);

/**
* Adds the influence of the defined input nodes to the current state.
*
* @param tick_number The current tick number.
* @param tick_ms Milliseconds in between each simulation tick.
* @param state 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
* @param number_partial_inputs The number of input nodes to be changed.
* @param partial_inputs Contains pointers to the input nodes to be changed.
* Length: number_partial_inputs
*/
void process_partial_inputs(int tick_number, double tick_ms,
	nodeval_t **state, int number_partial_inputs, nodeinputseries_t **partial_inputs);

/**
* Adds the influence of the defined input node to the current state.
*
* @param tick_number The current tick number.
* @param tick_ms Milliseconds in between each simulation tick.
* @param state 2D array of nodes with their current energy level.
* @param input The single input to be updated.
*/
void process_input(int tick_number, double tick_ms,
	nodeval_t **state, nodeinputseries_t *input);

#endif
