#ifndef BRAINSIMULATION_H
#define BRAINSIMULATION_H

#include "definitions.h"
#include "utils.h"

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
 * @return Return-codes.
 */
int simulate(double tick_ms,
             int num_ticks,
             int number_nodes_x,
             int number_nodes_y,
             nodeval_t **nodes,
             int num_obervationnodes,
             nodetimeseries_t *oberservationnodes);

/**
 * Executes one tick of the simulation.
 *
 * @param simulationexecutioncontext The technical gobal context of the simulation.
 * Contains information on threading settings, etc.
 * @param tick_ms Milliseconds in between each simulation tick.
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
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
 * @return Return-codes.
 */
unsigned int execute_tick(simulationexecutioncontext_t *simulationexecutioncontext,
				double tick_ms, int number_nodes_x, int number_nodes_y, nodeval_t **old_state,
                 nodeval_t **new_state, nodeval_t **slopes, nodeval_t ****kernels, kernelfunc_t d_ptr,
                 kernelfunc_t id_ptr);

/**
* Executes a partial tick of the simulation, as defined by a partial tick context.
* Usually executed in a separate thread.
* @param context The partial context to handle in this call.
* @return Return-codes, usually 0.
*/
unsigned int execute_partial_tick(partialtickcontext_t * context);

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
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
 * @param state 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
 */
void add_input_influence(int tick_number, double tick_ms, int number_nodes_x, int number_nodes_y,
                         nodeval_t **state);

#endif
