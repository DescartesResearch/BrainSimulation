#ifndef BRAINSETUP_H
#define BRAINSETUP_H

#include "definitions.h"

/**
 * @file
 * Setup all node arrays to be passed to the brainsimulation.
 */

/**
 * Initializes num_oberservationnodes timeseries structs and returns them in an array.
 * @param num_oberservationnodes The number of timeseries to create.
 * @param x_indices The x indices of the nodes to observe. Must have num_obervationnodes as length.
 * @param y_indices The y indices of the nodes to observe. Must have num_obervationnodes as length.
 * @param num_timeseries_elements The number of elements for the timeseries to hold.
 * Timeseries memory is allocated as part of initialization.
 * @return Array of newly initialized timeseries structs. Array has num_oberservationnodes as length.
 */
nodetimeseries_t *init_observation_timeseries(const int num_oberservationnodes,
                                              const int *x_indices, const int *y_indices,
                                              const int num_timeseries_elements);

/**
 * Sets a start time energy state for the node field. All unspecified nodes start with 0.
 * @param number_nodes_x The x dimension of the node field.
 * @param number_nodes_y The y dimension of the node field.
 * @param nodes The 2D node field to initialize.
 * @param num_start_levels The number of nodes in the field to initialize with non-zero values.
 * @param start_levels The energy levels of the starting non-zero nodes. Must have num_start_levels length.
 * @param start_nodes_x The x indices of the starting non-zero nodes. Must have num_start_levels length.
 * @param start_nodes_y The y indices of the starting non-zero nodes. Must have num_start_levels length.
 */
void init_start_time_state(const int number_nodes_x, const int number_nodes_y, nodeval_t **nodes,
                           const int num_start_levels, const nodeval_t *start_levels,
                           const int *start_nodes_x, const int *start_nodes_y);

/**
 * Reads and parses the inputs defined as csv files to be added to the given nodes during the simulation.
 *
 * @param number_of_inputnodes The number of input nodes and files.
 * @param x_indices An array of x-coordinates of the nodes. Defines, which node will be added the values written in
 * the corresponding csv file. Length: number_of_inputnodes.
 * @param y_indices An array of y-coordinates of the nodes. Defines, which node will be added the values written in
 * the corresponding csv file. Length: number_of_inputnodes.
 * @param inputnodefilenames The .csv filenames to read. Order must match the order of x_coordinates and y_coordinates.
 * Length: number_of_inputnodes.
 * @param number_of_elements Elements assigning each node the number of input elements in the corresponding file.
 * Length: number_of_inputnodes.
 * @return The array of input nodes as read from the given filepaths. Length: number_of_inputnodes.
 */
nodeinputseries_t *read_input_behavior(const int number_of_inputnodes, const int *x_indices, const int *y_indices,
                                       const char **inputnodefilenames, const int *number_of_elements);

/**
 * Generates a specified number of samples of a discretized sinoidal timeseries with the specified frequency and
 * returns it.
 *
 * @param hz The desired frequency in Hz.
 * @param tick_ms The milliseconds in between each simulation tick, i.e., the required resolution in milliseconds.
 * @param number_of_samples The number of samples to generate.
 * @return A series of doubles. Length: number_of_samples.
 */
double *generate_sin_time_series(int hz, double tick_ms, int number_of_samples);

/**
 * Generates a discretized sinoidal timeseries with the specified frequency and returns it. Automatically detects the
 * period, and returns exactly only period. Should be preferred way to generate the time-series.
 *
 * @param hz The desired frequency in Hz.
 * @param tick_ms The milliseconds in between each simulation tick, i.e., the required resolution in milliseconds.
 * @return A series of doubles. Length: Period of the frequency.
 */
double *generate_sin_frequency(int hz, double tick_ms);

/**
 * Calcuated the period length of the given frequency at the specified resolution, i.e., the number of samples to
 * generate, until the period is reached.
 *
 * @param hz The desired frequency in Hz.
 * @param tick_ms The milliseconds in between each simulation tick, i.e., the required resolution in milliseconds.
 * @return The number of samples required for one period.
 */
int calculate_period_length(int hz, double tick_ms);


/**
 * Generates input timeseries for the given nodes with the specified frequency. The different time-series may vary as
 * the minimal period length is detected.
 * @param number_of_inputnodes The number of input nodes and frequencies to generate.
 * @param x_indices An array of x-coordinates of the nodes. Defines which node will be modified with the defined
 * frequency. Length: number_of_inputnodes.
 * @param y_indices An array of y-coordinates of the nodes. Defines which node will be modified with the defined
 * frequency. Length: number_of_inputnodes.
 * @param frequencies An array describing the desired frequencies to generate for each node. Length:
 * number_of_inputnodes.
 * @param tick_ms The milliseconds in between each simulation tick, i.e., the required resolution in milliseconds.
 * This parameter influences the number of generated samples, as frequency is defined in periods/second (Hz).
 * @return The array of input nodes initialized with the specified frequencies. Length: number_of_inputnodes.
 */
nodeinputseries_t *generate_input_frequencies(const int number_of_inputnodes, const int *x_indices,
                                              const int *y_indices, const int *frequencies, double tick_ms);

#endif
