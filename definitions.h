#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/**
 * Common definitions.
 */

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

#endif
